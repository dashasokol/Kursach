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
        else if (end == END_OK)
        {
            data_table[(*tb_len)-1].fpos = (*tb_len)-1 == 0 ? buff_len : buff_len + data_table[(*tb_len)-2].fpos + data_table[(*tb_len)-2].flen;
            data_table[(*tb_len)-1].flen = strlen(buff) + 1;
            buff_len = 0;
        }
        else
            buff_len += strlen(buff)+1;

        str_num++;
    }

    return out_file;
}

void KursBDClass::close()
{
    fclose(bd_out_file);
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
void KursBDClass::select(char *s_file_name, char *field, unsigned int value)
{
    unsigned int i; // счетчик
    char *tmp = (char *) malloc(LINELEN * TABLELINES);
    // обнуляем строку
    tmp[0] = '\0';

    strcat(tmp, (char *) DESCRIPT);

    if (strmcmp(field, "id") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i].id == value)
                stringInsert(tmp, tb[i]);
    }
    else if (strmcmp(field, "years") == 0)
    {
            for (i = 0; i < table_length; i++)
                if (tb[i].years == value)
                    stringInsert(tmp, tb[i]);
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);

    // убираем последний перенос строки
    tmp[strlen(tmp)-1] = '\0';

    // открываем файл для select
    FILE *sel_file = fmopen(s_file_name, "r+", "KursBDClass::select");

    // стираем старые данные
    clean_db(sel_file);

    // добавляем данные в файл
    add_to_bd(sel_file, tmp, 0);
    free(tmp);
}

void KursBDClass::select(char *s_file_name, char *field, char *value)
{
    unsigned int i; // счетчик
    char *tmp = (char *) malloc(LINELEN * TABLELINES);

    // обнуляем строку
    tmp[0] = '\0';

    strcat(tmp, (char *) DESCRIPT);

    if (strmcmp(field, "fname") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].fname, value) == 0)
                stringInsert(tmp, tb[i]);
    }
    else if (strcmp(field, "lname") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].lname, value) == 0)
                stringInsert(tmp, tb[i]);
    }
    else if (strcmp(field, "position") == 0)
    {
        for (i = 0; i < table_length; i++)
            if (strmcmp(tb[i].position, value) == 0)
                stringInsert(tmp, tb[i]);
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);

    // убираем последний перенос строки
    tmp[strlen(tmp)-1] = '\0';

    // открываем файл для select
    FILE *sel_file = fmopen(s_file_name, "r+", "KursBDClass::select");

    // стираем старые данные
    clean_db(sel_file);

    // добавляем данные в файл
    add_to_bd(sel_file, tmp, 0);
    free(tmp);
}

void KursBDClass::insert(struct table insert_value)
{
    char *tmp = (char *) malloc(LINELEN);

    strcpy(tmp, "\n\0");

    stringInsert(tmp, insert_value);

    // убираем последний перенос строки
    tmp[strlen(tmp) - 1] = '\0';

    add_to_bd(bd_out_file, tmp, tb[table_length-1].fpos + tb[table_length].flen);
    free(tmp);
}

int KursBDClass::add_to_bd(FILE *bd, char *string, int pos)
{
    return fmwrite(bd, string, strlen(string), pos);
}

void KursBDClass::del(char *field, unsigned int value)
{
    int i; // счетчик

    if (strmcmp(field, "id") == 0)
    {
        for (i = table_length-1; i >= 0; i--)
            if (tb[i].id == value)
                del_from_db(bd_out_file, tb[i].flen, tb[i].fpos);
    }
    else if (strmcmp(field, "years") == 0)
    {
        for (i = table_length-1; i >= 0; i--)
            if (tb[i].years == value)
                del_from_db(bd_out_file, tb[i].flen, tb[i].fpos);
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);
}

void KursBDClass::del(char *field, char *value)
{
    int i; // счетчик

    if (strmcmp(field, "fname") == 0)
    {
        for (i = table_length-1; i >= 0; i--)
            if (strmcmp(tb[i].fname, value) == 0)
                del_from_db(bd_out_file, tb[i].flen, tb[i].fpos);
    }
    else if (strcmp(field, "lname") == 0)
    {
        for (i = table_length-1; i >= 0; i--)
            if (strmcmp(tb[i].lname, value) == 0)
                del_from_db(bd_out_file, tb[i].flen, tb[i].fpos);
    }
    else if (strcmp(field, "position") == 0)
    {
        for (i = table_length-1; i >= 0; i--)
            if (strmcmp(tb[i].position, value) == 0)
                del_from_db(bd_out_file, tb[i].flen, tb[i].fpos);
    }
    else
        fprintf(stderr, "Wrong field name %s\n", field);
}

int KursBDClass::del_from_db(FILE *bd, int size, int pos)
{
    return fmclean(bd, size, pos);
}

int KursBDClass::sort(char *s_file_name, char *field)
{
    char *tmp = (char *) malloc(LINELEN * TABLELINES);
    int ret = sort_table(tmp, field, tb, table_length);

    // открываем файл для select
    FILE *sel_file = fmopen(s_file_name, "r+", "KursBDClass::select");

    // стираем старые данные
    clean_db(sel_file);

    // добавляем данные в файл
    add_to_bd(sel_file, tmp, 0);
    free(tmp);

    return ret;
}

int KursBDClass::sort_table(char *buff, char *field, struct table *data_table, unsigned int tb_len)
{
    int order[tb_len]; // массив, по которому определяется порядок
    int type;
    unsigned int i;

    // заполняем массив
    for (i = 0; i < tb_len; i++)
        order[i] = i;

    if (strmcmp(field, "id") == 0)
    {
        int values[tb_len];

        type = T_INT;
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].id;

        qsort_dmas(order, values, 0, tb_len-1);
    }
    else if (strmcmp(field, "years") == 0)
    {
        int values[tb_len];

        type = T_INT;
        for (i = 0; i < tb_len; i++)
            values[i] = data_table[i].years;

        qsort_dmas(order, values, 0, tb_len-1);

    }
    else if (strmcmp(field, "fname") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < tb_len; i++)
            strcpy(values[i], data_table[i].fname);

        qsort_dmas(order, values, 0, tb_len-1);
    }
    else if (strmcmp(field, "lname") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < tb_len; i++)
            strcpy(values[i], data_table[i].lname);

        qsort_dmas(order, values, 0, tb_len-1);
    }
    else if (strmcmp(field, "position") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < tb_len; i++)
            strcpy(values[i], data_table[i].position);

        qsort_dmas(order, values, 0, tb_len-1);
    }
    else
    {
        fprintf(stderr, "Wrong field name %s\n", field);
        return END_NOT_FOUND;
    }

    // обнуляем строку
    buff[0] = '\0';

    strcat(buff, (char *) DESCRIPT);

    for (i = 0; i < tb_len; i++)
        stringInsert(buff, data_table[order[i]]);

    // убираем последний перенос строки
    buff[strlen(buff)-1] = '\0';

    return END_OK;
}

void KursBDClass::insert_sort(char *s_file_name, struct table insert_value, char *field)
{
    struct table sorted[TABLELINES];
    sorted[0] = insert_value;

    insert_and_sort(s_file_name, sorted, 1, field);
}

void KursBDClass::insert_and_sort(char *s_file_name, struct table *insert_value, unsigned int insert_value_len, char *field)
{
    char *tmp = (char *) malloc(LINELEN * TABLELINES);
    struct table sorted[TABLELINES];
    unsigned int tb_len = 0;
    unsigned int i;

    FILE *sort_file = open_and_parse(s_file_name, sorted, &tb_len);

    for (i = 0; i < insert_value_len; i++)
        sorted[tb_len++] = insert_value[i];

    sort_table(tmp, field, sorted, tb_len);

    // стираем старые данные
    clean_db(sort_file);

    add_to_bd(sort_file, tmp, 0);
    free(tmp);

    fclose(sort_file);
}

void KursBDClass::merge(char *if_DB, char *of_BD, char *field)
{
    struct table if_DB_table[TABLELINES];
    unsigned int if_DB_tb_len = 0;

    FILE *if_file = open_and_parse(if_DB, if_DB_table, &if_DB_tb_len);
    fclose(if_file);

    insert_and_sort(of_BD, if_DB_table, if_DB_tb_len, field);
}
