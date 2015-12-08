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
    counter = 0;
}

int KursBDClass::open(char *BD_file_name)
{
    return open_and_parse(BD_file_name, tb, &table_length, bd_out_file, &counter);
}

int KursBDClass::open_and_parse(char *BD_file_name, struct table *data_table, unsigned int *tb_len, FILE *out_file, unsigned int *cnt)
{
    char buff[LINELEN];        // буфер
    bool end_nl = false;       // если false - в последней строке нету переноса, true - в последней строке есть перенос
    int str_num = 0;           // номер строки
    int buff_len = 0;          // запоминанете длиннай строки, на случай, если строка не коректа, но после неё следуют корректные
    int end = 0;               // позиция в файле

    // открываем файл
    bd_out_file = fmopen(BD_file_name, "r+", "KursBDClass::open");

    // считываем данные в структуру
    while(fgets(buff, LINELEN, bd_out_file))
    {
        if ((end = parse(buff, data_table, tb_len)) == END_WRONG_FORMAT)
        {
            fprintf(stderr, "Line %d: Wrong string format\n\r", str_num);
            buff_len += strlen(buff)+1;
        }
        else if (end == END_OK)
        {
            data_table[*tb_len-1].fpos = *cnt;
            data_table[*tb_len-1].flen = buff_len + strlen(buff) + 1;
            *cnt += buff_len + strlen(buff)+1;
            end_nl = buff[strlen(buff)-1] == '\n' ? true : false;
            buff_len = 0;
        }

        str_num++;
    }

    *cnt -= end_nl ? 2 : 1;

    return END_OK;
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

    add_to_bd(bd_out_file, tmp, counter);
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

void KursBDClass::sort(char *s_file_name, char *field)
{
    char *tmp = (char *) malloc(LINELEN * TABLELINES);
    sort_table(tmp, field);

    // открываем файл для select
    FILE *sel_file = fmopen(s_file_name, "r+", "KursBDClass::select");

    // стираем старые данные
    clean_db(sel_file);

    // добавляем данные в файл
    add_to_bd(sel_file, tmp, 0);
    free(tmp);
}

void KursBDClass::sort_table(char *buff, char *field)
{
    int order[table_length]; // массив, по которому определяется порядок
    int type;
    unsigned int i;

    // заполняем массив
    for (i = 0; i < table_length; i++)
        order[i] = i;

    if (strmcmp(field, "id") == 0)
    {
        int values[table_length];

        type = T_INT;
        for (i = 0; i < table_length; i++)
            values[i] = tb[i].id;

        qsort_dmas(order, values, 0, table_length-1);
    }
    else if (strmcmp(field, "years") == 0)
    {
        int values[table_length];

        type = T_INT;
        for (i = 0; i < table_length; i++)
            values[i] = tb[i].years;

        qsort_dmas(order, values, 0, table_length-1);

    }
    else if (strmcmp(field, "fname") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < table_length; i++)
            strcpy(values[i], tb[i].fname);

        qsort_dmas(order, values, 0, table_length-1);
    }
    else if (strmcmp(field, "lname") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < table_length; i++)
            strcpy(values[i], tb[i].lname);

        qsort_dmas(order, values, 0, table_length-1);
    }
    else if (strmcmp(field, "position") == 0)
    {
        char values[LINELEN][TABLELINES];

        type = T_CHAR;
        for (i = 0; i < table_length; i++)
            strcpy(values[i], tb[i].position);

        qsort_dmas(order, values, 0, table_length-1);
    }
    else
    {
        fprintf(stderr, "Wrong field name %s\n", field);
        return;
    }

    // обнуляем строку
    buff[0] = '\0';

    strcat(buff, (char *) DESCRIPT);

    for (i = 0; i < table_length; i++)
        stringInsert(buff, tb[order[i]]);

    // убираем последний перенос строки
    buff[strlen(buff)-1] = '\0';
}

void KursBDClass::insert_sort(char *s_file_name, struct table insert_value)
{
    char *tmp = (char *) malloc(LINELEN);

    strcpy(tmp, "\n\0");

    stringInsert(tmp, insert_value);

    // убираем последний перенос строки
    tmp[strlen(tmp) - 1] = '\0';



    add_to_bd(bd_out_file, tmp, counter);
    free(tmp);
}
