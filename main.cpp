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
#define SORTBDNAME "SortBD"         // файл для вывода сортировки

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
    unsigned int i; // счетчик

    /* Смена кодировки на cp1251 */
	system("chcp 1251 > nul");
	
    /* Создаем экземпляр класса */ 
    KursBDClass mainBD, scnDB, selBD;

    /* неупорядоченные базы данных */
	string mpath = DBPATH;
	mpath += MAINDBNAME;
	
	string spath = DBPATH;
	spath += SECONDDBNAME;
	
    /* рабочие базы данных */
	string selpath = DBPATH;
	selpath += SELECTBDNAME;
	
	string sopath = DBPATH; 
	sopath += SORTBDNAME;
	
    cout << "Задание 1:\n\n\n";

    string sort_field = "FIRSTNAME"; // поле для сортировки
	
    /* открываем базу данных */
    openBD(&mainBD, mpath);
    openBD(&scnDB, spath);

    /* Соединяем и сортируем базы данных */
    cout << "Соедниение баз данных\n";
    for (i = 0; i < scnDB.get_length(); i++)
        mainBD.add(scnDB.get_entry(i));

    cout << "Сортировка по полю" << sort_field << "\n";
    mainBD.sort(sort_field);
    mainBD.write_buffer(sopath);


    cout << "Результат в файле: " << sopath << "\n\n\n";
    scnDB.close();
    scnDB.~KursBDClass();
    mainBD.close();
    mainBD.~KursBDClass();


    cout << "Задание 2:\n\n\n";

    openBD(&selBD, mpath);

    string field1 = "YEARS";
    string value1 = "30";

    string field2 = "FIRSTNAME";
    string value2 = "Maria";

    string field3 = "POSITION";
    string value3 = "Admin";


    /* Выполняем первую выборку */
    cout << "Сортировка по полю: " << field1 << "; со занчением: " << value1 << "\n";
    selBD.select(field1, value1);
    cout << "Сортировка по полю: " << field2 << "; со занчением: " << value2 << "\n";
    selBD.next_select(field2, value2);
    cout << "Сортировка по полю: " << field3 << "; со занчением: " << value3 << "\n";
    selBD.next_select(field3, value3);
    selBD.write_buffer(selpath);

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

    /* сортируем БД */
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


