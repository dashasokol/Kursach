#include <fcntl.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <typeinfo>
#include "helpfun.h"
#include "kursbdclass.h"

KursBDClass::KursBDClass()
{
    table_length = 0;
    order_len = 0;
}

int KursBDClass::open(char *BD_file_name)
{
    bd_out_file = open_and_parse(BD_file_name, tb, &table_length);

    if (!bd_out_file)
        return END_OPEN_FAIL;

    return END_OK;
}

FILE *KursBDClass::open_and_parse(char *BD_file_name, struct table *data_table, unsigned int *tb_len)
{
    char buff[LINELEN];        // буфер
    int str_num = 0;           // номер строки
    int buff_len = 0;          // запоминанете длиннай строки, на случай, если строка не коректа, но после неё следуют корректные
    int end = 0;               // позиция в файле
    FILE *out_file;            // ltcrhbgnjh afqkf


    // открываем файл
    out_file = fmopen(BD_file_name, "r+", "KursBDClass::open");

    if (!out_file)
        return out_file;

    // считываем данные в структуру
    while(fgets(buff, LINELEN, out_file))
    {
        if ((end = parse(buff, data_table, tb_len)) == END_WRONG_FORMAT)
        {
            fprintf(stderr, "Line %d: Wrong string format\n\r", str_num);
            buff_len += strlen(buff)+1;
        }

        str_num++;
    }

    return out_file;
}

void KursBDClass::close()
{
    unsigned int i = 0;


    fclose(bd_out_file);

    for (i = 0; i < table_length; i++)
    {
        tb[i].id = 0;
        tb[i].fname[0] = '\0';
        tb[i].lname[0] = '\0';
        tb[i].years = 0;
        tb[i].position[0] = '\0';
    }

    table_length = 0;

}

int KursBDClass::parse(char *string_to_parse, struct table *data_table, unsigned int *tb_len)
{
    int pos = 0;

    if (string_to_parse[pos] == '#')
        return END_EXIT;

    if (colMatch(string_to_parse, (char *) SEPARATOR) < MAX_COLUMNS - 1)
        return END_WRONG_FORMAT;

    // извлекаем идентификатор
    if ((pos = getValue(&data_table[*tb_len].id, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем имя
    if ((pos = getValue(data_table[*tb_len].fname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем фамилию
    if ((pos = getValue(data_table[*tb_len].lname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем количество лет
    if ((pos = getValue(&data_table[*tb_len].years, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем должность
    if ((pos = getValue(data_table[*tb_len].position, string_to_parse)) == END_WRONG_FORMAT)
        return pos;

    (*tb_len)++;

    return END_OK;
}

int KursBDClass::getValue(unsigned int *var, char *val)
{
    // поиск символа разделителя
    int pos = indexOf(val, (char *) SEPARATOR, LEFT);

    // если символ разделитель не найден
    if (pos == END_NOT_FOUND)
        return END_WRONG_FORMAT;

    // если найдено пустое значение
    if (pos == 0)
    {
        *var = 0;
        return END_OK;
    }

    // найдено не пустое значение
    char *tmp = (char *) malloc(pos+1);

    // обнуляем строку
    tmp[0] = '\0';

    strncat(tmp, val, pos);

    if (strspn(tmp, (char *) NUMBER) != strlen(tmp))
        return END_WRONG_FORMAT;

    *var = atoi(tmp);

    free(tmp);

    return pos;
}

int KursBDClass::getValue(char *var, char *val)
{
    int pos;

    // поиск символа разделителя
    if (val[0] == STRING_END[0])
    {
        val++;
        pos = indexOf(val, (char *) STRING_END, LEFT);
    }
    else
        pos = indexOf(val, (char *) SEPARATOR, LEFT);

    // если найдено пустое значение
    if (pos <= 0)
    {
        var[pos] = '\0';
        return END_OK;
    }

    // найдено не пустое значение
    char *tmp = (char *) malloc(pos+1);

    // обнуляем строку
    tmp[0] = '\0';

    strncat(tmp, val, pos);
    tmp[pos] = '\0';

    spacecut(var, tmp, RIGHTLEFT);

    free(tmp);

    return pos;
}

int KursBDClass::clean_db(FILE *bd)
{
    return fmclean(bd, -1, 0);
}

void KursBDClass::valueInsert(char *string, char *value)
{
    if (strlen(value) > 0)
    strcat(string, value);
    strcat(string, (char *) SEPARATOR);
}

void KursBDClass::stringInsert(char *string, struct table insert_value)
{
    char *number = (char *) malloc(LINELEN);

    // обнуляем строку
    number[0] = '\0';

    snprintf(number, LINELEN, "%d", insert_value.id);

    valueInsert(string, number);
    number[0] = '\0';

    valueInsert(string, insert_value.fname);
    valueInsert(string, insert_value.lname);

    snprintf(number, LINELEN, "%d", insert_value.years);
    valueInsert(string, number);
    number[0] = '\0';

    strcat(string, (char *) STRING_END);
    strcat(string, insert_value.position);
    strcat(string, (char *) STRING_END);

    strcat(string, (char *) "\n");

    free(number);
}
void KursBDClass::select(char *field, unsigned int value)
{
    unsigned int i; // счетчик
    // обнуляем строку

    order_clear(0);

    if (strmcmp(field, "id") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i].id == value)
                order[order_len++] = i;
    }
    else if (strmcmp(field, "years") == 0)
    {
            for (i = 0; i < table_length; i++)
                if (tb[i].years == value)
                    order[order_len++] = i;
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);

    get_order_string();
}

int KursBDClass::write_buffer(char *s_file_name)
{
    // открываем файл для select
    FILE *sel_file = fmopen(s_file_name, "r+", "KursBDClass::select");

    // стираем старые данные
    clean_db(sel_file);

    // добавляем данные в файл
    return add_to_bd(sel_file, string_for_write, 0);
}

int KursBDClass::write_buffer()
{
    // стираем старые данные
    clean_db(bd_out_file);

    // добавляем данные в файл
    return add_to_bd(bd_out_file, string_for_write, 0);
}

void KursBDClass::select(char *field, char *value)
{
    unsigned int i; // счетчик
    order_clear(0);

    if (strmcmp(field, "fname") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].fname, value) == 0)
                order[order_len++] = i;
    }
    else if (strcmp(field, "lname") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].lname, value) == 0)
                order[order_len++] = i;
    }
    else if (strcmp(field, "position") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].position, value) == 0)
                order[order_len++] = i;
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);

    get_order_string();
}

void KursBDClass::get_order_string()
{
    unsigned int i;

    // обнуляем строку
    string_for_write[0] = '\0';

    strcat(string_for_write, (char *) DESCRIPT);

    for (i = 0; i < order_len; i++)
        stringInsert(string_for_write, tb[order[i]]);

    // убираем последний перенос строки
    string_for_write[strlen(string_for_write)-1] = '\0';
}

void KursBDClass::order_clear(unsigned int length)
{
    unsigned int i;

    for (i = 0; i < table_length; i++)
        order[i] = i;

    order_len = length;
}

void KursBDClass::insert(struct table insert_value)
{
    tb[table_length++] = insert_value;

    order_clear(table_length);

    get_order_string();
}

int KursBDClass::add_to_bd(FILE *bd, char *string, int pos)
{
    return fmwrite(bd, string, strlen(string), pos);
}

void KursBDClass::del(char *field, unsigned int value)
{
    unsigned int i; // счетчик

    order_clear(0);

    if (strmcmp(field, "id") == 0)
    {

        for (i = 0; i < table_length; i++)
            if (tb[i].id != value)
                order[order_len++] = i;
    }
    else if (strmcmp(field, "years") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i].years != value)
                order[order_len++] = i;
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);

    get_order_string();
}

void KursBDClass::del(char *field, char *value)
{
    unsigned int i; // счетчик

    order_clear(0);

    if (strmcmp(field, "fname") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].fname, value) != 0)
                order[order_len++] = i;
    }
    else if (strcmp(field, "lname") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].lname, value) != 0)
                order[order_len++] = i;
    }
    else if (strcmp(field, "position") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].position, value) != 0)
                order[order_len++] = i;
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);

    get_order_string();
}

int KursBDClass::del_from_db(FILE *bd, int size, int pos)
{
    return fmclean(bd, size, pos);
}

int KursBDClass::sort(char *field)
{
    int ret = sort_table(order, field, tb, table_length);

    order_len = table_length;
    get_order_string();

    return ret;
}

int KursBDClass::sort_table(int *ord, char *field, struct table *data_table, unsigned int tb_len)
{
    int type;
    unsigned int i;

    // заполняем массив
    for (i = 0; i < tb_len; i++)
        ord[i] = i;

    if (strmcmp(field, "id") == 0)
    {
        int values[TABLELINES];

        type = T_INT;
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].id;

        qsort_dmas(ord, values, 0, tb_len-1);
    }
    else if (strmcmp(field, "years") == 0)
    {
        int values[TABLELINES];

        type = T_INT;
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].years;

        qsort_dmas(ord, values, 0, tb_len-1);

    }
    else if (strmcmp(field, "fname") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < tb_len; i++)
            strcpy(values[i], data_table[i].fname);

        qsort_dmas(ord, values, 0, tb_len-1);
    }
    else if (strmcmp(field, "lname") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < tb_len; i++)
            strcpy(values[i], data_table[i].lname);

        qsort_dmas(ord, values, 0, tb_len-1);
    }
    else if (strmcmp(field, "position") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < tb_len; i++)
            strcpy(values[i], data_table[i].position);

        qsort_dmas(ord, values, 0, tb_len-1);
    }
    else
    {
        fprintf(stderr, "Wrong field name %s\n", field);
        return END_NOT_FOUND;
    }

    return END_OK;
}

void KursBDClass::insert_sort(struct table insert_value, char *field)
{
    struct table sorted[TABLELINES];
    sorted[0] = insert_value;

    insert_and_sort(sorted, 1, field);
}

void KursBDClass::insert_and_sort(struct table *insert_value, unsigned int insert_value_len, char *field)
{
    struct table sorted[TABLELINES];
    unsigned int tb_len = 0;
    unsigned int i;

    for (i = 0; i < insert_value_len; i++)
        sorted[tb_len++] = insert_value[i];

    sort_table(order, field, sorted, tb_len);
    get_order_string();
}

void KursBDClass::merge(char *if_DB, char *field)
{
    struct table if_DB_table[TABLELINES];
    unsigned int if_DB_tb_len = 0;

    FILE *if_file = open_and_parse(if_DB, if_DB_table, &if_DB_tb_len);
    fclose(if_file);

    insert_and_sort(if_DB_table, if_DB_tb_len, field);

    write_buffer();
}
