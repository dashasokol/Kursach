/**
 *  @file	helpfun.c
 *  @brief	Вспомогательные функции, которые нужны в большинстве программ, но для которых не существует библиотек в базовом языке
 * 
 */
#include <iostream>     // printf, scanf, NULL
#include <cstring>      // strstr, str[n]cat, strlen
#include <cstdlib>      // malloc, free, rand
#include <unistd.h>     // ftruncate
#include "helpfun.h"    // Заголовочный файл данного модуля

/**
 * @fn int indexOf(char *str, char *substring, enum vector_type vector)
 * @brief Функция вычиления первого вхождения подстроки в строку
 * @param str - Строка в которой производится поиск
 * @param substring - Искомая строка
 * @param vector - Сторока с которой производится поиск символа
 * @return Индекс первого вхождения символа в строку / -1 - строка не найдена
 *
 * Пример:\n
 * @code
 * indexOf(string, substring, LEFT); // Определние индекса первого вхождения с начала строки
 * indexOf(string, substring, RIGHT); // Определние индекса первого вхождения с конца строки
 * @endcode
 */
int indexOf(char *str, char *substring, enum vector_type vector)
{
    char *rem_string;	// указатель на обрезанную строку
    int i;              // счётчик


    if (vector == LEFT) {
        /* находим первое вхождение */
        rem_string = strstr(str, substring);

        /* если найдена строка высчитать её местоположение */
        if (rem_string)
            return strlen(str) - strlen(rem_string);
    }
    else if (vector == RIGHT)
    {
        /* находим последнее вхождение */
        for (i=strlen(str)-1; i >= 0; i--)
        {
            rem_string = strstr(&str[i], substring);

            /* если найдена строка высчитать её местоположение */
            if (rem_string)
                return strlen(str) - strlen(rem_string);
        }
    }
    return -1;
}

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
int spacecut(char *out, char *str, enum vector_type vector)
{
    char string[STRING_MAX_LINE];   // строка, которая будет преобразовываться
    char cut_string[strlen(str)];   // строка, которая будет вырезана из string
    unsigned int i,                 // счётчик
                 j;                 // счётчик

    if (strspn(str, " ") != strlen(str))
    {
        /* обнуляем массив со строкой удаления */
        memset(cut_string, 0, sizeof(cut_string));
        strcat(cut_string, " ");

        /* перемещаем данные в строку для изменения */
        if (vector != ALL)
            strcpy(string, str);

        /* удаляем начальные пробелы */
        if (vector == LEFT || vector == RIGHTLEFT)
        {
            /* заполняем строку для удаления пока в начале есть пробелы */
            while (indexOf(string, cut_string, LEFT) == 0)
                strcat(cut_string, " ");

            strcpy(string, &string[strlen(cut_string)-1]);

            /* обнуляем массив со строкой удаления */
            memset(cut_string, 0, sizeof(cut_string));

            /* устанавливаем начальное значение строки, для вырезания */
            strcat(cut_string, " ");

            /* вычисляем строку вывода */
            if (vector == LEFT)
                memset(out, 0, (int) sizeof(out));
                strcpy(out, string);
        }

        /* удаляем пробелы из конца строки */
        if (vector == RIGHT || vector == RIGHTLEFT)
        {
            while (indexOf(string, cut_string, RIGHT) > -1 && (unsigned int) indexOf(string, cut_string, RIGHT) == strlen(string)-strlen(cut_string))
                strcat(cut_string, " ");

            /* вычисляем строку вывода */
            memset(out, 0, (int) sizeof(out));
            strncpy(out, string, strlen(string)-strlen(cut_string)+1);
        }

        /* удаляем все пробелы из строки */

        if (vector == ALL)
        {
            /* отчищаем строку для изменения */
            memset(string, 0, sizeof(string));

            /* добавляем посимвольно строку, исключая пробелы */
            for (i=j=0;i<strlen(str);i++)
            {
                if (str[i] != ' ')
                    string[j++] = str[i];
            }
            /* вычисляем строку вывода */
            memset(out, 0, (int) sizeof(out));
            strcpy(out, string);
        }

        /* вставляем нуль-терминатор */
        out[strlen(string)-strlen(cut_string)+1] = '\0';
    }
    else
        return 1;
            
    /* выводим обработанную строку */
    return 0;
}

/**
 * @fn int colMatch(char *string, char *substring)
 * @brief Функция посчёта вхождений подстроки в строку
 * @param string
 * @param substring
 * @return Количество вхождений
 */
int colMatch(char *string, char *substring)
{
    int index = -1;         // новое положение коретки
    unsigned int ret = 0;   // счтётчик

    /* обрезаем строку при нахождении подстроки в строке */
    while (indexOf(&string[index+1], substring, LEFT) >= 0)
        {
            index=index+indexOf(&string[index+1], substring, LEFT) + 1;
            ret++;
        }

    return ret;
}

/**
 * @fn int fmwrite(FILE * file, void * Data, int size, int from)
 * @brief Функция записи сиволов в конкретную позицию файла
 * @param file - Указатель на файл
 * @param Data - Указатель на блок данных
 * @param size - Размер блока данных
 * @param from - Смещение относительно начала файла
 * @return Код ошибки / удачного завершения
 */
int fmwrite(FILE * file, void * Data, int size, int from)
{
    unsigned int s;         // размер файла
    unsigned int nsize;     // размер итогового файла
    unsigned int alloc;     // требуемый размер буфера, значение меньшее, чем BSIZE или равное ему
    int pose;               // текщая позиция (при переносе) или итоговая
    unsigned int read;      // размер смещаемой информации
    char * temp;            // буфер, содержащий строку для смещения

    /* переходим в конец файла */
    fseek(file, 0, SEEK_END);

    /* определяем размер итогового файла */
    s = ftell(file);
    nsize = s + size;

    /* переходим в место вставки */
    fseek(file, from, SEEK_SET);

    /* определяем требуемый размер буфера и выделяем память */
    alloc = BSIZE > s ? s-from : BSIZE;
    temp = (char*) malloc(alloc);
	
    /* зануляем массив */
    memset(temp, '\0', alloc);
    pose = nsize;
    
    while (pose > from + size)
    {
        /* определяем, сколько информации необходимо сместить */
        read = pose-size <= from ? size : alloc;
        
        /* считываем в буфер строку для смещения */
        fseek(file,pose - size - read,SEEK_SET);
        fread((void*)temp,read,1,file);

        /* подсчёт переносов, для вычисления переносов каретки (carr = 0 для Linux) */
        int carr = colMatch(temp, (char *) "\n");
        read -= carr;
        temp[read] = '\0';

        /* вставляем данные из буфера в файл, начиная с символо от которого они должны идти */
        fseek(file, pose - (read + carr - 1), SEEK_SET);
        fwrite((void*) temp, read, 1, file);
		
        /* переходим на позицию, для следующего смещения */
        pose -= read;
    }
    
    /* переходим на позицию записи */
    fseek(file, from, SEEK_SET);
    
    /* записываем данные в файл и посылаем информацию от функции записи на выход */
    if (fwrite(Data, size, 1, file) == 1)
        return 0;
    else
        return 1;
}

/**
 * @fn int fmclean(FILE * file, int size, int from)
 * @brief Функция удаления сиволов с конкретной позиции файла
 * @param file - Указатель на файл
 * @param size - Размер блока данных (если -1 - отчистить весь файл)
 * @param from - Смещение относительно начала файла
 * @return Код ошибки / удачного завершения
 */
int fmclean(FILE * file, int size, unsigned int from)
{
    unsigned int s;                     // размер файла
    unsigned int nsize;                 // размер итогового файла
    unsigned int alloc;                 // требуемый размер буфера,
                                        // значение меньшее, чем BSIZE или равное ему
    unsigned int pose;                  // текщая позиция
    char *temp;                         // буфер, содержащий строку для смещения
    unsigned int offset = from + size;  // начало следующего блока данных
    char last[1];                       // последний символ
    
    /* переходим в конец файла */
    fseek(file, 0, SEEK_END);
    
    /* определяем размер итогового файла */
    s = ftell(file);

    /* отчистить весь файл */
    if (size < 0)
        size = s;

    /*
     * если файл меньше, чем возможно удалить, уменьшеить
     * требуемое количество знаков для удаления
     */
    if (s < from + size)
        size = s - from;

    /* определяем, есть ли символ перевода строки в конце файла, если нет, то ставим */
    fgets(last, 1, file);
    if (last[0] == '\n')
        s++;
    
    /* вычисляем размер итогового файла */
    nsize = s - size;

    /* переходим в место вставки */
    fseek(file, offset, SEEK_SET);

    /* определяем требуемый размер буфера и выделяем память */
    alloc = BSIZE > s ? s-offset : BSIZE;
    temp = (char*) malloc(alloc);

    /* перемещаем блоки данных, замещая удаленные данные */
    pose = from;
    if (pose != nsize)
        while (pose + alloc <= nsize)
        {
            /* считываем в буфер строку для смещения */
            fseek(file,pose + size,SEEK_SET);
            fread((void*)temp, alloc, 1, file);

            /* вставляем данные из буфера в файл, начиная с символо от которого они должны идти */
            fseek(file, pose, SEEK_SET);
            fwrite((void*)temp, alloc, 1, file);

            /* переходим на позицию, для следующего смещения */
            pose += alloc;
        }
    
    /* переходим на позицию записи */
    fseek(file, from, SEEK_SET);

    /* обрезаем конец файла и посылаем информацию от функции записи на выход */
    return ftruncate(fileno(file), nsize);
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
FILE *fmopen(char *file, const char *flag, const char *errstr)
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
