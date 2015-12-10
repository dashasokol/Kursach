/** 
 *  @headerfile	helpfun.h
 */
#ifndef HELPFUN_H_
#define HELPFUN_H_

#define STRING_MAX_LINE 500     ///< максимальное число символов на строку
#define REG_MAX_EX 50           ///< максимальное количество элементов регулярного выражения

#define NUMBER "0123456789"     ///< Символы из которых может состоять число
#define BSIZE 60000             ///< Максимальный размер временного буфера

/**
 * @enum Вектора действий со строкой
 */
enum vector_type {
    ALL = 1,            // вся строка
    RIGHT = 2,          // с конца строки
    LEFT = 3,           // с начала строки
    RIGHTLEFT = 4,      // с начала и с конца строки
};

/**
 * @enum Типы переменных
 */
enum srt_type {
    T_INT = 1,           // тип int
    T_CHAR = 2,          // тип char
};

/* Функция вычиления первого вхождения подстроки в строку */
int indexOf(char *str, char *substring, enum vector_type vector);

/* Функция удаляния пробелов из строки */
int spacecut(char *out, char *str, enum vector_type vector);

/* Функция посчёта вхождений подстроки в строку */
int colMatch(char *string, char *substring);

/* Функция записи сиволов в конкретную позицию файла */
int fmwrite(FILE * file, void * Data, int size, int from);

/* Функция удаления сиволов с конкретной позиции файла */
int fmclean(FILE * file, int size, unsigned int from);

/* Оболочка для функции strcmp, исключающая ошибки (остутсвие строки) */
int strmcmp(const char *str1, const char *str2);

/* Функция, открывающая файл и отключающая буферизацию записи */
FILE *fmopen(char *file, const char *flag, const char *errstr);

/* Функция сортировки одного массива, относительно элементов другого */
void qsort_proc(int *order, void *vmas, size_t length, int left,
                int right, int (*cmp) (const void *, int, int));

/* Функция оболочка над qsort_proc */
void qsort_dmas(int *order, void *mas, size_t length,  srt_type type);

/* Функция равнения лексиграфического строк, переданных через void указатель */
int cmp_string(void *csmas, int pos1, int pos2);

/* Функция сравнения целочисленных значений */
int cmp_int(void *csmas, int pos1, int pos2);

/* Функция, меняющая местами значения элементов целочисленного массива */
void swap(int *mas, int var1, int var2);

#endif /* CONF_H_ */
