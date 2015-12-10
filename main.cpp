#include <iostream>         // printf, cout, cin, fprintf
#include <cstring>          // strstr, str[n]cat, strlen
#include <kursbdclass.h>    // заголовочный файл модуля класса

/* используем стандартное пространство имен */
using namespace std;

#define DBPATH "C:\\1\\"            // путь до рабочей папки
#define MAINDBNAME "KurBD"          // основная база
#define SECONDDBNAME "ScnBD"        // вторая база
#define SELECTBDNAME "SelectBD"     // файл для вывода команды select
#define SORTBDNAME "SortBD"         // файл для вывода сортировки 1
#define SORTSCNBDNAME "SortscnBD"   // файл для вывода сортировки 2
#define FILENAMELEN 255             // максимальная длинна имени файла

/* Функция возвращает полное название файла */
void get_ffile_name(char *fullname, char *name);

/* Функция открывает БД */
int openBD(KursBDClass *BDclass, char *path);

/* Функция сортирует БД */
int sort(KursBDClass *BDclass, char *field);

/**
 * @fn void get_ffile_name(char *fullname, char *name)
 * @brief Функция возвращает полное название файла
 * @param fullname - строка вывода
 * @param name  - имя файла
 */
void get_ffile_name(char *fullname, char *name)
{
    /* копируем путь до рабочей папки */
    strcpy(fullname, (char *) DBPATH);

    /* копируем имя файла */
    strcat(fullname, name);
}

/**
 * @fn int openBD(KursBDClass *BDclass, char *path)
 * @brief Функция открывает БД
 * @param BDclass - указатель на экземпляр класса
 * @param path - полный путь до файла
 * @return Код удачного завершения/ошибки
 */
int openBD(KursBDClass *BDclass, char *path)
{
    printf("Open DB: %s\n", path);
    /* открываем файл */
    if (BDclass->open(path) == END_OPEN_FAIL)
    {
        /* не удалось открыть файл */
        printf("[Fail]\n");
        return EXIT_FAILURE;
    }

    /* файл удалось открыть */
    printf("[OK]\n\n");
    return EXIT_SUCCESS;
}

/**
 * @fn int sort(KursBDClass *BDclass, char *field)
 * @brief Функция сортирует БД
 * @param BDclass - указатель на экземпляр класса
 * @param field - поле, по которому происходит сортировка
 * @return Код удачного завершения/ошибки
 */
int sort(KursBDClass *BDclass, char *field)
{
    printf("Sort by field \"%s\"\n", field);

    /* фортируем БД */
    if (BDclass->sort(field) == END_NOT_FOUND)
    {
        /* указанного поля не существует */
        printf("[Fail]\n");
        return EXIT_FAILURE;
    }

    /* БД отсортирована */
    printf("[OK]\n");
    return EXIT_SUCCESS;
}

/**
 * @fn int main()
 * @brief Главная функция
 * @return Код возврата
 */
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

    // создаём пути для каждой БД
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


    printf("\nExercise 2:\n\n\n");

    openBD(&selectBD, mpath);

    char *selyear = (char *) "years";
    char *selfname = (char *) "fname";
    char *selpos = (char *) "position";
    int year = 30;
    char *fname = (char *) "Maria";
    char *pos = (char *) "Admin";

    // Выполняем первую выборку
    printf("Select on the field \"%s\" with the value %d\n", selyear, year);
    selectBD.select(selyear, year);
    selectBD.write_buffer(selpath);

    // закрываем файл базы данных
    selectBD.close();


    openBD(&selectBD, selpath);

    // Выполняем выборку
    printf("Select on the field \"%s\" with the value %s\n", selfname, fname);
    selectBD.select(selfname, fname);
    selectBD.write_buffer();

    selectBD.close();

    openBD(&selectBD, selpath);
    // Выполняем выборку
    printf("Select on the field \"%s\" with the value %s\n", selpos, pos);
    selectBD.select(selpos, pos);
    selectBD.write_buffer();

    printf("Result in the file: %s\n\n", selpath);

    // закрываем файл базы данных
    selectBD.close();

    return 0;
}

