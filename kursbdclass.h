#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#define TABLELINES 50      // максимальное количество записей в таблице
#define LINELEN 255         // максимальная длинна строки
#define SYM_SEPARATOR ";"       // символ разделитель полей
#define SYM_HEADER "#"
#define SYM_HEADER_SEPARATOR "|"
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
    struct table tb[MAXLEN * TABLELINES];               // структура с данными
    std::string string_for_write;                       // строка вывода
    std::string table_header[MAXLEN];                   // шапка таблицы
    unsigned int header_col;                            // количество полей таблицы
    int sort_field;                                     // поле сортировки
    unsigned int order_len;                             // длинна массива, по которому
                                                        // определяется порядок вывода
    unsigned int order[TABLELINES];                     // массив, по которому
                                                        // определяется порядок вывода
    unsigned int table_length;                          // количество записей в БД
    FILE *bd_out_file;                                  // дескриптор файла БД
    int getHeader(std::string string_to_parse);         // Функция получает шапку таблицы
    void getValue(std::string *var, std::string *val);  // Функция, получающая строковое значение
    int clean_db(FILE *bd);                             // Функция отчищает файл БД
    void stringInsert(unsigned int number);             // Функция вставляет строку содержащую данные
                                                        // одной записи из БД в строку вывода
    void get_order_string();                            // Функция записи данных в строку вывода
    void order_clear(unsigned int length);              // Отчистка массива, по кторому определяется порядок
    int add_to_bd(FILE *bd, const char *string);        // Оболочка для записи данных в файл БД
    int isOrder(unsigned int number);                   // Функция проверяет вхождение числа в массив order
    int sort_table(std::string field);                  // Функция сортировки одного массива относительно
                                                        // значений другого
public:
    KursBDClass();                                      // Конструктор
    ~KursBDClass();                                     // Деструктор
    int open(std::string BD_file_name);                 // Функция открывает и читает БД из файла
    void close();                                       // Функция закрытия БД
    int write_buffer(std::string s_file_name);          // Функция записывает строку вывода в указанный файл
    int write_buffer();                                 // Функция записывает строку вывода в основной файл
    void select(std::string field,                      // Функция ищет записи в таблице по послю, содержащему
                std::string value);                     // строковые значения
    void next_select(std::string field,                 // Функция продолжает искать записи в таблице
                     std::string value);                // по полю, содержащему значения
    int add(std::string string_to_parse);              // Функция вставляет новое значение в БД
    void del(std::string field,                         // Удаление всех записей, из поля соответствуюх
             std::string value);                        // указанному значению
    int sort(std::string field);                        // Оболочка для функции сортировки по полю
    void insert(std::string value);                     // Вставка в отсортированную БД
    unsigned int get_length();                          // Функция возвращает размер БД
    std::string get_entry(unsigned int number);         // Функция возвращает указанную запись БД
};

#endif // KURSBDCLASS_H

