/**
 *  @file	helpfun.c
 *  @brief	Вспомогательные функции, которые нужны в большинстве программ, но для которых не существует библиотек в базовом языке
 * 
 */
#include <iostream>     // printf, scanf, NULL
#include <cstring>      // strstr, str[n]cat, strlen
#include <cstdlib>      // malloc, free, rand
#include <unistd.h>     // ftruncate
#include "../include/helpfun.h"

/**
 * @fn int fmwrite(FILE * file, void * Data, int size)
 * @brief Функция записи символов в конкретную позицию файла
 * @param file - Указатель на файл
 * @param Data - Указатель на блок данных
 * @param size - Размер блока данных
 * @param from - Смещение относительно начала файла
 * @return Код ошибки / удачного завершения
 */
int fmwrite(FILE * file, const void * Data, int size)
{
    /* переходим в место вставки */
    fseek(file, 0, SEEK_SET);
    
    /* записываем данные в файл и посылаем информацию от функции записи на выход */
    if (fwrite(Data, size, 1, file) == 1)
        return 0;
    else
        return 1;
}

/**
 * @fn int fmclean(FILE * file, int size, int from)
 * @brief Функция удаления символов с конкретной позиции файла
 * @param file - Указатель на файл
 * @param size - Размер блока данных (если -1 - отчистить весь файл)
 * @param from - Смещение относительно начала файла
 * @return Код ошибки / удачного завершения
 */
int fmclean(FILE * file)
{
    fseek(file, 0, SEEK_SET);
    /* обрезаем конец файла и посылаем информацию от функции записи на выход */
    return ftruncate(fileno(file), ftell(file));
}

/**
 * @fn int strmcmp(const char *str1, const char *str2)
 * @brief Оболочка для функции strcmp, исключающая ошибки (остутсвие строки)
 * @param str1 - Первая строка
 * @param str2 - Вторая строка
 * @return -1 - первая строка меньше, 0 - строки одинаковы,
 *          1 - вторая строка меньше, 255 - ошибка
 */
int strmcmp(const char *str1, const char *str2)
{
    int ret;    // Код возврата

    /* если обса значения существуют, сравнить их */
    if (str1 && str2)
    {
        ret = strcmp(str1, str2);
        return ret > 0 ? 1 : ret < 0 ? -1 : 0;
    }

    /* если одно из значений или оба являются NULL вывести код ошибки */
    return 255;
}

/**
 * @fn FILE *fmopen(char *file, const char *flag, const char *errstr)
 * @brief Функция открывающая файл и отключающая буферизацию записи
 * @param file - Полное имя файла
 * @param flag - Режим открытия файла
 * @param errstr - Сообщение для вывода, в случае неудачного открытия файла
 * @return Дескриптор файла
 *
 * Примечание: Чтобы не использовать fflush (сброс буферов)
 */
FILE *fmopen(const char *file, const char *flag, const char *errstr)
{
    FILE *temp = fopen(file, flag);     // Дескриптор файла

    /* убираем буферизацию записи в файл */
    setvbuf(temp, NULL, _IONBF, 0);

    // если файл не открыт
    if (!temp)
        // записываем ошибку в поток вывода отладочных сообщений
        perror(errstr);

    return temp;
}

/**
 * @fn void qsort_proc(int *order, void *vmas, int left, int right,
                       int (*cmp) (void *, int, int))
 * @brief Функция сортировки одного массива, относительно элементов другого
 * @param order - Индексы, которые будут сортироваться
 * @param vmas - Массив значений из структуры, по которому будет сортироваться order
 * @param left - Начальный индекс сортируемых элементов
 * @param right - Конечный индекс сортируемых элементов
 * @param cmp - обработчик сравнения значений
 *
 * Примечание: Для случая, когда нужно сортировать структуры по индексу
 */
void qsort_proc(int *order, void *vmas, int left, int right,
                int (*cmp) (void *, int, int))
{
    int i = 0;          // счетчик
    int last = 0;       // последний элемент

    /* если уже отсортировано или в массиве один элемент */
    if (left >= right)
        return;

    /* меняем местами крайний левый и средний элементы */
    swap(order, left, (left + right)/2);
    last = left;

    /* сортируем массив относительно первого элемента */
    for (i = left+1; i <= right; i++)
    {
        if (cmp(vmas, order[i], order[left]) < 0)
            swap(order, ++last, i);
    }

    /* меняем местами крайний левый и последний отсортированный*/
    swap(order, left, last);

    /* сортируем левую часть массива */
    qsort_proc(order, vmas, left, last-1, cmp);

    /* сортируем правую часть массива */
    qsort_proc(order, vmas, last+1, right, cmp);
}

/**
 * @fn int cmp_string(void *csmas, int pos1, int pos2)
 * @brief Функция равнения лексиграфического строк, переданных через void указатель
 * @param csmas - Указатель на массив данных
 * @param pos1 - Позиция первой строки
 * @param pos2 - Позиция второй строки
 * @return -1 - первая строка меньше, 0 - строки одинаковы,
 *          1 - вторая строка меньше, 255 - ошибка
 */
int cmp_string(void *csmas, int pos1, int pos2)
{
    char **pcsmas = (char **)csmas; // указатель на массив типа char, получемых
                                    // через void

    /* сравниваем строки */
    return strmcmp(pcsmas[pos1], pcsmas[pos2]);
}

/**
 * @fn cmp_int(void *csmas, int pos1, int pos2)
 * @brief Функция сравнения целочисленных значений
 * @param csmas - Указатель на массив данных
 * @param pos1 - Позиция первой строки
 * @param pos2 - Позиция второй строки
 * @return -1 - первая переменная меньше, 0 - переменные одинаковы,
 *          1 - вторая переменная меньше
 */
int cmp_int(void *csmas, int pos1, int pos2)
{
    int *pcsmas = (int *)csmas; // указатель на массив типа int, получемых
                                // через void

    /* сравниваем значения и возвращаем код завершения функции */
    return pcsmas[pos1] < pcsmas[pos2] ? -1 : \
           pcsmas[pos1] > pcsmas[pos2] ? 1 : 0;
}

/**
 * @fn void qsort_dmas(int *order, void *mas, unsigned int col_el, srt_type type)
 * @brief Функция оболочка над qsort_proc
 * @param order - Индексы, которые будут сортироваться
 * @param mas - Массив значений из структуры, по которому будет сортироваться order
 * @param col_el - Количество элементов, по которым будет происходить сортировка
 * @param type - Тип данных в массиве mas
 */
void qsort_dmas(int *order, void *mas, unsigned int col_el, srt_type type)
{
    /* определяем к какому типу принадлежат данные */
    switch (type)
    {
    case T_INT:
        /* сортируем массив типа int */
        qsort_proc(order, mas, 0, col_el, cmp_int);
        break;
    case T_CHAR:
        /* сортируем массив типа char */
        qsort_proc(order, mas, 0, col_el, cmp_string);
        break;
    default:
        /* тип ввден неверно */
        fprintf(stderr, "Wrong variable type\n");
        return;
    }
}

/**
 * @fn void swap(int *mas, int var1, int var2)
 * @brief Функция, меняющая местами значения элементов целочисленного массива
 * @param mas - Массив значений
 * @param var1 - Первая переменная
 * @param var2 - Вторая переменная
 */
void swap(int *mas, int var1, int var2)
{
    int tmp;    // временная переменная

    /* меняем местами значения элементов */
    tmp = mas[var1];
    mas[var1] = mas[var2];
    mas[var2] = tmp;
}

