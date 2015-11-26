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




    // извлекаем идентификатор
    pos = get_value(&tb[table_length].id, string_to_parse);
    string_to_parse = string_to_parse + pos;

    // извлекаем имя
    pos = indexOf(string_to_parse, (char *) SEPARATOR, LEFT);


    return END_OK;
}

int KursBDClass::get_value(int *var, char *val)
{
    int pos = indexOf(val, (char *) SEPARATOR, LEFT);
    char tmp[LINELEN] = "";

    strncat(tmp, string_to_parse, pos);
    *var = atoi(tmp);

    return pos;
}

int KursBDClass::get_value(char *var, char *val)
{

    return 0;
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
