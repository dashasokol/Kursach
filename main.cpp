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

/* ������� ��������� �� */
int openBD(KursBDClass *BDclass, string path);

/* ������� ��������� �� */
int sort(KursBDClass *BDclass, string field);

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
    KursBDClass mainBD, scnDB, sortBD, selectBD;


	// ��������������� ���� ������
	string mpath = DBPATH;
	mpath += MAINDBNAME;
	
	string spath = DBPATH;
	spath += SECONDDBNAME;
	
	// ������� ���� ������
	string selpath = DBPATH;
	selpath += SELECTBDNAME;
	
	string sopath = DBPATH; 
	sopath += SORTBDNAME;
	
	string so2path = DBPATH; 
	so2path += SORTSCNBDNAME;


	cout << "Exercise 1:\n\n\n";

	string sort_field = "FIRSTNAME";
	
    // ��������� ���� ������
    openBD(&mainBD, mpath);
    openBD(&scnDB, spath);

    // ��������� 1 ��
    sort(&mainBD, sort_field);
    mainBD.write_buffer(sopath);
    cout << "��������� � �����: " << sopath << "\n\n\n";
    mainBD.close();

    // ��������� 2 ��
    sort(&scnDB, sort_field);
    scnDB.write_buffer(so2path);
    cout << "��������� � �����: " << so2path << "\n\n\n";
    scnDB.close();
    scnDB.~KursBDClass();


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
 * @fn int openBD(KursBDClass *BDclass, string path)
 * @brief ������� ��������� ��
 * @param BDclass - ��������� �� ��������� ������
 * @param path - ������ ���� �� �����
 * @return ��� �������� ����������/������
 */
int openBD(KursBDClass *BDclass, string path)
{
    cout << "�������� ���� ������: " << path << "\n";
    
    /* ��������� ���� */
    if (BDclass->open(path) == END_OPEN_FAIL)
    {
        /* �� ������� ������� ���� */
        cout << "[������]\n";
        return 1;
    }

    /* ���� ������� ������� */
    cout << "[�����]\n\n";
    return 0;
}

/**
 * @fn int sort(KursBDClass *BDclass, string field)
 * @brief ������� ��������� ��
 * @param BDclass - ��������� �� ��������� ������
 * @param field - ����, �� �������� ���������� ����������
 * @return ��� �������� ����������/������
 */
int sort(KursBDClass *BDclass, string field)
{
    cout << "C��������� �� ���� " << field << "\n";

    /* ��������� �� */
    if (BDclass->sort(field) == END_NOT_FOUND)
    {
        /* ���������� ���� �� ���������� */
        cout << "[������]\n";
        return 1;
    }

    /* �� ������������� */
    cout << "[�����]\n";
    return 0;
}


