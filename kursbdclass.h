#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#include "helpfun.h"

#define NAMELEN 50 // максимальная длинна имени и фамилии
#define POSLEN 100 // максимальная длинна названия должности
#define TABLELINES TLSIZE // максимальное количество записей в таблице
#define LINELEN SSIZE // максимальная длинна строки
#define SEPARATOR ";" // символ разделитель полей
#define STRING_END "\"" // символ ограничитель строковых значений
#define MAX_COLUMNS 5 // максимальное количество колонок в таблице
#define DESCRIPT "#ID|FIRSTNAME|LASTNAME|YEARS|POSITION\n" // описание таблицы

// структура таблицы
struct table
{
    unsigned int id; // идентификатор
    char fname[NAMELEN]; // имя
    char lname[NAMELEN]; // фамилия
    unsigned int years; // количество лет
    char position[POSLEN]; // должность
    unsigned int fpos;   // позиция в файле
    unsigned int flen;   // размер строки в файле
};

enum
{
    END_WRONG_FORMAT = -2, // неверный формат
    END_NOT_FOUND,   // поиск завершился неудачей
    END_OK,           // функция завершилась без ошибки
    END_EXIT,           // функция завершилась, обработка не треубется
};

/*
 *
 */
class KursBDClass
{
    struct table tb[TABLELINES]; // структура с данными
    unsigned int table_length; // количество записей в БД
    FILE *bd_out_file; // файл вывода
    unsigned int counter; // количество символов в файле
    int parse(char *string_to_parse, struct table (*data_table)[TABLELINES], unsigned int *tb_len);  // разбиение строки
    int getValue(unsigned int *var, char *val); // get_value для целочисленной переменной
    int getValue(char *var, char *val); // get_value для строковой переменной
    int clean_db(FILE *bd); // удаление старых данных
    int del_from_db(FILE *bd, int size, int pos); // добавить в файл
    int add_to_bd(FILE *bd, char *string, int pos); // добавить в файл
    void valueInsert(char *string, char *value); // добавление значения в строку формата БД
    void stringInsert(char *string, struct table insert_value); // добавление строки в выводную строку формата БД
    void sort_table(char *buff, char *field); // сортировка
    int open_and_parse(char *BD_file_name, struct table (*data_table)[TABLELINES], unsigned int *tb_len, FILE *out_file, unsigned int *cnt);
public:
    KursBDClass(); // инициализация
    int open(char *BD_file_name); // открытие базы
    void close(); // закрытие базы
    void select(char *s_file_name, char *field, unsigned int value); // выборка
    void select(char *s_file_name, char *field, char *value); // выборка
    void insert(struct table insert_value); // вставка
    void del(char *field, unsigned int value); // выборка
    void del(char *field, char *value); // выборка
    void sort(char *s_file_name, char *field); // сортировка и запись
    void insert_sort(char *s_file_name, struct table insert_value); // добавление в отсортированную таблицу
};

#endif // KURSBDCLASS_H
