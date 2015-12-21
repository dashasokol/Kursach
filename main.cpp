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
#define SORTBDNAME "SortBD"         // ���� ��� ������ ����������

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
    unsigned int i; // �������

    /* ����� ��������� �� cp1251 */
	system("chcp 1251 > nul");
	
    /* ������� ��������� ������ */ 
    KursBDClass mainBD, scnDB, selBD;

    /* ��������������� ���� ������ */
	string mpath = DBPATH;
	mpath += MAINDBNAME;
	
	string spath = DBPATH;
	spath += SECONDDBNAME;
	
    /* ������� ���� ������ */
	string selpath = DBPATH;
	selpath += SELECTBDNAME;
	
	string sopath = DBPATH; 
	sopath += SORTBDNAME;
	
    cout << "������� 1:\n\n\n";

    string sort_field = "FIRSTNAME"; // ���� ��� ����������
	
    /* ��������� ���� ������ */
    openBD(&mainBD, mpath);
    openBD(&scnDB, spath);

    /* ��������� � ��������� ���� ������ */
    cout << "���������� ��� ������\n";
    for (i = 0; i < scnDB.get_length(); i++)
        mainBD.add(scnDB.get_entry(i));

    cout << "���������� �� ����" << sort_field << "\n";
    mainBD.sort(sort_field);
    mainBD.write_buffer(sopath);


    cout << "��������� � �����: " << sopath << "\n\n\n";
    scnDB.close();
    scnDB.~KursBDClass();
    mainBD.close();
    mainBD.~KursBDClass();


    cout << "������� 2:\n\n\n";

    openBD(&selBD, mpath);

    string field1 = "YEARS";
    string value1 = "30";

    string field2 = "FIRSTNAME";
    string value2 = "Maria";

    string field3 = "POSITION";
    string value3 = "Admin";


    /* ��������� ������ ������� */
    cout << "���������� �� ����: " << field1 << "; �� ���������: " << value1 << "\n";
    selBD.select(field1, value1);
    cout << "���������� �� ����: " << field2 << "; �� ���������: " << value2 << "\n";
    selBD.next_select(field2, value2);
    cout << "���������� �� ����: " << field3 << "; �� ���������: " << value3 << "\n";
    selBD.next_select(field3, value3);
    selBD.write_buffer(selpath);

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


