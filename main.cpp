#include <iostream>
#include <kursbdclass.h>

using namespace std;

#define MAINDBPATH "C:\\Users\\Skyline\\Documents\\build-Kursach-Desktop_Qt_5_5_0_MinGW_32bit-Debug\\debug\\KurBD"
#define SECONDDBPATH "C:\\Users\\Skyline\\Documents\\build-Kursach-Desktop_Qt_5_5_0_MinGW_32bit-Debug\\debug\\ScnBD"



int main()
{
    // создаём экземпляр класса
    KursBDClass mainBD;

    // указатель на резулдьтирующую строку
    char *tmp = (char *) malloc(LINELEN * TABLELINES);

    // открываем базу данных
    mainBD.open((char *) MAINDBPATH);

    mainBD.select(tmp, (char *) "position", (char *) "Admin");

    printf(tmp);

    // закрываем файл базы данных
    mainBD.close();
    return 0;
}

