/** 
 *  @headerfile	helpfun.h
 */
#ifndef HELPFUN_H_
#define HELPFUN_H_

#define STRING_MAX_LINE 500                                            ///< максимальное число символов на строку
#define REG_MAX_EX 50                                                  ///< максимальное количество элементов регулярного выражения


#define NAME "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"      ///< Символы из которых может состоять имя
#define TYPE "[]_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"   ///< Символы из которых может состоять тип
#define PATH "/_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"     ///< Символы из которых может состоять путь
#define WORD "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"                 ///< Символы из которых может состоять слово
#define NUMBER "0123456789"                                                            ///< Символы из которых может состоять число
#define BSIZE 60000                                                                 ///< Максимальный размер временного буфера
#define SSIZE 255
#define TLSIZE 100

/**
 * @enum Вектора действий со строкой
 */
enum vector_type {
    ALL = 1,            // вся строка
    RIGHT = 2,          // с конца строки
    LEFT = 3,           // с начала строки
    RIGHTLEFT = 4,      // с начала и с конца строки
};

enum {
    T_INT = 1,            // тип int
    T_CHAR = 2,          // тип char
};

/**
 * @fn int indexOf(char *str, char *substring, enum vector_type vector)
 * @brief Функция вычиления первого вхождения подстроки в строку
 * @param str - Строка в которой производится поиск
 * @param substring - Искомая строка
 * @param vector - Сторона с которой производится поиск символа
 * @return Индекс первого вхождения символа в строку / -1 - строка не найдена
 * 
 * Пример:\n
 * @code
 * indexOf(string, substring, LEFT); // Определние индекса первого вхождения с начала строки
 * indexOf(string, substring, RIGHT); // Определние индекса первого вхождения с конца строки
 * @endcode
 */
int indexOf(char *str, char *substring, enum vector_type vector);

/**
 * @fn int charcut(char *out, char *str, char *substring)
 * @brief Функция удаления всех вхождений подстроки в строку
 * @param out - Строка вывода
 * @param str - Строка для обработки
 * @param substring - Искомая строка
 * @return 0 - удаление произведено / 1 - искомая строка не найдена
 *
 * Пример:\n
 * Строка line: "12121212"\n
 * Подстрока subline: "1"\n
 * Строка, в которую выводится данные endline: ""\n
 * @code
 * charcut(endline, line, subline);
 * @endcode
 * endline: "2222"
 */
int charcut(char *out, char *str, char *substring);

/**
 * @fn int spacecut(char *out, char *str, enum vector_type vector)
 * @brief Функция удаляния пробелов из строки
 * @param out - Строка вывода
 * @param str - Строка для обработки
 * @param vector - Сторона с которой производится поиск символа
 * @return 0 - удаление произведено / 1 - искомая строка не найдена
 *
 * Пример:\n
 * Строка line: " 1 1 1 1 "\n
 * @code
 * spacecut(endline, line, LEFT); // endline: "1 1 1 1 "
 * spacecut(endline, line, RIGHT); // endline: " 1 1 1 1"
 * spacecut(endline, line, RIGHTLEFT); // endline: "1 1 1 1"
 * spacecut(endline, line, ALL); // endline: "1111"
 * @endcode
 */
int spacecut(char *out, char *str, enum vector_type vector);

/**
 * @fn int colMatch(char *string, char *substring)
 * @brief Функция посчёта вхождений подстроки в строку
 * @param string
 * @param substring
 * @return Количество вхождений
 */
int colMatch(char *string, char *substring);

/**
 * @fn int fmwrite(FILE * file, void * Data, int size, int from)
 * @brief Функция записи сиволов в конкретную позицию файла
 * @param file - Указатель на файл
 * @param Data - Указатель на блок данных
 * @param size - Размер блока данных
 * @param from - Смещение относительно начала файла
 * @return Код ошибки / удачного завершения
 */
int fmwrite(FILE * file, void * Data, int size, int from);

/**
 * @fn int fmclean(FILE * file, int size, int from)
 * @brief Функция удаления сиволов с конкретной позиции файла
 * @param file - Указатель на файл
 * @param size - Размер блока данных (если -1 - отчистить весь файл)
 * @param from - Смещение относительно начала файла
 * @return Код ошибки / удачного завершения
 */
int fmclean(FILE * file, int size, unsigned int from);

/**
 * @fn int strmcmp(const char *str1, const char *str2)
 * @brief Оболочка для функции strcmp, исключающая ошибки
 * @param str1 - Первая строка
 * @param str2 - Вторая строка
 * @return Код ошибки / удачного завершения
 */
int strmcmp(const char *str1, const char *str2);

/**
 * @fn int fileExist(char *file)
 * @brief Функция для проверки существования файла
 * @param file - Полное имя файла
 * @return 0 - файл найден / 2 - файл не найден
 */
int fileExist(char *file);

FILE *fmopen(char *file, const char *flag, const char *errstr);

void qsort_dmas(int *v, int *mas, int left, int right);

void qsort_dmas(int *v, char (*mas)[TLSIZE], int left, int right);

void qsort_dmas(int *v, void *mas, unsigned int maslen, int left, int right, int type);

void swap(int *mas, int var1, int var2);

#endif /* CONF_H_ */
