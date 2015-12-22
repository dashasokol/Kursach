#include <iostream>         // printf, scanf, NULL
#include <cstdlib>          // malloc, free, rand
#include <cstring>          // strstr, str[n]cat, strlen
#include <string>           // std::string
#include "helpfun.h"        // Заголовочный файл helpfun
#include "kursbdclass.h"    // Заголовочный файл этого модуля

/**
 * fn KursBDClass()
 * @brief Конструктор класса
 */
KursBDClass::KursBDClass()
{
    table_length = 0;   // обнуляем количество элементов
    order_len = 0;      // обнуляем размер массива сортировки
    header_col = 0;     // обнуляем количество полей таблицы
    sort_field = -1;    // обнуляем id поля сортировки
}

/**
 * fn KursBDClass()
 * @brief Деструктор класса
 */
KursBDClass::~KursBDClass()
{
    table_length = 0;   // обнуляем количество элементов
    order_len = 0;      // обнуляем размер массива сортировки
    header_col = 0;     // обнуляем количество полей таблицы
    sort_field = -1;    // обнуляем id поля сортировки
}

/**
 * fn int getHeader(std::string string_to_parse)
 * @brief Функция получает шапку таблицы
 * @param string_to_parse - Строка для парсинга
 * @return Код завершения (END_EXIT - найден лишний символ "|")
 */
int KursBDClass::getHeader(std::string string_to_parse)
{
    unsigned int h_sep_pos; // позиция символа разделителя

    /* убираем из строки символ "#" */
    string_to_parse = string_to_parse.substr(1, string_to_parse.size() - 1);

    /* пока есть символ разделитель */
    while ((h_sep_pos = string_to_parse.find(SYM_HEADER_SEPARATOR)) < string_to_parse.size())
    {
        /* добавляем новое значение */
        table_header[header_col++] = string_to_parse.substr(0, h_sep_pos);

        /* обрезаем строку */
        string_to_parse = string_to_parse.substr(h_sep_pos + 1, string_to_parse.size() - h_sep_pos);
    }

    /* после последнего символа "|" нет ничего */
    if (string_to_parse.size() == 0)
        return END_EXIT;

    /* ищем конец строки */
    if ((h_sep_pos = string_to_parse.find("\n")) > string_to_parse.size())
        h_sep_pos = string_to_parse.size();

    /* добавляем последнее значение */
    table_header[header_col++] = string_to_parse.substr(0, h_sep_pos);

    return END_OK;
}

/**
 * fn FILE *open_and_parse(char *BD_file_name)
 * @brief Функция открывает и читает БД из файла
 * @param BD_file_name - Полное имя файла БД
 * @return Код завершения / ошибка
 */
int KursBDClass::open(std::string BD_file_name)
{
    char buff[LINELEN];             // буфер
    int str_num = 0;                // номер строки
    std::string string_to_parse;    // строка для разбора

    /* открываем файл */
    bd_out_file = fmopen(BD_file_name.c_str(), "r+", "KursBDClass::open");

    /* не вышло открыть файл */
    if (!bd_out_file)
        return END_OPEN_FAIL;

    /* считываем построчно данные в структуру */
    while(fgets(buff, LINELEN, bd_out_file))
    {
        string_to_parse = buff;

        /* если найден заголовок */
        if (string_to_parse.find(SYM_HEADER) == 0)
        {
            /* загрузить заголовок и отметить, что он найден */
            if (header_col == 0)
                getHeader(string_to_parse);
        }
        else
        {
            /* если нет шапки, но найден другой символ - вернуть ошибку */
            if (header_col == 0)
                return END_WRONG_FORMAT;

            /* парсим строку */
            if (add(string_to_parse) == END_WRONG_FORMAT)
            {
                /* строка имеет неправильный формат */
                fprintf(stderr, "Строка %d: Неправильный формат \n", str_num);
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
 * @fn int add(std::string string_to_parse)
 * @brief Функция обработки строки
 * @param string_to_parse - строка
 * @return Код удачного завершения / ошибки
 */
int KursBDClass::add(std::string string_to_parse)
{
    unsigned int i = 0;     // счетчик
    unsigned int el_num;    // номер записи

    /* добавляем элементы записи в структуру данных */
    for (i = 0; i<header_col; i++)
    {
        el_num = table_length * header_col + i;
        tb[el_num].number = table_length;

        tb[el_num].field = table_header[i];

        /* извлекаем значение */
        getValue(&tb[el_num].value, &string_to_parse);
    }

    /* увеличиваем количество добавленных записей */
    table_length++;
    
    /* обнуляем массив order */
    order_clear(table_length);

    return END_OK;
}

/**
 * @fn int getValue(std::string *var, std::string *val)
 * @brief Функция извлекает значение
 * @param var - переменная для записи значения
 * @param val - строка для парсинга
 * @return Код удачного завершения / ошибки
 */
void KursBDClass::getValue(std::string *var, std::string *val)
{
    unsigned int pos;    // позиция для чтения

    /* поиск символа разделителя */
    if ((pos = val->find(SYM_SEPARATOR)) > val->size())
        /* найдеа последняя часть строки */
        if (val->find("\n") < val->size())
            /* найден перенос строки */
            *var = val->substr(0, val->size() - 1);
        else
            /* перенос строки не найден */
            *var = *val;
    else
    {
        /* найдена очередная часть строки (не конец) */
        *var = val->substr(0, pos);

        /* обрезаем строку для парсинга */
        *val = val->substr(pos+1, val->size() - pos);
    }
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
 * void stringInsert(unsigned int number)
 * @brief Функция вставляет строку, содержащую данные одной записи из БД, в строку вывода
 * @param number - Номер записи
 */
void KursBDClass::stringInsert(unsigned int number)
{
    unsigned int i; // счетчик

    /* добавляем запись в строку вывода */
    for (i = 0; i < header_col; i++)
    {
        string_for_write += tb[(number * header_col + i)].value;
        string_for_write += i == header_col-1 ? "\n" : ";";
    }
}

/**
 * @fn int write_buffer(std::string s_file_name)
 * @brief Функция записывает строку вывода в указанный файл
 * @param s_file_name - Имя файла для записи
 * @return Код удачной записи / ошибки
 */
int KursBDClass::write_buffer(std::string s_file_name)
{
	/* записать найденные записи в строку вывода */
    get_order_string();
	
    /* открываем файл */
    FILE *file = fmopen(s_file_name.c_str(), "r+", "");

    /* стираем старые данные */
    clean_db(file);

    /* добавляем данные в файл */
    return add_to_bd(file, string_for_write.c_str());
}

/**
 * @fn int write_buffer()
 * @brief Функция записывает строку вывода в основной файл
 * @return Код удачной записи / ошибки
 */
int KursBDClass::write_buffer()
{
	/* записать найденные записи в строку вывода */
    get_order_string();
	
    /* стираем старые данные */
    clean_db(bd_out_file);

    /* добавляем данные в файл */
    return add_to_bd(bd_out_file, string_for_write.c_str());
}

/**
 * @fn void select(std::string field, std::string value)
 * @brief Функция ищет записи в таблице, по полю, содержащему строковые значения
 * @param field - имя поля для поиска
 * @param value - значение для поиска
 */
void KursBDClass::select(std::string field, std::string value)
{
    unsigned int i; // счетчик
    int h_num = -1; // номер поля

    /* ищем номер поля */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            h_num = i;

    /* обнуляем массив order */
    order_clear(0);

    /* ищем записи, соответствующие условию */
    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i*header_col+h_num].value == value)
                /* добавляем найденный номер записи в order */
                order[order_len++] = i;
    }

    /* записать все найденные записи в строку вывода */
    get_order_string();
}
/**
 * @fn void next_select(std::string field, std::string value)
 * @brief Функция продолжает искать записи в таблице, по полю, содержащему строковые значения
 * @param field - имя поля для поиска
 * @param value - значение для поиска
 */
void KursBDClass::next_select(std::string field, std::string value)
{
    unsigned int i; // счетчик
    int h_num = -1; // номер поля
    unsigned int ord[TABLELINES]; // массив для запоминания order
    unsigned int ord_len = order_len; // переменная для запоминания order_len

    /* копируем массив order в ord */
    for (i = 0; i < order_len; i++)
        ord[i] = order[i];

    /* ищем номер поля */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            h_num = i;

    /* обнуляем массив order */
    order_clear(0);

    /* ищем записи, соответствующие условию */
    if (h_num >= 0)
    {
        for (i = 0; i < ord_len; i++)
            if (tb[ord[i]*header_col+h_num].value == value)
                /* добавляем найденный номер записи в order */
                order[order_len++] = ord[i];
    }
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
    /* записываем, в строку вывода, описание таблицы */
    for (i = 0; i < header_col; i++)
    {
        string_for_write += table_header[i];
        string_for_write += i == header_col-1 ? "" : "|";
    }
    string_for_write += "\n";

    /* записывем, в строку вывода, записи из таблицы, согласно массиву порядка */
    for (i = 0; i < order_len; i++)
        stringInsert(order[i]);
}

/**
 * @fn void order_clear(unsigned int length)
 * @brief Отчистка массива, по которому определяется порядок
 * @param length - итоговая длинна, которую должен иметь массив
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
 * @fn int add_to_bd(FILE *bd, const char *string)
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

/**
 * @fn int isOrder(unsigned int number)
 * @brief Функция проверяет вхождение числа в массив order
 * @param number - Число
 * @return END_OK - элемент найден / END_EXIT - элемент не найден
 */
int KursBDClass::isOrder(unsigned int number)
{
    unsigned int i; // счетчик

    /* проверяем каждый элемент order */
    for (i = 0; i < order_len; i++)
        if (number == order[i])
            /* элемент найден */
            return END_OK;

    /* элемент не найден */
    return END_EXIT;
}


/**
 * @fn void del(std::string field, std::string value)
 * @brief Удаление всех записей, из поля соответствуюх указанному значению
 * @param field - Название поля
 * @param value - Значение
 */
void KursBDClass::del(std::string field, std::string value)
{
    unsigned int i, j;              // счетчик
    int h_num = -1;                 // номер поля
  //  unsigned int new_length = 0;    // новый размер БД

    /* ищем номер поля */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            h_num = i;

    /* обнуляем массив по которому определяется порядок вывода */
    order_clear(0);

    /* если запись не удовлетворяет условию, добавить её в order */
    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i*header_col+h_num].value != value)
                order[order_len++] = i;
    }

    /* обновляем параметр number для каждой записи */
    for (i = 0; i < order_len; i++)
    {
        for (j = 0; j < header_col; j++)
            tb[i*header_col+j] = tb[order[i]*header_col+j];

//        if (isOrder(i) == END_EXIT)
//        {
//            for (j = 0; j < header_col; j++)
//                tb[i*header_col+j].number = new_length;

//            new_length++;
//        }
    }

    /* изменяем размер БД */
    table_length = order_len;
}

/**
 * @fn int sort(std::string field)
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

    return ret;
}

/**
 * @fn int sort_table(std::string field)
 * @brief Функция сортировки одного массива относительно значений другого
 * @param field - Массив, относительно которого сортируется ord
 * @return Код удачной сортировки / ошибки
 */
int KursBDClass::sort_table(std::string field)
{
    unsigned int i;                 // счетчик
    const char *values[TABLELINES]; // данные

    sort_field = -1;

    /* определяем номер поля для сортировки */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            sort_field = i;

    /* если поле надйено */
    if (sort_field >= 0)
    {
        /* заполняем массив values */
        for (i = 0; i < table_length; i++)
            values[i] = tb[i*header_col+sort_field].value.c_str();

        /* сортируем массив */
        qsort_dmas((int *) order, values, table_length-1, T_CHAR);
    }
    else
    {
        /* имя поля не найдено */
        fprintf(stderr, "Неверное имя поля %s\n", field.c_str());
        return END_NOT_FOUND;
    }

    return END_OK;
}

/**
 * @fn void insert(std::string value)
 * @brief Вставка в отсортированную БД
 * @param value - Запись для добавления в таблицу
 */
void KursBDClass::insert(std::string value)
{
    unsigned int i, j; // счетчики

    /* добаляем запись в таблицу */
    add(value);

    if (sort_field > 0)
    {
        /* сортировка БД */
        sort_table(table_header[sort_field]);

        /* обновляем параметр number */
        for(i = 0; i < table_length; i++)
            for(j = 0; j < header_col; j++)
                tb[i*header_col+j].number = order[i];

        order_len = table_length;
    }
}

/**
 * @fn unsigned int get_length()
 * @brief Функция возвращает размер БД
 * @return Размер БД
 */
unsigned int KursBDClass::get_length()
{
    return table_length;
}

/**
 * @fn std::string get_entry(unsigned int number)
 * @brief Функция возвращает указанную запись БД
 * @param number - Номер записи
 * @return Запись из БД
 */
std::string KursBDClass::get_entry(unsigned int number)
{
    std::string tmp;    // временная переменная для хранения вывода
    unsigned int i;     // счетчик

    /* отчищаем переменную */
    tmp.clear();

    /* Формируем запись */
    for (i = 0; i < header_col; i++)
    {
        tmp += tb[(number * header_col + i)].value;
        tmp += i == header_col-1 ? "\n" : ";";
    }

    /* возвращаем запись в текстовом формате */
    return tmp;
}

/**
 * @fn std::string get_sort_field()
 * @brief Функция возвращает поле сортировки
 * @return Поле сортировки
 */
std::string KursBDClass::get_sort_field()
{
    return table_header[sort_field];
}
