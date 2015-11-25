/**
 *  @file	helpfun.c
 *  @brief	Вспомогательные функции, которые нужны в большинстве программ, но для которых не существует библиотек в базовом языке
 * 
 */

#include <stdio.h>                        // printf, scanf, NULL 
#include <string.h>                       // strstr, str[n]cat, strlen
#include <stdlib.h>                       // malloc, free, rand
#include "helpfun.h"    						// Заголовочный файл данного модуля

// Функция вычиления первого вхождения подстроки в строку
int indexOf(char *str, char *substring, enum vector_type vector)
{
    char string[SSIZE]; // копия строки для поиска
    char *rem_string;	// указатель на обрезанную строку
    int i; 		// счётчик

    strcpy(string, str);
    if (vector == LEFT) {
        rem_string = strstr(string, substring); // находим первое вхождение

        /* если найден разделитель высчитать его местоположение */
        if (rem_string)
            return strlen(string) - strlen(rem_string);
    }
    else if (vector == RIGHT)
    {
        /* находим последнее вхождение */
        for (i=strlen(string)-1; i>=0; i--)
        {
            rem_string = strstr(&string[i], substring);
            if (rem_string)
                return strlen(string) - strlen(rem_string);
        }
    }
    return -1;
}

///**
// * @fn int charcut(char *out, char *str, char *substring)
// * @brief Функция удаления всех вхождений подстроки в строку
// * @param out - Строка вывода
// * @param str - Строка для обработки
// * @param substring - Искомая строка
// * @return 0 - удаление произведено / 1 - искомая строка не найдена
// *
// * Пример:\n
// * Строка line: "12121212"\n
// * Подстрока subline: "1"\n
// * Строка, в которую выводится данные endline: ""\n
// * @code
// * charcut(endline, line, subline);
// * @endcode
// * endline: "2222"
// */
//int charcut(char *out, char *str, char *substring)
//{
//    int i,                                            // счётчик
//		  j;                                           // счётчик
//    char string[STRING_MAX_LINE];                       // строка, которая будет преобразовываться
//    int found = 0;                                    // символ найден
    
//    /* отчищаем строку для изменения */
//    memset(string, 0, sizeof(string));

//    /* добавляем посимвольно строку, исключая пробелы */
//    for (i=j=0;i<strlen(str);i++)
//    {
//        if (strspn(&str[i], substring) == 0)
//            string[j++] = str[i];
//        else
//            found = 1;
//    }
    
//    /* вычисляем строку вывода */
//    memset(out, 0, sizeof(out));
//    strcpy(out, string);

//    /* вставляем нуль-терминатор */
//    out[strlen(string)+1] = '\0';

//    /* выводим обработанную строку */
//    return found;
//}

///**
// * @fn int spacecut(char *out, char *str, enum vector_type vector)
// * @brief Функция удаляния пробелов из строки
// * @param out - Строка вывода
// * @param str - Строка для обработки
// * @param vector - Сторона с которой производится поиск символа
// * @return 0 - удаление произведено / 1 - искомая строка не найдена
// *
// * Пример:\n
// * Строка line: " 1 1 1 1 "\n
// * @code
// * spacecut(endline, line, LEFT); // endline: "1 1 1 1 "
// * spacecut(endline, line, RIGHT); // endline: " 1 1 1 1"
// * spacecut(endline, line, RIGHTLEFT); // endline: "1 1 1 1"
// * spacecut(endline, line, ALL); // endline: "1111"
// * @endcode
// */
//int spacecut(char *out, char *str, enum vector_type vector)
//{
//    char string[STRING_MAX_LINE];                       // строка, которая будет преобразовываться
//    char cut_string[strlen(str)];                       // строка, которая будет вырезана из string
//    int i, 															 // счётчик
//		  j;                                           // счётчик

//    if (strspn(str, " ") != strlen(str))
//    {
//        /* обнуляем массив со строкой удаления */
//        memset(cut_string, 0, sizeof(cut_string));
//        strcat(cut_string, " ");

//        /* перемещаем данные в строку для изменения */
//        if (vector != ALL)
//            strcpy(string, str);

//        /* удаляем начальные пробелы */
//        if (vector == LEFT || vector == RIGHTLEFT)
//        {
//            /* заполняем строку для удаления пока в начале есть пробелы */
//            while (indexOf(string, cut_string, LEFT) == 0)
//                strcat(cut_string, " ");

//            strcpy(string, &string[strlen(cut_string)-1]);

//            /* обнуляем массив со строкой удаления */
//            memset(cut_string, 0, sizeof(cut_string));

//            /* устанавливаем начальное значение строки, для вырезания */
//            strcat(cut_string, " ");

//            /* вычисляем строку вывода */
//            if (vector == LEFT)
//                memset(out, 0, sizeof(out));
//                strcpy(out, string);
//        }

//        /* удаляем пробелы из конца строки */
//        if (vector == RIGHT || vector == RIGHTLEFT)
//        {
//            while (indexOf(string, cut_string, RIGHT) == strlen(string)-strlen(cut_string))
//                strcat(cut_string, " ");

//            /* вычисляем строку вывода */
//            memset(out, 0, sizeof(out));
//            strncpy(out, string, strlen(string)-strlen(cut_string)+1);
//        }

//        /* удаляем все пробелы из строки */

//        if (vector == ALL)
//        {
//            /* отчищаем строку для изменения */
//            memset(string, 0, sizeof(string));

//            /* добавляем посимвольно строку, исключая пробелы */
//            for (i=j=0;i<strlen(str);i++)
//            {
//                if (str[i] != ' ')
//                    string[j++] = str[i];
//            }
//            /* вычисляем строку вывода */
//            memset(out, 0, sizeof(out));
//            strcpy(out, string);
//        }

//        /* вставляем нуль-терминатор */
//        out[strlen(string)-strlen(cut_string)+1] = '\0';
//    }
//    else
//        return 1;
            
//    /* выводим обработанную строку */
//    return 0;
//}

///**
// * @fn int col_match(char *string, char *substring)
// * @brief Функция посчёта вхождений подстроки в строку
// * @param string
// * @param substring
// * @return Количество вхождений
// */
//int col_match(char *string, char *substring)
//{
//	int index = -1; // новое положение коретки
//	int ret = 0;    // счтётчик

//	/* обрезаем строку при нахождении подстроки в троке */
//	while (indexOf(&string[index+1], substring, LEFT) >= 0)
//		{
//			index=index+indexOf(&string[index+1], substring, LEFT) + 1;
//			ret++;
//		}

//	return ret;
//}

///**
// * @fn int fmwrite(FILE * file, void * Data, int size, int from)
// * @brief Функция записи сиволов в конкретную позицию файла
// * @param file - Указатель на файл
// * @param Data - Указатель на блок данных
// * @param size - Размер блока данных
// * @param from - Смещение относительно начала файла
// * @return Код ошибки / удачного завершения
// */
//int fmwrite(FILE * file, void * Data, int size, int from)
//{
//    int s;          // размер файла
//    int nsize;      // размер итогового файла
//    int alloc;      // требуемый размер буфера, значение меньшее, чем BSIZE или равное ему
//    int pose;       // текщая позиция
//    int read;       // размер смещаемой информации
//    char * temp;    // буфер, содержащий строку для смещения
   
//    /* переходим в конец файла */
//	fseek(file, 0, SEEK_END);
     
//    /* определяем размер итогового файла */
//	s = ftell(file);
//    nsize = s + size;
    
//    /* переходим в место вставки */
//	fseek(file, from, SEEK_SET);

//	/* определяем требуемый размер буфера и выделяем память */
//	alloc = BSIZE > s ? s-from : BSIZE;
//	temp = (char*) malloc(alloc);
	
//	pose = nsize;
    
//	while (pose > from + size)
//	{
//        /* определяем, сколько информации необходимо сместить */
//		read = pose-size <= from ? size : alloc;
        
//        /* считываем в буфер строку для смещения */
//		fseek(file,pose - size - read,SEEK_SET);
//		fread((void*)temp,read,1,file);

//        /* вставляем данные из буфера в файл, начиная с символо от которого они должны идти */
//		fseek(file, pose - read, SEEK_SET);
//		fwrite((void*)temp, read, 1, file);
		
//        /* переходим на позицию, для следующего смещения */
//		pose -= read;
//	}
    
//    /* переходим на позицию записи */
//	fseek(file, from, SEEK_SET);
    
//    /* записываем данные в файл и посылаем информацию от функции записи на выход */
//	if (fwrite(Data, size, 1, file) == 1)
//        return 0;
//    else
//        return 1;
//}

///**
// * @fn int fmclean(FILE * file, int size, int from)
// * @brief Функция удаления сиволов с конкретной позиции файла
// * @param file - Указатель на файл
// * @param size - Размер блока данных
// * @param from - Смещение относительно начала файла
// * @return Код ошибки / удачного завершения
// */
//int fmclean(FILE * file, int size, int from)
//{
//    int s;                      // размер файла
//    int nsize;                  // размер итогового файла
//    int alloc;                  // требуемый размер буфера, значение меньшее, чем BSIZE или равное ему
//    int pose;                   // текщая позиция
//    char *temp;                 // буфер, содержащий строку для смещения
//    int offset = from + size;   // начало следующего блока данных
//    int i;                      // счётчик
//    char last[1];     // последний символ
    
//    /* переходим в конец файла */
//    fseek(file, 0, SEEK_END);
    
//    /* определяем размер итогового файла */
//    s = ftell(file);
    
//    if (s < from + size)
//        size = s - from;

//    /* определяем, есть ли символ перевода строки в конце файла, если нет, то ставим */
//    fgets(last, 1, file);
//    if (last[0] == '\n')
//        s++;
    
//    nsize = s - size;

//    /* переходим в место вставки */
//    fseek(file, offset, SEEK_SET);

//    /* определяем требуемый размер буфера и выделяем память */
//    alloc = BSIZE > s ? s-offset : BSIZE;
//    temp = (char*) malloc(alloc);

//    /* перемещаем блоки данных, замещая удаленные данные */
//    pose = from;
//    if (pose != nsize)
//        while (pose + alloc <= nsize)
//        {
//            /* считываем в буфер строку для смещения */
//            fseek(file,pose + size,SEEK_SET);
//            fread((void*)temp, alloc, 1, file);

//            /* вставляем данные из буфера в файл, начиная с символо от которого они должны идти */
//            fseek(file, pose, SEEK_SET);
//            fwrite((void*)temp, alloc, 1, file);

//            /* переходим на позицию, для следующего смещения */
//            pose += alloc;
//        }
    
//    /* переходим на позицию записи */
//    fseek(file, from, SEEK_SET);

//    /* обрезаем конец файла и посылаем информацию от функции записи на выход */
//    return ftruncate(fileno(file), nsize);
//}

///**
// * @fn char *inttostr(int number)
// * @brief Функция перевода целого числа в строку
// * @param number
// * @return Строкове представление числа
// */
//char *inttostr(int number)
//{
//    int i;          // счётчик
//    int tmpnum;     // изменяемая переменная
//    char *rv;       // строка вывода

//    i = 0;
//    tmpnum = number;
    
//    /* делим каждый раз на 10, определяя тем самым количество разрядов */
//    do {
//        ++i;
//        tmpnum /= 10;
//    } while (tmpnum > 0);

//    /* выделяем память согласно количеству разрядов увеличенному на 1 */
//    rv = malloc(i + 1);
//    rv[i] = 0; // конец строки
    
//    tmpnum = number;
    
//    /* первеводи поразрядно число в строку */
//    do {
//        --i;
//        rv[i] = '0' + tmpnum % 10; // получаем остаток от деления
//        tmpnum /= 10;
//    } while (tmpnum > 0);
//    return rv;
//}

///**
// * @fn int strmcmp(__const char *__str1, __const char *__str2)
// * @brief Оболочка для функции strcmp, исключающая ошибки
// * @param __str1 - Первая строка
// * @param __str2 - Вторая строка
// * @return Код ошибки / удачного завершения
// */
//int strmcmp(__const char *__str1, __const char *__str2)
//{
    
//    if (__str1 && __str2)
//        return strcmp(__str1, __str2);
    
//    /* если одно из значений или оба являются NULL вывести код ошибки */
//    return 255;
//}

///**
// * @fn int fileExist(char *file)
// * @brief Функция для проверки существования файла
// * @param file - Полное имя файла
// * @return 0 - файл найден / 2 - файл не найден
// */
//int fileExist(char *file)
//{
//    FILE *config;           // указатель на файл
    
//    /* пытаемся открыть файл, если файл успешно открыт, то файл существет, иначе - файл не существует*/
//    if ((config = fopen(file, "r")) == NULL)
//            return 2;
    
//    fclose(config);  // закрываем файл
    
//    return 0;
//}
