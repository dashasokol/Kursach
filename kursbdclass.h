#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#include "helpfun.h"

#define NAMELEN 50 // максимальная длинна имени и фамилии
#define POSLEN 100 // максимальная длинна названия должности
#define TABLELINES 100 // максимальное количество записей в таблице
#define LINELEN 255 // максимальная длинна строки
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
};

enum
{
    END_WRONG_FORMAT = -2, // неверный формат
    END_NOT_FOUND,   // поиск завершился неудачей
    END_OK,           // функция завершилась без ошибки
    END_EXIT,           // функция завершилась, обработка не треубется
    END_OPEN_FAIL,    // проблема с открытием файла

};

/*
 *
 */
class KursBDClass
{
    struct table tb[TABLELINES]; // структура с данными
    char string_for_write[LINELEN * TABLELINES];
    unsigned int order_len;
    int order[TABLELINES]; // массив, по которому определяется порядок
    unsigned int table_length; // количество записей в БД
    FILE *bd_out_file; // файл вывода
    unsigned int counter; // количество символов в файле
    int parse(char *string_to_parse, struct table *data_table, unsigned int *tb_len);  // разбиение строки
    int getValue(unsigned int *var, char *val); // get_value для целочисленной переменной
    int getValue(char *var, char *val); // get_value для строковой переменной
    int clean_db(FILE *bd); // удаление старых данных
    int del_from_db(FILE *bd, int size, int pos); // добавить в файл
    int add_to_bd(FILE *bd, char *string, int pos); // добавить в файл
    void valueInsert(char *string, char *value); // добавление значения в строку формата БД
    void stringInsert(char *string, struct table insert_value); // добавление строки в выводную строку формата БД
    int sort_table(int *ord, char *field, struct table *data_table, unsigned int tb_len); // сортировка
    FILE *open_and_parse(char *BD_file_name, struct table *data_table, unsigned int *tb_len);
    void insert_and_sort(struct table *insert_value, unsigned int insert_value_len, char *field); //вставка значений в таблицу и сортировка
    void get_order_string(); // получение строки вывода
    void order_clear(unsigned int length); // сброс порядка
public:
    KursBDClass(); // инициализация
    int open(char *BD_file_name); // открытие базы
    void close(); // закрытие базы
    void select(char *field, unsigned int value); // выборка
    void select(char *field, char *value); // выборка
    void insert(struct table insert_value); // вставка
    void del(char *field, unsigned int value); // выборка
    void del(char *field, char *value); // выборка
    int sort(char *field); // сортировка и запись
    void insert_sort(struct table insert_value, char *field); // добавление значения в отсортированную таблицу
    void merge(char *if_DB, char *field);
    int write_buffer(char *s_file_name);
    int write_buffer();
};

#endif // KURSBDCLASS_H
