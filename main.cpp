#include <iostream>
#include <cstring>
#include <kursbdclass.h>

using namespace std;

#define DBPATH "C:\\Users\\Skyline\\Documents\\build-Kursach-Desktop_Qt_5_5_0_MinGW_32bit-Debug\\debug\\"
#define MAINDBNAME "KurBD"
#define SECONDDBNAME "ScnBD"
#define SELECTBDNAME "SelectBD"
#define SORTBDNAME "SortBD"
#define SORTSCNBDNAME "SortscnBD"
#define FILENAMELEN 255

void get_ffile_name(char *fullname, char *name);

void get_ffile_name(char *fullname, char *name)
{
    strcpy(fullname, (char *) DBPATH);
    strcat(fullname, name);
}

struct d
{
    char a[5];
    int b;
};

void f2(char *s)
{
    strcpy(s, (char *) "str");
}

void f(struct d (*mstr)[2])
{
    f2(mstr[0]->a);
}



int main()
{
    // создаём экземпляр класса
    KursBDClass mainBD;
    char mpath[FILENAMELEN];
    char spath[FILENAMELEN];
    char sopath[FILENAMELEN];
    char so2path[FILENAMELEN];
    char selpath[FILENAMELEN];

    get_ffile_name(mpath, (char *) MAINDBNAME);
    get_ffile_name(spath, (char *) SECONDDBNAME);
    get_ffile_name(selpath, (char *) SELECTBDNAME);
    get_ffile_name(sopath, (char *) SORTBDNAME);
    get_ffile_name(so2path, (char *) SORTSCNBDNAME);

    // указатель на резулдьтирующую строку
//    struct table ins_d;
//
//    ins_d.id = 15;
//    strcpy(ins_d.fname, (char *) "Nickita");
//    strcpy(ins_d.lname, (char *) "Levin");
//    ins_d.years = 59;
//    strcpy(ins_d.position, (char *) "CT");

    // открываем базу данных
    mainBD.open(mpath);

    mainBD.sort(sopath, (char *) "position");

    // закрываем файл базы данных
    mainBD.close();


//    struct d var[2];

//    f(&var);

//    printf("%s\n", var[0].a);

    return 0;
}

