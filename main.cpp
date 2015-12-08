#include <iostream>
#include <cstring>
#include <kursbdclass.h>


using namespace std;

#define DBPATH "C:\\1\\"
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

int openBD(KursBDClass *BDclass, char *path)
{
    printf("Open DB: %s\n", path);
    if (BDclass->open(path) == END_OPEN_FAIL)
    {
        printf("[Fail]\n");
        return EXIT_FAILURE;
    }

    printf("[OK]\n\n");
    return EXIT_SUCCESS;
}

int sort(KursBDClass *BDclass, char *field)
{
    printf("Sort by field \"%s\"\n", field);
    if (BDclass->sort(field) == END_NOT_FOUND)
    {
        printf("[Fail]\n");
        return EXIT_FAILURE;
    }
    printf("[OK]\n");

    return EXIT_SUCCESS;
}

int main()
{
    // создаём экземпляр класса
    KursBDClass mainBD, scnDB, sortBD, sort2DB, selectBD;

    // неупорядоченные базы данных
    char mpath[FILENAMELEN];
    char spath[FILENAMELEN];

    // рабочие базы данных
    char sopath[FILENAMELEN];
    char so2path[FILENAMELEN];
    char selpath[FILENAMELEN];

    // создаём пути для каждой
    get_ffile_name(mpath, (char *) MAINDBNAME);
    get_ffile_name(spath, (char *) SECONDDBNAME);
    get_ffile_name(selpath, (char *) SELECTBDNAME);
    get_ffile_name(sopath, (char *) SORTBDNAME);
    get_ffile_name(so2path, (char *) SORTSCNBDNAME);


    printf("Exercise 1:\n\n\n");

    char *sort_field = (char *) "fname";
    // открываем базу данных
    openBD(&mainBD, mpath);
    openBD(&scnDB, spath);

    // сортируем 1 БД
    sort(&mainBD, sort_field);
    mainBD.write_buffer(sopath);
    printf("Result in file: %s\n\n\n", sopath);
    mainBD.close();

    // сортируем 2 БД
    sort(&scnDB, sort_field);
    scnDB.write_buffer(so2path);
    printf("Result in file: %s\n\n\n", so2path);
    scnDB.close();


    // соединяем БД
    openBD(&sortBD, sopath);
    printf("Merges on the field: \"%s\"\n\n", sort_field);
    sortBD.merge(so2path, sort_field);
    sortBD.write_buffer();
    printf("Result in file: %s\n\n\n", sopath);
    sortBD.close();



    printf("\n\n\n\nExercise 2:\n\n\n");
    openBD(&selectBD, mpath);
//    openBD(&sort2DB, so2path);

    char *selyear = (char *) "years";
    char *selfname = (char *) "fnames";
    char *selpos = (char *) "position";
    int year = 30;
    char *fname = (char *) "Maria";
    char *pos = (char *) "Admin";

    // Выполняем первую выборку
    printf("Делаем выборку по полю \"%s\" со значением %d\n\"", selyear, year);
    selectBD.select(selyear, year);
    selectBD.write_buffer(selpath);

    // закрываем файл базы данных
    selectBD.close();


    openBD(&selectBD, selpath);

    // Выполняем выборку
    printf("Делаем выборку по полю \"%s\" со значением %s\n\"", selfname, fname);
    selectBD.select(selfname, fname);
    selectBD.write_buffer();

    selectBD.close();

    openBD(&selectBD, selpath);
    // Выполняем выборку
    printf("Делаем выборку по полю \"%s\" со значением %s\n\"", selpos, pos);
    selectBD.select(selpos, pos);
    selectBD.write_buffer();

    printf("Результат операции записан в файл %s\n\n", selpath);

    // закрываем файл базы данных
    selectBD.close();
    printf("База данных закрыта\n\n\n");

    return 0;
}

