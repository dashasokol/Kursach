#include <iostream>         // printf, cout, cin, fprintf
#include <cstring>          // strstr, str[n]cat, strlen
#include <stdlib.h>         // system
#include "kursbdclass.h"    // заголовочный файл модуля класса

/* используем стандартное пространство имен */
using namespace std;

#define DBPATH "C:\\1\\"            // путь до рабочей папки
#define MAINDBNAME "KurBD"          // основная база
#define SECONDDBNAME "ScnBD"        // вторая база
#define SELECTBDNAME "SelectBD"     // файл для вывода команды select
#define SORTBDNAME "SortBD"         // файл для вывода сортировки 1
#define SORTSCNBDNAME "SortscnBD"   // файл для вывода сортировки 2

/* Функция открывает БД */
int openBD(KursBDClass *BDclass, string path);

/* Функция сортирует БД */
int sort(KursBDClass *BDclass, string field);

/**
 * @fn int main()
 * @brief Главная функция
 * @return Код возврата
 */
int main()
{
    /* Смена кодировки на cp1251 */
	system("chcp 1251 > nul");
	
    /* Создаем экземпляр класса */ 
    KursBDClass mainBD, scnDB, sortBD, selectBD;


	// неупорядоченные базы данных
	string mpath = DBPATH;
	mpath += MAINDBNAME;
	
	string spath = DBPATH;
	spath += SECONDDBNAME;
	
	// рабочие базы данных
	string selpath = DBPATH;
	selpath += SELECTBDNAME;
	
	string sopath = DBPATH; 
	sopath += SORTBDNAME;
	
	string so2path = DBPATH; 
	so2path += SORTSCNBDNAME;


	cout << "Exercise 1:\n\n\n";

	string sort_field = "FIRSTNAME";
	
    // открываем базу данных
    openBD(&mainBD, mpath);
    openBD(&scnDB, spath);

    // сортируем 1 БД
    sort(&mainBD, sort_field);
    mainBD.write_buffer(sopath);
    cout << "Результат в файле: " << sopath << "\n\n\n";
    mainBD.close();

    // сортируем 2 БД
    sort(&scnDB, sort_field);
    scnDB.write_buffer(so2path);
    cout << "Результат в файле: " << so2path << "\n\n\n";
    scnDB.close();
    scnDB.~KursBDClass();


//    // соединяем БД
//    openBD(&sortBD, sopath);
//    printf("Merges on the field: \"%s\"\n\n", sort_field);
//    sortBD.merge(so2path, sort_field);
//    sortBD.write_buffer();
//    printf("Result in file: %s\n\n\n", sopath);
//    sortBD.close();


//    printf("\nExercise 2:\n\n\n");

//    openBD(&selectBD, mpath);

//    char *selyear = (char *) "years";
//    char *selfname = (char *) "fname";
//    char *selpos = (char *) "position";
//    int year = 30;
//    char *fname = (char *) "Мария";
//    char *pos = (char *) "Администратор"
//    // Выполняем первую выборку
//    printf("Select on the field \"%s\" with the value %d\n", selyear, year);
//    selectBD.select(selyear, year);
//    selectBD.write_buffer(selpath);

//    // закрываем файл базы данных
//    selectBD.close();


//    openBD(&selectBD, selpath);

//    // Выполняем выборку
//    printf("Select on the field \"%s\" with the value %s\n", selfname, fname);
//    selectBD.select(selfname, fname);
//    selectBD.write_buffer();

//    selectBD.close();

//    openBD(&selectBD, selpath);
//    // Выполняем выборку
//    printf("Select on the field \"%s\" with the value %s\n", selpos, pos);
//    selectBD.select(selpos, pos);
//    selectBD.write_buffer();

//    printf("Result in the file: %s\n\n", selpath);

//    // закрываем файл базы данных
//    selectBD.close();

	/* Остановка программы */
    system("pause");

    return 0;
}

/**
 * @fn int openBD(KursBDClass *BDclass, string path)
 * @brief Функция открывает БД
 * @param BDclass - указатель на экземпляр класса
 * @param path - полный путь до файла
 * @return Код удачного завершения/ошибки
 */
int openBD(KursBDClass *BDclass, string path)
{
    cout << "Открытие базы данных: " << path << "\n";
    
    /* открываем файл */
    if (BDclass->open(path) == END_OPEN_FAIL)
    {
        /* не удалось открыть файл */
        cout << "[Ошибка]\n";
        return 1;
    }

    /* файл удалось открыть */
    cout << "[Успех]\n\n";
    return 0;
}

/**
 * @fn int sort(KursBDClass *BDclass, string field)
 * @brief Функция сортирует БД
 * @param BDclass - указатель на экземпляр класса
 * @param field - поле, по которому происходит сортировка
 * @return Код удачного завершения/ошибки
 */
int sort(KursBDClass *BDclass, string field)
{
    cout << "Cортировка по полю " << field << "\n";

    /* фортируем БД */
    if (BDclass->sort(field) == END_NOT_FOUND)
    {
        /* указанного поля не существует */
        cout << "[Ошибка]\n";
        return 1;
    }

    /* БД отсортирована */
    cout << "[Успех]\n";
    return 0;
}


