/**
 *  @file	helpfun.c
 *  @brief	��������������� �������, ������� ����� � ����������� ��������, �� ��� ������� �� ���������� ��������� � ������� �����
 * 
 */
#include <iostream>     // printf, scanf, NULL
#include <cstring>      // strstr, str[n]cat, strlen
#include <cstdlib>      // malloc, free, rand
#include <unistd.h>     // ftruncate
#include "../include/helpfun.h"

/**
 * @fn int fmwrite(FILE * file, void * Data, int size)
 * @brief ������� ������ �������� � ���������� ������� �����
 * @param file - ��������� �� ����
 * @param Data - ��������� �� ���� ������
 * @param size - ������ ����� ������
 * @param from - �������� ������������ ������ �����
 * @return ��� ������ / �������� ����������
 */
int fmwrite(FILE * file, const void * Data, int size)
{
    /* ��������� � ����� ������� */
    fseek(file, 0, SEEK_SET);
    
    /* ���������� ������ � ���� � �������� ���������� �� ������� ������ �� ����� */
    if (fwrite(Data, size, 1, file) == 1)
        return 0;
    else
        return 1;
}

/**
 * @fn int fmclean(FILE * file, int size, int from)
 * @brief ������� �������� �������� � ���������� ������� �����
 * @param file - ��������� �� ����
 * @param size - ������ ����� ������ (���� -1 - ��������� ���� ����)
 * @param from - �������� ������������ ������ �����
 * @return ��� ������ / �������� ����������
 */
int fmclean(FILE * file)
{
    fseek(file, 0, SEEK_SET);
    /* �������� ����� ����� � �������� ���������� �� ������� ������ �� ����� */
    return ftruncate(fileno(file), ftell(file));
}

/**
 * @fn int strmcmp(const char *str1, const char *str2)
 * @brief �������� ��� ������� strcmp, ����������� ������ (��������� ������)
 * @param str1 - ������ ������
 * @param str2 - ������ ������
 * @return -1 - ������ ������ ������, 0 - ������ ���������,
 *          1 - ������ ������ ������, 255 - ������
 */
int strmcmp(const char *str1, const char *str2)
{
    int ret;    // ��� ��������

    /* ���� ���� �������� ����������, �������� �� */
    if (str1 && str2)
    {
        ret = strcmp(str1, str2);
        return ret > 0 ? 1 : ret < 0 ? -1 : 0;
    }

    /* ���� ���� �� �������� ��� ��� �������� NULL ������� ��� ������ */
    return 255;
}

/**
 * @fn FILE *fmopen(char *file, const char *flag, const char *errstr)
 * @brief ������� ����������� ���� � ����������� ����������� ������
 * @param file - ������ ��� �����
 * @param flag - ����� �������� �����
 * @param errstr - ��������� ��� ������, � ������ ���������� �������� �����
 * @return ���������� �����
 *
 * ����������: ����� �� ������������ fflush (����� �������)
 */
FILE *fmopen(const char *file, const char *flag, const char *errstr)
{
    FILE *temp = fopen(file, flag);     // ���������� �����

    /* ������� ����������� ������ � ���� */
    setvbuf(temp, NULL, _IONBF, 0);

    // ���� ���� �� ������
    if (!temp)
        // ���������� ������ � ����� ������ ���������� ���������
        perror(errstr);

    return temp;
}

/**
 * @fn void qsort_proc(int *order, void *vmas, int left, int right,
                       int (*cmp) (void *, int, int))
 * @brief ������� ���������� ������ �������, ������������ ��������� �������
 * @param order - �������, ������� ����� �������������
 * @param vmas - ������ �������� �� ���������, �� �������� ����� ������������� order
 * @param left - ��������� ������ ����������� ���������
 * @param right - �������� ������ ����������� ���������
 * @param cmp - ���������� ��������� ��������
 *
 * ����������: ��� ������, ����� ����� ����������� ��������� �� �������
 */
void qsort_proc(int *order, void *vmas, int left, int right,
                int (*cmp) (void *, int, int))
{
    int i = 0;          // �������
    int last = 0;       // ��������� �������

    /* ���� ��� ������������� ��� � ������� ���� ������� */
    if (left >= right)
        return;

    /* ������ ������� ������� ����� � ������� �������� */
    swap(order, left, (left + right)/2);
    last = left;

    /* ��������� ������ ������������ ������� �������� */
    for (i = left+1; i <= right; i++)
    {
        if (cmp(vmas, order[i], order[left]) < 0)
            swap(order, ++last, i);
    }

    /* ������ ������� ������� ����� � ��������� ���������������*/
    swap(order, left, last);

    /* ��������� ����� ����� ������� */
    qsort_proc(order, vmas, left, last-1, cmp);

    /* ��������� ������ ����� ������� */
    qsort_proc(order, vmas, last+1, right, cmp);
}

/**
 * @fn int cmp_string(void *csmas, int pos1, int pos2)
 * @brief ������� �������� ����������������� �����, ���������� ����� void ���������
 * @param csmas - ��������� �� ������ ������
 * @param pos1 - ������� ������ ������
 * @param pos2 - ������� ������ ������
 * @return -1 - ������ ������ ������, 0 - ������ ���������,
 *          1 - ������ ������ ������, 255 - ������
 */
int cmp_string(void *csmas, int pos1, int pos2)
{
    char **pcsmas = (char **)csmas; // ��������� �� ������ ���� char, ���������
                                    // ����� void

    /* ���������� ������ */
    return strmcmp(pcsmas[pos1], pcsmas[pos2]);
}

/**
 * @fn cmp_int(void *csmas, int pos1, int pos2)
 * @brief ������� ��������� ������������� ��������
 * @param csmas - ��������� �� ������ ������
 * @param pos1 - ������� ������ ������
 * @param pos2 - ������� ������ ������
 * @return -1 - ������ ���������� ������, 0 - ���������� ���������,
 *          1 - ������ ���������� ������
 */
int cmp_int(void *csmas, int pos1, int pos2)
{
    int *pcsmas = (int *)csmas; // ��������� �� ������ ���� int, ���������
                                // ����� void

    /* ���������� �������� � ���������� ��� ���������� ������� */
    return pcsmas[pos1] < pcsmas[pos2] ? -1 : \
           pcsmas[pos1] > pcsmas[pos2] ? 1 : 0;
}

/**
 * @fn void qsort_dmas(int *order, void *mas, unsigned int col_el, srt_type type)
 * @brief ������� �������� ��� qsort_proc
 * @param order - �������, ������� ����� �������������
 * @param mas - ������ �������� �� ���������, �� �������� ����� ������������� order
 * @param col_el - ���������� ���������, �� ������� ����� ����������� ����������
 * @param type - ��� ������ � ������� mas
 */
void qsort_dmas(int *order, void *mas, unsigned int col_el, srt_type type)
{
    /* ���������� � ������ ���� ����������� ������ */
    switch (type)
    {
    case T_INT:
        /* ��������� ������ ���� int */
        qsort_proc(order, mas, 0, col_el, cmp_int);
        break;
    case T_CHAR:
        /* ��������� ������ ���� char */
        qsort_proc(order, mas, 0, col_el, cmp_string);
        break;
    default:
        /* ��� ����� ������� */
        fprintf(stderr, "Wrong variable type\n");
        return;
    }
}

/**
 * @fn void swap(int *mas, int var1, int var2)
 * @brief �������, �������� ������� �������� ��������� �������������� �������
 * @param mas - ������ ��������
 * @param var1 - ������ ����������
 * @param var2 - ������ ����������
 */
void swap(int *mas, int var1, int var2)
{
    int tmp;    // ��������� ����������

    /* ������ ������� �������� ��������� */
    tmp = mas[var1];
    mas[var1] = mas[var2];
    mas[var2] = tmp;
}

