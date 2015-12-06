#include <iostream>
#include <cstring>
#include <kursbdclass.h>

using namespace std;

#define DBPATH "C:\\Users\\Skyline\\Documents\\build-Kursach-Desktop_Qt_5_5_0_MinGW_32bit-Debug\\debug\\"
#define MAINDBNAME "KurBD"
#define SECONDDBNAME "ScnBD"
#define FILENAMELEN 255


int main()
{
    // создаём экземпляр класса
    KursBDClass mainBD;
    char mpath[FILENAMELEN];
    char spath[FILENAMELEN];

    strcpy(mpath, (char *) DBPATH);
    strcat(mpath, (char *) MAINDBNAME);

    strcpy(spath, (char *) DBPATH);
    strcat(spath, (char *) SECONDDBNAME);

    // указатель на резулдьтирующую строку
    char *tmp = (char *) malloc(LINELEN * TABLELINES);
    struct table ins_d;

    ins_d.id = 15;
    strcpy(ins_d.fname, (char *) "Nickita");
    strcpy(ins_d.lname, (char *) "Levin");
    ins_d.years = 59;
    strcpy(ins_d.position, (char *) "CT");

    // открываем базу данных
    mainBD.open(mpath);

    mainBD.insert(ins_d);

    // закрываем файл базы данных
    mainBD.close();
    return 0;
}

