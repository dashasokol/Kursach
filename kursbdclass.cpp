#include <iostream>         // printf, scanf, NULL
#include <cstdlib>          // malloc, free, rand
#include <cstring>          // strstr, str[n]cat, strlen
#include "helpfun.h"        // Заголовочный файл helpfun
#include "kursbdclass.h"    // Заголовочный файл этого модуля

/* инициализация класса */
KursBDClass::KursBDClass()
{
    table_length = 0;   // обнуляем количество элементов
    order_len = 0;      // обнуляем размер массива сортировки
}

/**
 * @fn int open(char *BD_file_name)
 * @brief Оболочка для функции open_and_parse
 * @param BD_file_name - Полное имя файла БД
 * @return Код удачного открытия файла/ошибки
 */
int KursBDClass::open(char *BD_file_name)
{
    /* открываем файл и читаем БД */
    bd_out_file = open_and_parse(BD_file_name, tb, &table_length);

    /* если база не прочиталась или файл не открылся */
    if (!bd_out_file)
        return END_OPEN_FAIL;

    /* чтение БД прошло удачно */
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
FILE *KursBDClass::open_and_parse(char *BD_file_name, struct table *data_table, unsigned int *tb_len)
{
    char buff[LINELEN];        // буфер
    int str_num = 0;           // номер строки
    FILE *out_file;            // дескриптор файла


    /* открываем файл */
    out_file = fmopen(BD_file_name, "r+", "KursBDClass::open");

    /* Не вышло открыть файл */
    if (!out_file)
        return out_file;

    /* считываем построчно данные в структуру */
    while(fgets(buff, LINELEN, out_file))
    {
        /* парсим строку */
        if (parse(buff, data_table, tb_len) == END_WRONG_FORMAT)
        {
            /* строка имеет неправильный формат */
            fprintf(stderr, "Line %d: Wrong string format\n", str_num);
        }

        /* увеличиваем номер строки */
        str_num++;
    }

    return out_file;
}

/**
 * @fn void close()
 * @brief Функция закрытия БД
 */
void KursBDClass::close()
{
    unsigned int i = 0; // счетчик

    fclose(bd_out_file); // закрытие файла

    // обнуление структуры
    for (i = 0; i < table_length; i++)
    {
        tb[i].id = 0;
        tb[i].fname[0] = '\0';
        tb[i].lname[0] = '\0';
        tb[i].years = 0;
        tb[i].position[0] = '\0';
    }

    // обнуление количества строк в БД
    table_length = 0;
}

/**
 * @fn int parse(char *string_to_parse, struct table *data_table, unsigned int *tb_len)
 * @brief Функция обработки строки
 * @param string_to_parse - строка
 * @param data_table - указатель на структуру БД
 * @param tb_len - количество записей в таблице
 * @return Код удачного завершения / ошибки
 */
int KursBDClass::parse(char *string_to_parse, struct table *data_table, unsigned int *tb_len)
{
    int pos = 0; // позиция для чтения

    /* если найден комментарий, пропустить строку */
    if (string_to_parse[pos] == '#')
        return END_EXIT;

    /* проверка строки на соответствующее число символов разделителей */
    if (colMatch(string_to_parse, (char *) SEPARATOR) < MAX_COLUMNS - 1)
        return END_WRONG_FORMAT;

    /* извлекаем идентификатор */
    if ((pos = getValue(&data_table[*tb_len].id, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    /* извлекаем имя */
    if ((pos = getValue(data_table[*tb_len].fname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    /* извлекаем фамилию */
    if ((pos = getValue(data_table[*tb_len].lname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    /* извлекаем количество лет */
    if ((pos = getValue(&data_table[*tb_len].years, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    /* извлекаем должность */
    if ((pos = getValue(data_table[*tb_len].position, string_to_parse)) == END_WRONG_FORMAT)
        return pos;

    /* увеличиваем количество записей в таблице */
    (*tb_len)++;

    return END_OK;
}

/**
 * @fn int getValue(unsigned int *var, char *val)
 * @brief Функция, получающая целочисленное значение
 * @param var - переменная для записи значения
 * @param val - строка для парсинга
 * @return Код удачного завершения / ошибки
 */
int KursBDClass::getValue(unsigned int *var, char *val)
{
    /* поиск символа разделителя */
    int pos = indexOf(val, (char *) SEPARATOR, LEFT);   // позиция для чтения

    /* если символ разделитель не найден */
    if (pos == END_NOT_FOUND)
        return END_WRONG_FORMAT;

    /* если найдено пустое значение */
    if (pos == 0)
    {
        /* если пустое значение, считать, что значение 0 */
        *var = 0;
        return END_OK;
    }

    /* найдено не пустое значение */

    /* выделяем память под временную переменную */
    char *tmp = (char *) malloc(pos+1);

    /* обнуляем строку */
    tmp[0] = '\0';

    /* копируем строку содержащую символьное предствление числа */
    strncat(tmp, val, pos);

    /* проверяем строку на содержание символов, не являющихся цифрами */
    if (strspn(tmp, (char *) NUMBER) != strlen(tmp))
        return END_WRONG_FORMAT;

    /* переводим строку в число */
    *var = atoi(tmp);

    /* освобождаем временную переменную */
    free(tmp);

    return pos;
}

/**
 * @fn int getValue(char *var, char *val)
 * @brief Функция, получающая строковое значение
 * @param var - переменная для записи значения
 * @param val - строка для парсинга
 * @return Код удачного завершения / ошибки
 */
int KursBDClass::getValue(char *var, char *val)
{
    int pos;    // позиция для чтения

    /* поиск символа разделителя */
    if (val[0] == STRING_END[0])
    {
        /* найдена строка с кавычками */
        val++;
        pos = indexOf(val, (char *) STRING_END, LEFT);
    }
    else
        /* строка без кавычек */
        pos = indexOf(val, (char *) SEPARATOR, LEFT);

    /* если найдено пустое значение */
    if (pos <= 0)
    {
        /* если пустое значение, считать, что значение "\0" */
        var[pos] = '\0';
        return END_OK;
    }

    /* найдено не пустое значение */

    /* выделяем память под временную переменную */
    char *tmp = (char *) malloc(pos+1);

    /* обнуляем строку */
    tmp[0] = '\0';

    /* копируем строку во временную переменную */
    strncat(tmp, val, pos);

    /* удаляем пробелы с обоих сторон строки, если они обнаружены */
    spacecut(var, tmp, RIGHTLEFT);

    /* освобождаем временную переменную */
    free(tmp);

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
    return fmclean(bd, -1, 0);
}

/**
 * @fn void valueInsert(char *string, char *value)
 * @brief Функция вставляет очередное значение в строку вывода
 * @param string - Строка вывода
 * @param value - Значение для вставки
 */
void KursBDClass::valueInsert(char *string, char *value)
{
    /* если значение существует, вставляем его в строку */
    if (strlen(value) > 0)
        strcat(string, value);

    /* добавляем символ разделитель */
    strcat(string, (char *) SEPARATOR);
}

/**
 * void stringInsert(char *string, struct table insert_value)
 * @brief Функция вставляет строку содержащую данные одной записи из БД в строку вывода
 * @param string - Строка вывода
 * @param insert_value - Структура содержащая одну запись из таблицы БД
 */
void KursBDClass::stringInsert(char *string, struct table insert_value)
{
    char *number = (char *) malloc(LINELEN); // переменная для перевода числа в строку

    /* обнуляем строку */
    number[0] = '\0';

    /* переводим ID в cтроку*/
    snprintf(number, LINELEN, "%d", insert_value.id);

    /* вставляем ID */
    valueInsert(string, number);
    number[0] = '\0';

    /* вставляем имя и фамилию */
    valueInsert(string, insert_value.fname);
    valueInsert(string, insert_value.lname);

    /* переводим количество лет в строку */
    snprintf(number, LINELEN, "%d", insert_value.years);

    /* вставляем количество лет */
    valueInsert(string, number);
    number[0] = '\0';

    /* вставляем должность */
    strcat(string, (char *) STRING_END);
    strcat(string, insert_value.position);
    strcat(string, (char *) STRING_END);

    /* добавляем символ переноса строки в конец */
    strcat(string, (char *) "\n");

    /* освобождаем память */
    free(number);
}

/**
 * @fn void select(char *field, unsigned int value)
 * @brief Функция ищет записи в таблице по послю, содержащему целочисленные значения
 * @param field - имя поля для поиска
 * @param value - значение для поиска
 */
void KursBDClass::select(char *field, unsigned int value)
{
    unsigned int i; // счетчик

    /* обнуляем массив по которому определяется порядок вывода */
    order_clear(0);

    /* проверяем, по какому полю искать целочисленное значение */
    if (strmcmp(field, "id") == 0)
    {
        /* значение необходимо искать в поле id */
        for (i = 0; i < table_length; i++)
            if (tb[i].id == value)
                order[order_len++] = i;
    }
    else if (strmcmp(field, "years") == 0)
    {
        /* значение необходимо искать в поле years */
        for (i = 0; i < table_length; i++)
            if (tb[i].years == value)
                order[order_len++] = i;
    }
    else
        /* поле с таким именем не найдено */
        fprintf(stderr, "Wrong field name %s\n", field);

    /* записать все найденные записи в строку вывода */
    get_order_string();
}

/**
 * @fn int write_buffer(char *s_file_name)
 * @brief Функция записывает строку вывода в указанный файл
 * @param s_file_name - Имя файла для записи
 * @return Код удачной записи / ошибки
 */
int KursBDClass::write_buffer(char *s_file_name)
{
    /* открываем файл для select */
    FILE *sel_file = fmopen(s_file_name, "r+", "KursBDClass::select");

    /* стираем старые данные */
    clean_db(sel_file);

    /* добавляем данные в файл */
    return add_to_bd(sel_file, string_for_write);
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
    return add_to_bd(bd_out_file, string_for_write);
}

/**
 * @fn void select(char *field, char *value)
 * @brief Функция ищет записи в таблице по послю, содержащему строковые значения
 * @param field - имя поля для поиска
 * @param value - значение для поиска
 */
void KursBDClass::select(char *field, char *value)
{
    unsigned int i; // счетчик

    /* обнуляем массив по которому определяется порядок вывода */
    order_clear(0);

    /* проверяем, по какому полю искать строковое значение */
    if (strmcmp(field, "fname") == 0)
    {
        /* значение необходимо искать в поле fname */
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].fname, value) == 0)
                order[order_len++] = i;
    }
    else if (strmcmp(field, "lname") == 0)
    {
        /* значение необходимо искать в поле lname */
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].lname, value) == 0)
                order[order_len++] = i;
    }
    else if (strmcmp(field, "position") == 0)
    {
        /* значение необходимо искать в поле position */
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].position, value) == 0)
                order[order_len++] = i;
    }
    else
        /* поле с таким именем не найдено */
        fprintf(stderr, "Wrong field name %s\n", field);

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
    string_for_write[0] = '\0';

    /* записываем в строку вывода описание таблицы */
    strcat(string_for_write, (char *) DESCRIPT);

    /* записывем в строку вывода записи из таблицы согласно массиву порядка */
    for (i = 0; i < order_len; i++)
        stringInsert(string_for_write, tb[order[i]]);

    /* убираем последний перенос строки */
    string_for_write[strlen(string_for_write)-1] = '\0';
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
void KursBDClass::insert(struct table insert_value)
{
    /* добаляем запись в таблицу */
    tb[table_length++] = insert_value;

    /* обнуляем массив по которому определяется порядок вывода */
    order_clear(table_length);

    /* записать все найденные записи в строку вывода */
    get_order_string();
}

/**
 * @fn int add_to_bd(FILE *bd, char *string)
 * @brief Оболочка для записи данных в файл БД
 * @param bd - Дескриптор файла
 * @param string - Строка для записи
 * @return Код удачной записи / ошибки записи
 */
int KursBDClass::add_to_bd(FILE *bd, char *string)
{
    /* записываем весь массив символов в файл с позиции 0 */
    return fmwrite(bd, string, strlen(string), 0);
}

/**
 * @fn void del(char *field, unsigned int value)
 * @brief Удаление всех записей, из поля соответствуюх указанному целочисленному значению
 * @param field - Название поля
 * @param value - Значение
 */
void KursBDClass::del(char *field, unsigned int value)
{
    unsigned int i; // счетчик

    /* обнуляем массив по которому определяется порядок вывода */
    order_clear(0);

    /* проверяем, по какому полю искать целочисленное значение */
    if (strmcmp(field, "id") == 0)
    {
        /* значение необходимо искать в поле id */
        for (i = 0; i < table_length; i++)
            if (tb[i].id != value)
                order[order_len++] = i;
    }
    else if (strmcmp(field, "years") == 0)
    {
        /* значение необходимо искать в поле years */
        for (i = 0; i < table_length; i++)
            if (tb[i].years != value)
                order[order_len++] = i;
    }
    else
        /* поле с таким именем не найдено */
        fprintf(stderr, "Wrong field name %s\n", field);

    /* записать все несоответствующие записи в строку вывода */
    get_order_string();
}

/**
 * @fn void del(char *field, char *value)
 * @brief Удаление всех записей, из поля соответствуюх указанному строковому значению
 * @param field - Название поля
 * @param value - Значение
 */
void KursBDClass::del(char *field, char *value)
{
    unsigned int i; // счетчик

    /* обнуляем массив по которому определяется порядок вывода */
    order_clear(0);

    /* проверяем, по какому полю искать строковое значение */
    if (strmcmp(field, "fname") == 0)
    {
        /* значение необходимо искать в поле fname */
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].fname, value) != 0)
                order[order_len++] = i;
    }
    else if (strcmp(field, "lname") == 0)
    {
        /* значение необходимо искать в поле lname */
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].lname, value) != 0)
                order[order_len++] = i;
    }
    else if (strcmp(field, "position") == 0)
    {
        /* значение необходимо искать в поле position */
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].position, value) != 0)
                order[order_len++] = i;
    }
    else
        /* поле с таким именем не найдено */
        fprintf(stderr, "Wrong field name %s\n", field);

    /* записать все несоответствующие записи в строку вывода */
    get_order_string();
}

/**
 * @fn int sort(char *field)
 * @brief Оболочка для функции сортировки по полю
 * @param field - Название поля
 * @return Код удачной сортировки / ошибки
 */
int KursBDClass::sort(char *field)
{
    /* переопределяем массив порядка */
    order_clear(table_length);

    /* сортируем данные */
    int ret = sort_table(order, field, tb, table_length);

    /* изменяем длинну массива порядка */
    order_len = table_length;

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
int KursBDClass::sort_table(int *ord, char *field, struct table *data_table, unsigned int tb_len)
{
    unsigned int i;     // счетчик

    /* проверяем, по какому полю сортировать */
    if (strmcmp(field, "id") == 0)
    {
        /* сортировка по полю id */
        int values[TABLELINES]; // массив, относительно которого будет сортировка

        /* заполняем массив */
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].id;

        /* сортируем массив */
        qsort_dmas(ord, values, tb_len-1, T_INT);
    }
    else if (strmcmp(field, "years") == 0)
    {
        /* сортировка по полю years */
        int values[TABLELINES]; // массив, относительно которого будет сортировка

        /* заполняем массив */
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].years;

        /* сортируем массив */
        qsort_dmas(ord, values, tb_len-1, T_INT);

    }
    else if (strmcmp(field, "fname") == 0)
    {
        /* сортировка по полю fname */
        char *values[TABLELINES];   // массив, относительно которого будет сортировка

        /* заполняем массив */
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].fname;

        /* сортируем массив */
        qsort_dmas(ord, values, tb_len-1, T_CHAR);
    }
    else if (strmcmp(field, "lname") == 0)
    {
        /* сортировка по полю lname */
        char *values[TABLELINES];   // массив, относительно которого будет сортировка

        /* заполняем массив */
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].lname;

        /* сортируем массив */
        qsort_dmas(ord, values, tb_len-1, T_CHAR);
    }
    else if (strmcmp(field, "position") == 0)
    {
        /* сортировка по полю position */
        char *values[TABLELINES];   // массив, относительно которого будет сортировка

        /* заполняем массив */
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].position;

        /* сортируем массив */
        qsort_dmas(ord, values, tb_len-1, T_CHAR);
    }
    else
    {
        /* имя поля не найдено */
        fprintf(stderr, "Wrong field name %s\n", field);
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
int KursBDClass::insert_sort(struct table insert_value, char *field)
{
    struct table sorted[TABLELINES];    // структура для добавления в таблицу

    /* используется только первая запись таблицы */
    sorted[0] = insert_value;

    /* вставить и пересортировать по полю */
    return insert_and_sort(sorted, 1, field);
}

/**
 * @fn void insert_and_sort(struct table *insert_value, unsigned int insert_value_len, char *field)
 * @brief Функция которая добавляет записи в таблицу и сортирует её
 * @param insert_value - Массив данных, которые нужно добавить
 * @param insert_value_len - Размер добавляемого массива
 * @param field - Поле, по которому необходимо произвести сортировку
 * @return Код удачной сортировки / ошибки
 */
int KursBDClass::insert_and_sort(struct table *insert_value, unsigned int insert_value_len, char *field)
{
    unsigned int i; // счетчик

    /* добавляем все данные из массва insert_value в основную структуру */
    for (i = 0; i < insert_value_len; i++)
        tb[table_length++] = insert_value[i];

    /* переопределяем массив порядка */
    order_clear(table_length);

    /* сортируем данные */
    int ret = sort_table(order, field, tb, table_length);

    /* записать все записи в строку вывода */
    get_order_string();

    return ret;
}

/**
 * @fn void merge(char *if_DB, char *field)
 * @brief Добавление данных из другой БД и последующая сортировка
 * @param if_DB - БД, из которой нужно добавить данные
 * @param field - Поле для сортировки
 * @return Код удачной сортировки / ошибки
 */
int KursBDClass::merge(char *if_DB, char *field)
{
    struct table if_DB_table[TABLELINES];   // структура для добавления в таблицу
    unsigned int if_DB_tb_len = 0;          // размер второй БД

    /* открываем и парсим файл второй БД */
    FILE *if_file = open_and_parse(if_DB, if_DB_table, &if_DB_tb_len);

    /* закрываем файл второй БД */
    fclose(if_file);

    /* вставляем данные в основную таблицу и сортируем */
    int ret = insert_and_sort(if_DB_table, if_DB_tb_len, field);

    /* записываем данные в файл основной БД */
    write_buffer();

    return ret;
}
