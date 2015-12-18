#include <iostream>         // printf, cout, cin, fprintf
#include <cstring>          // strstr, str[n]cat, strlen
#include <stdlib.h>         // system
#include "kursbdclass.h"    // ������������ ���� ������ ������

/* ���������� ����������� ������������ ���� */
using namespace std;

#define DBPATH "C:\\1\\"            // ���� �� ������� �����
#define MAINDBNAME "KurBD"          // �������� ����
#define SECONDDBNAME "ScnBD"        // ������ ����
#define SELECTBDNAME "SelectBD"     // ���� ��� ������ ������� select
#define SORTBDNAME "SortBD"         // ���� ��� ������ ���������� 1
#define SORTSCNBDNAME "SortscnBD"   // ���� ��� ������ ���������� 2
#define FILENAMELEN 255             // ������������ ������ ����� �����

/* ������� ���������� ������ �������� ����� */
void get_ffile_name(char *fullname, char *name);

/* ������� ��������� �� */
int openBD(KursBDClass *BDclass, char *path);

/* ������� ��������� �� */
int sort(KursBDClass *BDclass, char *field);

/**
 * @fn int main()
 * @brief ������� �������
 * @return ��� ��������
 */
int main()
{
    /* ����� ��������� �� cp1251 */
	system("chcp 1251 > nul");
	
    /* ������� ��������� ������ */ 
//    KursBDClass mainBD, scnDB, sortBD, selectBD;

//    // ��������������� ���� ������
//    char mpath[FILENAMELEN];
//    char spath[FILENAMELEN];

//    // ������� ���� ������
//    char sopath[FILENAMELEN];
//    char so2path[FILENAMELEN];
//    char selpath[FILENAMELEN];

//    // ������ ���� ��� ������ ��
//    get_ffile_name(mpath, (char *) MAINDBNAME);
//    get_ffile_name(spath, (char *) SECONDDBNAME);
//    get_ffile_name(selpath, (char *) SELECTBDNAME);
//    get_ffile_name(sopath, (char *) SORTBDNAME);
//    get_ffile_name(so2path, (char *) SORTSCNBDNAME);


//    printf("Exercise 1:\n\n\n");

//    char *sort_field = (char *) "fname";
//    // ��������� ���� ������
//    openBD(&mainBD, mpath);
//    openBD(&scnDB, spath);

//    // ��������� 1 ��
//    sort(&mainBD, sort_field);
//    mainBD.write_buffer(sopath);
//    printf("Result in file: %s\n\n\n", sopath);
//    mainBD.close();

//    // ��������� 2 ��
//    sort(&scnDB, sort_field);
//    scnDB.write_buffer(so2path);
//    printf("Result in file: %s\n\n\n", so2path);
//    scnDB.close();


//    // ��������� ��
//    openBD(&sortBD, sopath);
//    printf("Merges on the field: \"%s\"\n\n", sort_field);
//    sortBD.merge(so2path, sort_field);
//    sortBD.write_buffer();
//    printf("Result in file: %s\n\n\n", sopath);
//    sortBD.close();


//    printf("\nExercise 2:\n\n\n");

//    openBD(&selectBD, mpath);

//    char *selyear = (char *) "years";
//    char *selfname = (char *) "fname";
//    char *selpos = (char *) "position";
//    int year = 30;
//    char *fname = (char *) "�����";
//    char *pos = (char *) "�������������"
//    // ��������� ������ �������
//    printf("Select on the field \"%s\" with the value %d\n", selyear, year);
//    selectBD.select(selyear, year);
//    selectBD.write_buffer(selpath);

//    // ��������� ���� ���� ������
//    selectBD.close();


//    openBD(&selectBD, selpath);

//    // ��������� �������
//    printf("Select on the field \"%s\" with the value %s\n", selfname, fname);
//    selectBD.select(selfname, fname);
//    selectBD.write_buffer();

//    selectBD.close();

//    openBD(&selectBD, selpath);
//    // ��������� �������
//    printf("Select on the field \"%s\" with the value %s\n", selpos, pos);
//    selectBD.select(selpos, pos);
//    selectBD.write_buffer();

//    printf("Result in the file: %s\n\n", selpath);

//    // ��������� ���� ���� ������
//    selectBD.close();

	/* ��������� ��������� */
	system("pause"); 

    return 0;
}


/**
 * @fn void get_ffile_name(char *fullname, char *name)
 * @brief ������� ���������� ������ �������� �����
 * @param fullname - ������ ������
 * @param name  - ��� �����
 */
void get_ffile_name(char *fullname, char *name)
{
//    /* �������� ���� �� ������� ����� */
//    strcpy(fullname, (char *) DBPATH);

//    /* �������� ��� ����� */
//    strcat(fullname, name);
}

/**
 * @fn int openBD(KursBDClass *BDclass, char *path)
 * @brief ������� ��������� ��
 * @param BDclass - ��������� �� ��������� ������
 * @param path - ������ ���� �� �����
 * @return ��� �������� ����������/������
 */
int openBD(KursBDClass *BDclass, char *path)
{
//    printf("Open DB: %s\n", path);
//    /* ��������� ���� */
//    if (BDclass->open(path) == END_OPEN_FAIL)
//    {
//        /* �� ������� ������� ���� */
//        printf("[Fail]\n");
//        return 1;
//    }

//    /* ���� ������� ������� */
//    printf("[OK]\n\n");
    return 0;
}

/**
 * @fn int sort(KursBDClass *BDclass, char *field)
 * @brief ������� ��������� ��
 * @param BDclass - ��������� �� ��������� ������
 * @param field - ����, �� �������� ���������� ����������
 * @return ��� �������� ����������/������
 */
int sort(KursBDClass *BDclass, char *field)
{
    printf("C��������� �� ���� %s\"\n", field);

//    /* ��������� �� */
//    if (BDclass->sort(field) == END_NOT_FOUND)
//    {
//        /* ���������� ���� �� ���������� */
//        printf("[Fail]\n");
//        return 1;
//    }

//    /* �� ������������� */
//    printf("[OK]\n");
    return 0;
}


