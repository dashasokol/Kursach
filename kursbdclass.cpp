#include <iostream>         // printf, scanf, NULL
#include <cstdlib>          // malloc, free, rand
#include <cstring>          // strstr, str[n]cat, strlen
#include <string>
#include "helpfun.h"        // Заголовочный файл helpfun
#include "kursbdclass.h"    // Заголовочный файл этого модуля

/* инициализация класса */
KursBDClass::KursBDClass()
{
    table_length = 0;   // обнуляем количество элементов
    order_len = 0;      // обнуляем размер массива сортировки
    header_col = 0;
    sort_field = -1;
}

KursBDClass::~KursBDClass()
{
    table_length = 0;   // обнуляем количество элементов
    order_len = 0;      // обнуляем размер массива сортировки
    header_col = 0;
    sort_field = -1;

    memset(tb, 0, sizeof(tb));
    memset(table_header, 0, sizeof(table_header));
    memset(order, 0, sizeof(order));
}

int KursBDClass::getHeader(std::string string_to_parse)
{
    unsigned int h_sep_pos;

    string_to_parse = string_to_parse.substr(1, string_to_parse.size() - 1);

    while ((h_sep_pos = string_to_parse.find(SYM_HEADER_SEPARATOR)) > 0 &&
            h_sep_pos < string_to_parse.size())
    {
        table_header[header_col++] = string_to_parse.substr(0, h_sep_pos);

        string_to_parse = string_to_parse.substr(h_sep_pos, string_to_parse.size() - (h_sep_pos + 1));
    }

    if (string_to_parse.size() == 0)
        return END_EXIT;

    table_header[header_col++] = string_to_parse.substr(0, string_to_parse.size());

    return END_OK;
}

/**
 * fn FILE *open_and_parse(char *BD_file_name, struct table *data_table, unsigned int *tb_len)
 * @brief Функция открывает и читает БД из файла
 * @param BD_file_name - Полное имя файла БД
 * @param data_table - структура с данными
 * @param tb_len - размер БД
 * @return Дескриптор файла
 */
int KursBDClass::open(std::string BD_file_name)
{
    char buff[LINELEN];        // буфер
    int str_num = 0;           // номер строки
    FILE *out_file;            // дескриптор файла
    std::string string_to_parse;

    /* открываем файл */
    out_file = fmopen(BD_file_name.c_str(), "r+", "KursBDClass::open");

    /* Не вышло открыть файл */
    if (!out_file)
        return END_OPEN_FAIL;

    /* считываем построчно данные в структуру */
    while(fgets(buff, LINELEN, out_file))
    {
        string_to_parse = buff;

        /* если найден заголовок, пропустить строку */
        if (string_to_parse.find(SYM_HEADER) == 0)
        {
            if (header_col == 0)
                getHeader(string_to_parse); // загрузить заголовок и отметить, что он найден
        }
        else
        {
            if (header_col == 0)
                return END_WRONG_FORMAT;

            /* парсим строку */
            if (parse(string_to_parse) == END_WRONG_FORMAT)
            {
                /* строка имеет неправильный формат */
                fprintf(stderr, "Line %d: Wrong string format\n", str_num);
            }

            /* увеличиваем номер строки */
            str_num++;
        }
    }

    return END_OK;
}

/**
 * @fn void close()
 * @brief Функция закрытия БД
 */
void KursBDClass::close()
{
    fclose(bd_out_file); // закрытие файла
}

/**
 * @fn int parse(char *string_to_parse, struct table *data_table, unsigned int *tb_len)
 * @brief Функция обработки строки
 * @param string_to_parse - строка
 * @param data_table - указатель на структуру БД
 * @param tb_len - количество записей в таблице
 * @return Код удачного завершения / ошибки
 */
int KursBDClass::parse(std::string string_to_parse)
{
    unsigned int pos = 0; // позиция для чтения
    unsigned int i = 0;
    unsigned int el_num;

    for (i = 0; i<header_col; i++)
    {
        el_num = table_length * header_col + i;
        tb[el_num].number = table_length;

        tb[el_num].field = table_header[i];

        /* извлекаем значение */
        if ((pos = getValue(&tb[el_num].value, string_to_parse)) != (unsigned int) END_WRONG_FORMAT)
            string_to_parse = string_to_parse.substr(pos, string_to_parse.size() - pos);
    }

    table_length++;

    return END_OK;
}

/**
 * @fn int getValue(char *var, char *val)
 * @brief Функция, получающая строковое значение
 * @param var - переменная для записи значения
 * @param val - строка для парсинга
 * @return Код удачного завершения / ошибки
 */
int KursBDClass::getValue(std::string *var, std::string val)
{
    int pos;    // позиция для чтения

    /* поиск символа разделителя */
    if (val.find(SYM_STRING_END) == 0)
    {
        /* найдена строка с кавычками */
        val = val.substr(1, val.size()-1);
        pos = val.find(SYM_STRING_END);
    }
    else
        /* строка без кавычек */
        pos = val.find(SYM_STRING_END);

    /* если найдено пустое значение */
    if (pos <= 0)
    {
        /* если пустое значение, считать, что значение "\0" */
        *var = "";
        return END_OK;
    }

    /* найдено не пустое значение */
    *var = val.substr(1, pos);

    return pos;
}

/**
 * @fn int clean_db(FILE *bd)
 * @brief Функция отчищает файл БД
 * @param bd - Дескриптор файла
 * @return Код удачного завершения / ошибки
 */
int KursBDClass::clean_db(FILE *bd)
{
    /* отчистить файл полностью */
    return fmclean(bd);
}

/**
 * void stringInsert(char *string, struct table insert_value)
 * @brief Функция вставляет строку содержащую данные одной записи из БД в строку вывода
 * @param string - Строка вывода
 * @param insert_value - Структура содержащая одну запись из таблицы БД
 */
void KursBDClass::stringInsert(unsigned int number)
{
    unsigned int i;

    for (i = 0; i < header_col; i++)
    {
        string_for_write += tb[(number * header_col + i)].value;
        string_for_write += i == header_col-1 ? ";" : "\n";
    }
}

/**
 * @fn int write_buffer(char *s_file_name)
 * @brief Функция записывает строку вывода в указанный файл
 * @param s_file_name - Имя файла для записи
 * @return Код удачной записи / ошибки
 */
int KursBDClass::write_buffer(std::string s_file_name)
{
    /* открываем файл для select */
    FILE *sel_file = fmopen(s_file_name.c_str(), "r+", "KursBDClass::select");

    /* стираем старые данные */
    clean_db(sel_file);

    /* добавляем данные в файл */
    return add_to_bd(sel_file, string_for_write.c_str());
}

/**
 * @fn int write_buffer()
 * @brief Функция записывает строку вывода в основной файл
 * @return Код удачной записи / ошибки
 */
int KursBDClass::write_buffer()
{
    /* стираем старые данные */
    clean_db(bd_out_file);

    /* добавляем данные в файл */
    return add_to_bd(bd_out_file, string_for_write.c_str());
}
/**
 * @fn void select(char *field, char *value)
 * @brief Функция ищет записи в таблице по послю, содержащему строковые значения
 * @param field - имя поля для поиска
 * @param value - значение для поиска
 */
void KursBDClass::select(std::string field, std::string value)
{
    unsigned int i; // счетчик
    int h_num = -1;

    /* обнуляем массив по которому определяется порядок вывода */
    for (i = 0; i < header_col; i++)
        if (field == value)
            h_num = i;

    order_clear(0);

    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i*header_col+h_num].value == value)
                order[order_len++] = i;

    }

    /* записать все найденные записи в строку вывода */
    get_order_string();
}

/**
 * @fn void get_order_string()
 * @brief Функция записи данных в строку вывода
 */
void KursBDClass::get_order_string()
{
    unsigned int i; // счетчик

    /* обнуляем строку */
    string_for_write.clear();

    string_for_write = "#";
    /* записываем в строку вывода описание таблицы */
    for (i = 0; i < header_col; i++)
    {
        string_for_write += table_header[i];
        string_for_write += i == header_col-1 ? "" : "|";
    }

    /* записывем в строку вывода записи из таблицы согласно массиву порядка */
    for (i = 0; i < order_len; i++)
        stringInsert(i);
}

/**
 * @fn void order_clear(unsigned int length)
 * @brief Отчистка массива, по кторому определяется порядок
 * @param length - итоговая длинна массива, которую должен иметь массив
 */
void KursBDClass::order_clear(unsigned int length)
{
    unsigned int i; //счетчик

    /* расставляем  элементы массива в порядке возрастания */
    for (i = 0; i < length; i++)
        order[i] = i;

    /* указываем итоговый размер массива */
    order_len = length;
}

/**
 * @fn void insert(struct table insert_value)
 * @brief Функция вставляет новое значение в строку вывода
 * @param insert_value - Запись для добавления в таблицу
 */
void KursBDClass::add(std::string value)
{
    /* добаляем запись в таблицу */
    parse(value);
}

/**
 * @fn int add_to_bd(FILE *bd, char *string)
 * @brief Оболочка для записи данных в файл БД
 * @param bd - Дескриптор файла
 * @param string - Строка для записи
 * @return Код удачной записи / ошибки записи
 */
int KursBDClass::add_to_bd(FILE *bd, const char *string)
{
    /* записываем весь массив символов в файл с позиции 0 */
    return fmwrite(bd, string, strlen(string));
}

int KursBDClass::isOrder(unsigned int number)
{
    unsigned int i;

    for (i = 0; i < order_len; i++)
        if (number == order[i])
            return END_OK;

    return END_EXIT;
}


/**
 * @fn void del(char *field, char *value)
 * @brief Удаление всех записей, из поля соответствуюх указанному строковому значению
 * @param field - Название поля
 * @param value - Значение
 */
void KursBDClass::del(std::string field, std::string value)
{
    unsigned int i, j; // счетчик
    int h_num = -1;
    unsigned int new_length = 0;

    /* обнуляем массив по которому определяется порядок вывода */
    for (i = 0; i < header_col; i++)
        if (field == value)
            h_num = i;

    /* обнуляем массив по которому определяется порядок вывода */
    order_clear(0);

    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[(i-1)*header_col+h_num].value != value)
                order[order_len++] = i;
    }

    for (i = 0; i < table_length; i++)
    {
        if (isOrder(i) == END_EXIT)
        {
            for (j = 0; j < header_col; j++)
                tb[i*header_col+j].number = new_length;

            new_length++;
        }
    }
}

/**
 * @fn int sort(char *field)
 * @brief Оболочка для функции сортировки по полю
 * @param field - Название поля
 * @return Код удачной сортировки / ошибки
 */
int KursBDClass::sort(std::string field)
{
    /* переопределяем массив порядка */
    order_clear(table_length);

    /* сортируем данные */
    int ret = sort_table(field);

    /* записать все записи в строку вывода */
    get_order_string();

    return ret;
}

/**
 * @fn int sort_table(int *ord, char *field, struct table *data_table, unsigned int tb_len)
 * @brief Функция сортировки одного массива относительно значений другого
 * @param ord - Сортируемый массив
 * @param field - Массив, относительно которого сортируется ord
 * @param data_table - Указатель на таблицу, которую нужно упорядочить
 * @param tb_len - Размер таблицы
 * @return Код удачной сортировки / ошибки
 */
int KursBDClass::sort_table(std::string field)
{
    unsigned int i; // счетчик
    char *values[TABLELINES];

    /* обнуляем массив по которому определяется порядок вывода */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            sort_field = i;

    if (sort_field >= 0)
    {
        for (i = 0; i < table_length; i++)
            strcpy(values[i], tb[i*header_col+sort_field].value.c_str());

        /* сортируем массив */
        qsort_dmas((int *) order, values, table_length-1, T_CHAR);
    }
    else
    {
        /* имя поля не найдено */
        fprintf(stderr, "Wrong field name %s\n", field.c_str());
        return END_NOT_FOUND;
    }

    return END_OK;
}

/**
 * @fn void insert_sort(struct table insert_value, char *field)
 * @brief Вставка в отсортированную БД
 * @param insert_value - Запись для добавления в таблицу
 * @param field - Имя поля, по которому отсортирована таблица
 * @return Код удачной сортировки / ошибки
 */
void KursBDClass::insert(std::string value)
{
    unsigned int i, j;
    /* добаляем запись в таблицу */
    parse(value);

    sort_table(table_header[sort_field]);

    for(i = 0; i < table_length; i++)
        for(j = 0; j < header_col; j++)
            tb[i*header_col+j].number = order[i];
}

