/**
 *  @file	helpfun.c
 *  @brief	Вспомогательные функции, которые нужны в большинстве программ, но для которых не существует библиотек в базовом языке
 * 
 */
#include <stdio.h>                        // printf, scanf, NULL 
#include <string.h>                       // strstr, str[n]cat, strlen
#include <stdlib.h>                       // malloc, free, rand
#include <unistd.h>
#include "helpfun.h"    						// Заголовочный файл данного модуля

// Функция вычиления первого вхождения подстроки в строку
int indexOf(char *str, char *substring, enum vector_type vector)
{
    char *rem_string;	// указатель на обрезанную строку
    int i; 		// счётчик

    if (vector == LEFT) {
        rem_string = strstr(str, substring); // находим первое вхождение

        /* если найден разделитель высчитать его местоположение */
        if (rem_string)
            return strlen(str) - strlen(rem_string);
    }
    else if (vector == RIGHT)
    {
        /* находим последнее вхождение */
        for (i=strlen(str)-1; i >= 0; i--)
        {
            rem_string = strstr(&str[i], substring);
            if (rem_string)
                return strlen(str) - strlen(rem_string);
        }
    }
    return -1;
}

// Функция удаления всех вхождений подстроки в строку
int charcut(char *out, char *str, char *substring)
{
    unsigned int i,                                            // счётчик
          j;                                            // счётчик
    char string[STRING_MAX_LINE];                       // строка, которая будет преобразовываться
    unsigned int found = 0;                                      // символ найден
    
    /* отчищаем строку для изменения */
    memset(string, 0, sizeof(string));

    /* добавляем посимвольно строку, исключая пробелы */
    for (i=j=0;i<strlen(str);i++)
    {
        if (strspn(&str[i], substring) == 0)
            string[j++] = str[i];
        else
            found = 1;
    }
    
    /* вычисляем строку вывода */
    memset(out, 0, (int) sizeof(out));
    strcpy(out, string);

    /* вставляем нуль-терминатор */
    out[strlen(string)+1] = '\0';

    /* выводим обработанную строку */
    return found;
}

// Функция удаляния пробелов из строки
int spacecut(char *out, char *str, enum vector_type vector)
{
    char string[STRING_MAX_LINE];                       // строка, которая будет преобразовываться
    char cut_string[strlen(str)];                       // строка, которая будет вырезана из string
    unsigned int i, 											// счётчик
          j;                                            // счётчик

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

// Функция посчёта вхождений подстроки в строку
int colMatch(char *string, char *substring)
{
    int index = -1; // новое положение коретки
    unsigned int ret = 0;    // счтётчик

    /* обрезаем строку при нахождении подстроки в троке */
    while (indexOf(&string[index+1], substring, LEFT) >= 0)
        {
            index=index+indexOf(&string[index+1], substring, LEFT) + 1;
            ret++;
        }

    return ret;
}

// Функция записи сиволов в конкретную позицию файла
int fmwrite(FILE * file, void * Data, int size, int from)
{
    unsigned int s;          // размер файла
    unsigned int nsize;      // размер итогового файла
    unsigned int alloc;      // требуемый размер буфера, значение меньшее, чем BSIZE или равное ему
    int pose;       // текщая позиция (при переносе) или итоговая
    unsigned int read;       // размер смещаемой информации
    char * temp;    // буфер, содержащий строку для смещения

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
	
    memset(temp, '\0', alloc);
    pose = nsize;
    
    while (pose > from + size)
    {
        /* определяем, сколько информации необходимо сместить */
        read = pose-size <= from ? size : alloc;
        
        /* считываем в буфер строку для смещения */
        fseek(file,pose - size - read,SEEK_SET);
        fread((void*)temp,read,1,file);

        int carr = colMatch(temp, (char *) "\n"); // подсчёт переносов, для вычисления переносов каретки
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

// Функция удаления сиволов с конкретной позиции файла
int fmclean(FILE * file, int size, unsigned int from)
{
    unsigned int s;                      // размер файла
    unsigned int nsize;                  // размер итогового файла
    unsigned int alloc;                  // требуемый размер буфера, значение меньшее, чем BSIZE или равное ему
    unsigned int pose;                   // текщая позиция
    char *temp;                 // буфер, содержащий строку для смещения
    unsigned int offset = from + size;   // начало следующего блока данных
    char last[1];     // последний символ
    
    /* переходим в конец файла */
    fseek(file, 0, SEEK_END);
    
    /* определяем размер итогового файла */
    s = ftell(file);

    // отчистить весь файл
    if (size < 0)
        size = s;

    if (s < from + size)
        size = s - from;

    /* определяем, есть ли символ перевода строки в конце файла, если нет, то ставим */
    fgets(last, 1, file);
    if (last[0] == '\n')
        s++;
    
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

// Оболочка для функции strcmp, исключающая ошибки
int strmcmp(const char *str1, const char *str2)
{
    
    if (str1 && str2)
        return strcmp(str1, str2);
    
    /* если одно из значений или оба являются NULL вывести код ошибки */
    return 255;
}

// Функция для проверки существования файла
int fileExist(char *file)
{
    FILE *config;           // указатель на файл
    
    /* пытаемся открыть файл, если файл успешно открыт, то файл существет, иначе - файл не существует*/
    if ((config = fopen(file, "r")) == NULL)
            return 2;
    
    fclose(config);  // закрываем файл
    
    return 0;
}

// функция открывающая файл
FILE *fmopen(char *file, const char *flag, const char *errstr)
{
    FILE *temp = fopen(file, flag);

    // если файл не открыт
    if (!temp)
        // записываем ошибку в поток вывода отладочных сообщений
        perror(errstr);

    return temp;
}
