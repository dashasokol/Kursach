/** 
 *  @headerfile	helpfun.h
 */
#ifndef HELPFUN_H_
#define HELPFUN_H_

#define STRING_MAX_LINE 500     ///< максимальное число символов на строку

#define NUMBER "0123456789"     ///< Символы из которых может состоять число
#define BSIZE 60000             ///< Максимальный размер временного буфера

/**
 * @enum Типы переменных
 */
enum srt_type {
    T_INT = 1,           // тип int
    T_CHAR = 2,          // тип char
};

/* Функция записи символов в конкретную позицию файла */
int fmwrite(FILE * file, const void * Data, int size);

/* Функция удаления символов с конкретной позиции файла */
int fmclean(FILE * file);

/* Оболочка для функции strcmp, исключающая ошибки (остутсвие строки) */
int strmcmp(const char *str1, const char *str2);

/* Функция, открывающая файл и отключающая буферизацию записи */
FILE *fmopen(const char *file, const char *flag, const char *errstr);

/* Функция сортировки одного массива, относительно элементов другого */
void qsort_proc(int *order, void *vmas, size_t length, int left,
                int right, int (*cmp) (const void *, int, int));

/* Функция оболочка над qsort_proc */
void qsort_dmas(int *order, void *mas, unsigned int col_el, srt_type type);

/* Функция равнения лексиграфического строк, переданных через void указатель */
int cmp_string(void *csmas, int pos1, int pos2);

/* Функция сравнения целочисленных значений */
int cmp_int(void *csmas, int pos1, int pos2);

/* Функция, меняющая местами значения элементов целочисленного массива */
void swap(int *mas, int var1, int var2);

#endif /* CONF_H_ */

