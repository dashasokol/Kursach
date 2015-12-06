#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpfun.h"
#include "kursbdclass.h"

KursBDClass::KursBDClass()
{
    table_length = 0;
    counter = 0;
}

int KursBDClass::open(char *BD_file_name)
{
    char buff[LINELEN];        // буфер
    bool end_nl = false;       // если false - в последней строке нету переноса, true - в последней строке есть перенос
    int str_num = 0;           // номер строки
    int buff_len = 0;          // запоминанете длиннай строки, на случай, если строка не коректа, но после неё следуют корректные

    // открываем файл
    bd_out_file = fmopen(BD_file_name, "r+", "KursBDClass::open");

    // считываем данные в структуру
    while(fgets(buff, LINELEN, bd_out_file))
    {
        if (parse(buff) == END_WRONG_FORMAT)
        {
            fprintf(stderr, "Line %d: Wrong string format\n\r", str_num);
            buff_len += strlen(buff)+1;
        }
        else
        {
            counter += buff_len + strlen(buff)+1;
            end_nl = buff[strlen(buff)-1] == '\n' ? true : false;
            buff_len = 0;
        }

        str_num++;
    }

    counter -= end_nl ? 2 : 1;

    return END_OK;
}

void KursBDClass::close()
{
    fclose(bd_out_file);
}

int KursBDClass::parse(char *string_to_parse)
{
    int pos = 0;

    if (string_to_parse[pos] == '#')
        return END_OK;

    if (colMatch(string_to_parse, (char *) SEPARATOR) < MAX_COLUMNS - 1)
        return END_WRONG_FORMAT;

    // извлекаем идентификатор
    if ((pos = getValue(&tb[table_length].id, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем имя
    if ((pos = getValue(tb[table_length].fname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем фамилию
    if ((pos = getValue(tb[table_length].lname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем количество лет
    if ((pos = getValue(&tb[table_length].years, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем должность
    if ((pos = getValue(tb[table_length].position, string_to_parse)) == END_WRONG_FORMAT)
        return pos;

    table_length++;

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
    if (pos == 0)
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
    strcat(string, value);
    strcat(string, (char *) SEPARATOR);
}

void KursBDClass::stringInsert(char *string, struct table insert_value)
{
    char *number = (char *) malloc(LINELEN);

    // обнуляем строку
    number[0] = '\0';

    itoa(insert_value.id, number, 10);
    valueInsert(string, number);
    number[0] = '\0';

    valueInsert(string, insert_value.fname);
    valueInsert(string, insert_value.lname);

    itoa(insert_value.years, number, 10);
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
        fprintf(stderr, "Wrong field name %s\n\r", field);

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

    if (strcmp(field, "fname") == 0)
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
        fprintf(stderr, "Wrong field name %s\n\r", field);

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

void KursBDClass::del(char *query_string)
{

}

int KursBDClass::merge(char *if_BD, char *of_BD)
{


    return 0;
}
