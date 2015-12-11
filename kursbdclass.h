#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#define NAMELEN 50          // максимальная длинна имени и фамилии
#define POSLEN 100          // максимальная длинна названия должности
#define TABLELINES 100      // максимальное количество записей в таблице
#define LINELEN 255         // максимальная длинна строки
#define SEPARATOR ";"       // символ разделитель полей
#define STRING_END "\""     // символ ограничитель строковых значений
#define MAX_COLUMNS 5       // максимальное количество колонок в таблице
#define DESCRIPT "#ID|FIRSTNAME|LASTNAME|YEARS|POSITION\n"  // описание таблицы

/* структура таблицы */
struct table
{
    unsigned int id;        // идентификатор
    char fname[NAMELEN];    // имя
    char lname[NAMELEN];    // фамилия
    unsigned int years;     // количество лет
    char position[POSLEN];  // должность
};

/* возвращаемые значения */
enum
{
    END_WRONG_FORMAT = -2,  // неверный формат
    END_NOT_FOUND,          // поиск завершился неудачей
    END_OK,                 // функция завершилась без ошибки
    END_EXIT,               // функция завершилась, обработка не треубется
    END_OPEN_FAIL           // проблема с открытием файла
};

/*
 * Класс для работы с БД
 */
class KursBDClass
{
    struct table tb[TABLELINES];                    // структура с данными
    char string_for_write[LINELEN * TABLELINES];    // строка вывода
    unsigned int order_len;                         // длинна массива, по которому
                                                    // определяется порядок вывода
    int order[TABLELINES];                          // массив, по которому
                                                    // определяется порядок вывода
    unsigned int table_length;                      // количество записей в БД
    FILE *bd_out_file;                              // дескриптор файла БД
    int parse(char *string_to_parse,
              struct table *data_table,
              unsigned int *tb_len);                // Функция обработки строки
    int getValue(unsigned int *var, char *val);     // Функция, получающая целочисленное значение
    int getValue(char *var, char *val);             // Функция, получающая строковое значение
    int clean_db(FILE *bd);                         // Функция отчищает файл БД
    int add_to_bd(FILE *bd, char *string);          // Оболочка для записи данных в файл БД
    void valueInsert(char *string, char *value);    // Функция вставляет очередное значение в строку вывода
    void stringInsert(char *string,
                      struct table insert_value);   // Функция вставляет строку содержащую данные
                                                    // одной записи из БД в строку вывода
    int sort_table(int *ord, char *field,
                   struct table *data_table,
                   unsigned int tb_len);            // Функция сортировки одного массива относительно
                                                    // значений другого
    FILE *open_and_parse(char *BD_file_name,
                         struct table *data_table,
                         unsigned int *tb_len);     // Функция открывает и читает БД из файла
    int insert_and_sort(struct table *insert_value,
                         unsigned int insert_value_len,
                         char *field);              // Функция которая добавляет записи в таблицу и
                                                    // сортирует её
    void get_order_string();                        // Функция записи данных в строку вывода
    void order_clear(unsigned int length);          // Отчистка массива, по кторому определяется порядок
public:
    KursBDClass();                                  // Инициализация
    int open(char *BD_file_name);                   // Оболочка для функции open_and_parse
    void close();                                   // Функция закрытия БД
    void select(char *field, unsigned int value);   // Функция ищет записи в таблице по послю, содержащему
                                                    // целочисленные значения
    void select(char *field, char *value);          // Функция ищет записи в таблице по послю, содержащему
                                                    // строковые значения
    void insert(struct table insert_value);         // Функция вставляет новое значение в строку вывода
    void del(char *field, unsigned int value);      // Удаление всех записей, из поля соответствуюх
                                                    // указанному целочисленному значению
    void del(char *field, char *value);             // Удаление всех записей, из поля соответствуюх
                                                    // строковому указанному значению
    int sort(char *field);                          // Оболочка для функции сортировки по полю
    int insert_sort(struct table insert_value,
                     char *field);                  // Вставка в отсортированную БД
    int merge(char *if_DB, char *field);            // Добавление данных из другой БД и последующая сортировка
    int write_buffer(char *s_file_name);            // Функция записывает строку вывода в указанный файл
    int write_buffer();                             // Функция записывает строку вывода в основной файл
};

#endif // KURSBDCLASS_H
