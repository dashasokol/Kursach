#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#define NAMELEN 50 // максимальная длинна имени и фамилии
#define POSLEN 100 // максимальная длинна названия должности
#define TABLELINES 100 // максимальное количество записей в таблице
#define LINELEN 255 // максимальная длинна строки
#define SEPARATOR ";\0" // символ разделитель полей
#define STRING_END "\"\0" // символ ограничитель строковых значений
#define MAX_COLUMNS 5 // максимальное количество колонок в таблице

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
    END_NOT_FOUND = -1,   // поиск завершился неудачей
    END_OK = 0,           // функция завершилась без ошибки
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
    int get_value(unsigned int *var, char *val); // get_value для целочисленной переменной
    int get_value(char *var, char *val); // get_value для строковой переменной
    int findId(int id); // поиск элемента
    int create(char *BD_file_name); // создание новой базы
    int checkSpace();
public:
    KursBDClass();
    int open(char *BD_file_name); // открытие базы
    void close(); // закрытие базы
    char *select(char *query_string); // выборка
    void insert(char *insert_string); // вставка
    void del(char *query_string); // удаление
    int merge(char *if_BD, char *of_BD); // слияние
};

#endif // KURSBDCLASS_H
