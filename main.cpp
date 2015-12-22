#include <iostream>         // printf, cout, cin, fprintf
#include <cstring>          // strstr, str[n]cat, strlen
#include <stdlib.h>         // system
#include <clocale>
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

/* Вывод меню */
void print_menu();

/* пункт А: тест */
void a();

/* пункт B: тест */
void b();

/* пункт C: тест */
void c();

/* пункт D: тест */
void d();

/* пункт E: тест */
void e();

/* Задача 1 */
void ex1();

/* Задача 2 */
void ex2();

/**
 * @fn int main()
 * @brief Главная функция
 * @return Код возврата
 */
int main()
{
    char ch;

    /* Смена кодировки на cp1251 */
    setlocale( LC_ALL, "Russian" );

    print_menu();

    while((ch=getchar()) != 'q' && ch != 'Q' && ch != EOF)
    {
        switch(ch)
        {
        case 'a':
        case 'A':
            a();
            break;
        case 'b':
        case 'B':
            b();
            break;
        case 'c':
        case 'C':
            c();
            break;
        case 'd':
        case 'D':
            d();
            break;
        case 'e':
        case 'E':
            b();
            break;
        case '1':
            ex1();
        case '2':
            ex2();
            break;
        default:
            cout << "  !Нет такого пункта\n";
            system("pause");
            break;
        }

        print_menu();
    }

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

void print_menu()
{
    /* Отчищаем экран */
    system("cls");
    cout << "Меню программы:\n\nТесты:\na) Выполнить тест a \nb) Выполнить тест b\n" \
            "c) Выполнить тест c \nd) Выполнить тест d \ne) Выполнить тест e \n" \
            "\nЗадания\n1) Выполнить тест задания 1 \n2) Выполнить тест задания 2\n\n" \
            "Другие комманды:\nq) Выход\n\n\nВаш выбор: ";
}

void a()
{
    /* Отчищаем экран */
    system("cls");

    KursBDClass mainBD;

    /* Неупорядоченные базы данных */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* Открываем БД */
    openBD(&mainBD, mpath);

    /* Заносим данные в конец файла */
    mainBD.add("124;Galina;Zuyeva;46;Cleaner");

    /* записываем файл */
    mainBD.write_buffer();

    cout << "Результат в файле: " << mpath << "\n\n\n";

    /* Закрываем БД */
    mainBD.close();
}

void b()
{
    /* Отчищаем экран */
    system("cls");

    KursBDClass mainBD;

    /* Неупорядоченные базы данных */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* Критерий */
    string field = "YEARS";
    string value = "30";

    /* Открываем БД */
    openBD(&mainBD, mpath);

    /* Удаляем запись */
    mainBD.del(field, value);

    /* записываем файл */
    mainBD.write_buffer();

    cout << "Результат в файле: " << mpath << "\n\n\n";

    /* Закрываем БД */
    mainBD.close();
}

void c()
{
    /* Отчищаем экран */
    system("cls");

    KursBDClass mainBD;

    /* Неупорядоченные базы данных */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* Рабочие базы данных */
    string selpath = DBPATH;
    selpath += SELECTBDNAME;

    /* Критерий */
    string field = "FIRSTNAME";

    /* Открываем БД */
    openBD(&mainBD, mpath);

    /* Удаляем запись */
    mainBD.sort(field);

    /* записываем файл */
    mainBD.write_buffer(selpath);

    cout << "Результат в файле: " << selpath << "\n\n\n";

    /* Закрываем БД */
    mainBD.close();
}

void d()
{
    /* Отчищаем экран */
    system("cls");

    KursBDClass mainBD;

    /* Неупорядоченные базы данных */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* Рабочие базы данных */
    string selpath = DBPATH;
    selpath += SELECTBDNAME;

    /* Критерий */
    string field = "YEARS";
    string value = "30";

    /* Открываем БД */
    openBD(&mainBD, mpath);

    /* Удаляем запись */
    mainBD.select(field, value);

    /* записываем файл */
    mainBD.write_buffer(selpath);

    cout << "Результат в файле: " << selpath << "\n\n\n";

    /* Закрываем БД */
    mainBD.close();
}

void e()
{
    /* Отчищаем экран */
    system("cls");

    KursBDClass mainBD;

    /* Неупорядоченные базы данных */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* Открываем БД */
    openBD(&mainBD, mpath);

    /* Заносим данные в конец файла */
    mainBD.insert("124;Galina;Zuyeva;46;Cleaner");

    /* записываем файл */
    mainBD.write_buffer();

    cout << "Результат в файле: " << mpath << "\n\n\n";

    /* Закрываем БД */
    mainBD.close();
}

void ex1()
{
    unsigned int i; // счетчик

    /* Отчищаем экран */
    system("cls");

    /* Создаем экземпляр класса */
    KursBDClass mainBD, scnDB;

    /* Неупорядоченные базы данных */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    string spath = DBPATH;
    spath += SECONDDBNAME;

    /* Рабочие базы данных */
    string sopath = DBPATH;
    sopath += SORTBDNAME;

    cout << "Задание 1:\n\n\n";

    string sort_field = "FIRSTNAME"; // поле для сортировки

    /* открываем базу данных */
    openBD(&mainBD, mpath);
    openBD(&scnDB, spath);

    /* сортируем базу данных (поскольку база, по заданию, должна быть отсортирована) */
    mainBD.sort(sort_field);

    /* Соединяем и сортируем базы данных */
    cout << "Соедниение баз данных\n";
    for (i = 0; i < scnDB.get_length(); i++)
        mainBD.add(scnDB.get_entry(i));

    sort_field = mainBD.get_sort_field();
    cout << "Сортировка по полю" << sort_field << "\n";
    mainBD.sort(mainBD.get_sort_field());

    /* Записываем данные в файл */
    mainBD.write_buffer(sopath);

    cout << "Результат в файле: " << sopath << "\n\n\n";
    scnDB.close();
    mainBD.close();

    /* Остановка программы */
    system("pause");
}

void ex2()
{

    /* Отчищаем экран */
    system("cls");

    /* Создаем экземпляр класса */
    KursBDClass mainBD;

    /* Неупорядоченные базы данных */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* Рабочие базы данных */
    string selpath = DBPATH;
    selpath += SELECTBDNAME;

    cout << "Задание 2:\n\n\n";

    openBD(&mainBD, mpath);

    string field1 = "YEARS";
    string value1 = "30";

    string field2 = "FIRSTNAME";
    string value2 = "Maria";

    string field3 = "POSITION";
    string value3 = "Admin";

    /* Выполняем первую выборку */
    cout << "Сортировка по полю: " << field1 << "; со занчением: " << value1 << "\n";
    /* Выполняем вторую выборку*/
    mainBD.select(field1, value1);
    cout << "Сортировка по полю: " << field2 << "; со занчением: " << value2 << "\n";
    /* Выполняем третью выборку */
    mainBD.next_select(field2, value2);
    cout << "Сортировка по полю: " << field3 << "; со занчением: " << value3 << "\n";
    mainBD.next_select(field3, value3);

    /* Записываем данные в файл */
    mainBD.write_buffer(selpath);
    mainBD.close();

    /* Остановка программы */
    system("pause");
}
