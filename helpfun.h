/** 
 *  @headerfile	helpfun.h
 */
#ifndef HELPFUN_H_
#define HELPFUN_H_

#define STRING_MAX_LINE 500     ///< ������������ ����� �������� �� ������

#define NUMBER "0123456789"     ///< ������� �� ������� ����� �������� �����
#define BSIZE 60000             ///< ������������ ������ ���������� ������

/**
 * @enum ���� ����������
 */
enum srt_type {
    T_INT = 1,           // ��� int
    T_CHAR = 2,          // ��� char
};

/* ������� ������ �������� � ���������� ������� ����� */
int fmwrite(FILE * file, const void * Data, int size);

/* ������� �������� �������� � ���������� ������� ����� */
int fmclean(FILE * file);

/* �������� ��� ������� strcmp, ����������� ������ (��������� ������) */
int strmcmp(const char *str1, const char *str2);

/* �������, ����������� ���� � ����������� ����������� ������ */
FILE *fmopen(const char *file, const char *flag, const char *errstr);

/* ������� ���������� ������ �������, ������������ ��������� ������� */
void qsort_proc(int *order, void *vmas, size_t length, int left,
                int right, int (*cmp) (const void *, int, int));

/* ������� �������� ��� qsort_proc */
void qsort_dmas(int *order, void *mas, unsigned int col_el, srt_type type);

/* ������� �������� ����������������� �����, ���������� ����� void ��������� */
int cmp_string(void *csmas, int pos1, int pos2);

/* ������� ��������� ������������� �������� */
int cmp_int(void *csmas, int pos1, int pos2);

/* �������, �������� ������� �������� ��������� �������������� ������� */
void swap(int *mas, int var1, int var2);

#endif /* CONF_H_ */

