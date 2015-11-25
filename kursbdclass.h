#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#include <stdio.h>

#define NAMELEN 50 // максимальная длинна имени и фамилии
#define POSLEN 100 // максимальная длинна названия должности
#define TABLELINES 100 // максимальное количество записей в таблице
#define LINELEN 255 // максимальная длинна строки

struct table
{
    int id; // идентификатор
    char fname[NAMELEN]; // имя
    char lname[NAMELEN]; // фамилия
    int years; // количество лет
    char position[POSLEN]; // должность
};

/*
 *
 */
class KursBDClass
{
    struct table tb[TABLELINES]; // структура с данными
    int table_length; // количество записей в БД
    FILE *bd_out_file; // файл вывода
    int parse(char *string_to_parse);  // разбиение строки
    int findId(int id); // поиск элемента
    int create(char *BD_file_name); // создание новой базы
    int checkSpace();
public:
    KursBDClass();
    int open(char *BD_file_name); // открытие базы
    void close(); // закрытие базы
    void select(char *query_string); // выборка
    void insert(char *insert_string); // вставка
    void del(char *query_string); // удаление
    int merge(char *if_BD, char *of_BD); // слияние
};

#endif // KURSBDCLASS_H
