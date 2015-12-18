#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#define NAMELEN 50          // максимальная длинна имени и фамилии
#define POSLEN 100          // максимальная длинна названия должности
#define TABLELINES 100      // максимальное количество записей в таблице
#define LINELEN 255         // максимальная длинна строки
#define SYM_SEPARATOR ";"       // символ разделитель полей
#define SYM_STRING_END "\""     // символ ограничитель строковых значений
#define SYM_HEADER "#"
#define SYM_HEADER_SEPARATOR "|"
#define DESCRIPT "#ID|FIRSTNAME|LASTNAME|YEARS|POSITION\n"  // описание таблицы
#define MAXLEN 10

/* структура таблицы */
struct table
{
    unsigned int number; // идентификатор
    std::string field;    // имя
    std::string value;    // фамилия
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
    struct table tb[LINELEN * TABLELINES];                    // структура с данными
    std::string string_for_write;    // строка вывода
    std::string table_header[MAXLEN];                       // шапка таблицы
    unsigned int header_col;
    int sort_field;
    unsigned int order_len;                         // длинна массива, по которому
                                                    // определяется порядок вывода
    unsigned int order[TABLELINES];                          // массив, по которому
                                                    // определяется порядок вывода
    unsigned int table_length;                      // количество записей в БД
    FILE *bd_out_file;                              // дескриптор файла БД
    int getHeader(std::string string_to_parse);
    int parse(std::string string_to_parse);                // Функция обработки строки
    int getValue(std::string *var, std::string val);             // Функция, получающая строковое значение
    int clean_db(FILE *bd);                         // Функция отчищает файл БД
    void stringInsert(unsigned int number);   // Функция вставляет строку содержащую данные
                                                    // одной записи из БД в строку вывода
    void get_order_string();                        // Функция записи данных в строку вывода
    void order_clear(unsigned int length);          // Отчистка массива, по кторому определяется порядок
    int add_to_bd(FILE *bd, const char *string);          // Оболочка для записи данных в файл БД
    int isOrder(unsigned int number);
    int sort_table(std::string field);            // Функция сортировки одного массива относительно
                                                    // значений другого
public:
    KursBDClass();                                  // Конструктор
    ~KursBDClass();                                 // Деструктор
    int open(std::string BD_file_name);                   // Оболочка для функции open_and_parse
    void close();                                   // Функция закрытия БД
    int write_buffer(std::string s_file_name);            // Функция записывает строку вывода в указанный файл
    int write_buffer();                             // Функция записывает строку вывода в основной файл
    void select(std::string field,
                std::string value);          // Функция ищет записи в таблице по послю, содержащему
                                                    // строковые значения
    void add(std::string value);         // Функция вставляет новое значение в строку вывода
    void del(std::string field,             // Удаление всех записей, из поля соответствуюх
             std::string value);             // строковому указанному значению
    int sort(std::string field);                          // Оболочка для функции сортировки по полю
    void insert(std::string value);                  // Вставка в отсортированную БД
};

#endif // KURSBDCLASS_H

