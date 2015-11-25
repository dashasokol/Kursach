#include "kursbdclass.h"
#include <fcntl.h>
#include <stdio.h>
#include "helpfun.h"

KursBDClass::KursBDClass()
{
    table_length = 0;
}

KursBDClass::open(char *BD_file_name)
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
        if (parse(buff) == 1)
        {
            fprintf(stderr, "Line %d: Wrong string format", table_length + 1);
        }

        table_length++;
    }

    return 0;
}

KursBDClass::close()
{
    fclose(bd_out_file);
}

KursBDClass::parse(char *string_to_parse)
{
    int pos = 0;

    if (string_to_parse[pos] == '#')
        return 0;




    return 0;
}

KursBDClass::findId(int id)
{
    cout << id << endl;

    return 0;
}

KursBDClass::create(char *BD_file_name)
{
    cout << BD_file_name << endl;

    return 0;
}

KursBDClass::checkSpace()
{
    return 0;
}

KursBDClass::select(char* query_string)
{
    cout << query_string << endl;
}

KursBDClass::insert(char *insert_string)
{
    cout << insert_string << endl;
}

KursBDClass::del(char *query_string)
{
    cout << query_string << endl;
}

KursBDClass::merge(char *if_BD, char *of_BD)
{
    cout << if_BD << of_BD << endl;

    return 0;
}
