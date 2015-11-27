#include "kursbdclass.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpfun.h"

KursBDClass::KursBDClass()
{
    table_length = 0;
}

int KursBDClass::open(char *BD_file_name)
{
    char buff[LINELEN]; // буфер
    bd_out_file = fopen(BD_file_name, "r+");

    if (!bd_out_file)
    {
        // записываем ошибку в поток вывода отладочных сообщений
        perror("KursBDClass::open: Cannot open file\n\r");
        return 1;
    }

    table_length = 0; // ??????????????????

    // считываем данные в структуру
    while(fgets(buff, LINELEN, bd_out_file))
    {
        if (parse(buff) == END_WRONG_FORMAT)
        {
            fprintf(stderr, "Line %d: Wrong string format", table_length + 1);
        }

        table_length++;
    }

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

    if (col_match(string_to_parse, (char *) SEPARATOR) < MAX_COLUMNS - 1)
        return END_WRONG_FORMAT;

    // ????????????????????????????????????????? //
    // извлекаем идентификатор
    if ((pos = get_value(&tb[table_length].id, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем имя
    if ((pos = get_value(tb[table_length].fname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем фамилию
    if ((pos = get_value(tb[table_length].lname, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем количество лет
    if ((pos = get_value(&tb[table_length].years, string_to_parse)) != END_WRONG_FORMAT)
        string_to_parse += pos + 1;
    else
        return pos;

    // извлекаем должность
    if ((pos = get_value(tb[table_length].position, string_to_parse)) == END_WRONG_FORMAT)
        return pos;

    return END_OK;
}

int KursBDClass::get_value(int *var, char *val)
{
    // поиск символа разделителя
    int pos = indexOf(val, (char *) SEPARATOR, LEFT);

    // если символ разделитель не найден
    if (pos == -1)
        return END_WRONG_FORMAT;

    // если найдено пустое значение
    if (pos == 0)
    {
        *var = 0;
        return END_OK;
    }

    // найдено не пустое значение
    char *tmp = malloc(pos+1);

    strncat(tmp, val, pos);
    *var = atoi(tmp);

    free(tmp);

    return pos;
}

int KursBDClass::get_value(char *var, char *val)
{
    int pos;

    // поиск символа разделителя
    if (val[0] == STRING_END)
        val++;
        pos = indexOf(val, (char *) STRING_END, LEFT);
    else
        pos = indexOf(val, (char *) SEPARATOR, LEFT);

    // если символ разделитель не найден
    if (pos == -1)
        return END_WRONG_FORMAT;

    // если найдено пустое значение
    if (pos == 0)
        var[pos] = '/0';
        return END_OK;


    // найдено не пустое значение
    char *tmp = malloc(pos+1);

    strncat(tmp, val, pos);
    tmp[pos] = '/0';

    spacecut(var, tmp, RIGHTLEFT);

    free(tmp);

    return pos;
}

int KursBDClass::findId(int id)
{

    return 0;
}

int KursBDClass::create(char *BD_file_name)
{


    return 0;
}

int KursBDClass::checkSpace()
{
    return 0;
}

char *KursBDClass::select(char* query_string)
{
    return (char *) "dd";
}

void KursBDClass::insert(char *insert_string)
{

}

void KursBDClass::del(char *query_string)
{

}

int KursBDClass::merge(char *if_BD, char *of_BD)
{


    return 0;
}
