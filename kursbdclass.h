#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

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
    unsigned int fpos;   // позиция в файле
    unsigned int flen;   // размер строки в файле
};

enum
{
    END_WRONG_FORMAT = -2, // неверный формат
    END_NOT_FOUND = -1,   // поиск завершился неудачей
    END_OK = 0,           // функция завершилась без ошибки
};

/*
 *
 */
class KursBDClass
{
    struct table tb[TABLELINES]; // структура с данными
    unsigned int table_length; // количество записей в БД
    FILE *bd_out_file; // файл вывода
    int counter; // количество символов в файле
    int parse(char *string_to_parse);  // разбиение строки
    int getValue(unsigned int *var, char *val); // get_value для целочисленной переменной
    int getValue(char *var, char *val); // get_value для строковой переменной
    int clean_db(FILE *bd); // удаление старых данных
    int del_from_db(FILE *bd, int size, int pos); // добавить в файл
    int add_to_bd(FILE *bd, char *string, int pos); // добавить в файл
    void valueInsert(char *string, char *value); // добавление значения в строку формата БД
    void stringInsert(char *string, struct table insert_value); // добавление строки в выводную строку формата БД
public:
    KursBDClass(); // инициализация
    int open(char *BD_file_name); // открытие базы
    void close(); // закрытие базы
    void select(char *s_file_name, char *field, unsigned int value); // выборка
    void select(char *s_file_name, char *field, char *value); // выборка
    void insert(struct table insert_value); // вставка
    void del(char *field, unsigned int value); // выборка
    void del(char *field, char *value); // выборка
    void sort(char *field); // сортировка
    int merge(char *if_BD, char *of_BD); // слияние
};

#endif // KURSBDCLASS_H
