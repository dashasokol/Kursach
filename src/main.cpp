#include <iostream>         // printf, cout, cin, fprintf
#include <cstring>          // strstr, str[n]cat, strlen
#include <stdlib.h>         // system
#include <clocale>
#include "../include/kursbdclass.h"    // ������������ ���� ������ ������

/* ���������� ����������� ������������ ���� */
using namespace std;

#define DBPATH "G:\\kyrsovaya\\base\\"            // ���� �� ������� �����
#define MAINDBNAME "KurBD"          // �������� ����
#define SECONDDBNAME "ScnBD"        // ������ ����
#define SELECTBDNAME "SelectBD"     // ���� ��� ������ ������� select
#define SORTBDNAME "SortBD"         // ���� ��� ������ ����������

/* ������� ��������� �� */
int openBD(KursBDClass *BDclass, string path);

/* ������� ��������� �� */
int sort(KursBDClass *BDclass, string field);

/* ����� ���� */
void print_menu();

/* ����� �: ���� */
void a();

/* ����� B: ���� */
void b();

/* ����� C: ���� */
void c();

/* ����� D: ���� */
void d();

/* ����� E: ���� */
void e();

/* ������ 1 */
void ex1();

/* ������ 2 */
void ex2();

/**
 * @fn int main()
 * @brief ������� �������
 * @return ��� ��������
 */
int main()
{
    char ch;

    /* ����� ��������� �� cp1251 */
    setlocale( LC_ALL, "Russian" );

    print_menu();

    while((ch=getchar()) != 'q' && ch != 'Q' && ch != EOF)
    {
        switch(ch)
        {
        case 'a':
        case 'A':
            a();
            break;
        case 'b':
        case 'B':
            b();
            break;
        case 'c':
        case 'C':
            c();
            break;
        case 'd':
        case 'D':
            d();
            break;
        case 'e':
        case 'E':
            e();
            break;
        case '1':
            ex1();
            break;
        case '2':
            ex2();
            break;
        case '\n':
           	break;
        default:
            cout << "  !��� ������ ������\n";
            system("pause");
            break;
        }

        print_menu();
    }

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

/**
 * @fn void print_menu()
 * @brief ����� ����
 */
void print_menu()
{
    /* �������� ����� */
    system("cls");
    cout << "���� ���������:\n\n�����:\na) ��������� ���� a \nb) ��������� ���� b\n" \
            "c) ��������� ���� c \nd) ��������� ���� d \ne) ��������� ���� e \n" \
            "\n�������\n1) ��������� ���� ������� 1 \n2) ��������� ���� ������� 2\n\n" \
            "������ ��������:\nq) �����\n\n\n��� �����: ";
}


/**
 * @fn void a()
 * @brief ������������ ������ �
 */
 void a()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;
    mpath += "a";

    /* ��������� �� */
    openBD(&mainBD, mpath);
	
	string new_entry = "124;������;�������;46;���";
	cout << "���������� ������: " << new_entry << "\n";

    /* ������� ������ � ����� ����� */
    mainBD.add(new_entry);

    /* ���������� ���� */
    mainBD.write_buffer();

    cout << "��������� � �����: " << mpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
    
    /* ������������� ��������� */
    system("pause");
}

/**
 * @fn void b()
 * @brief ������������ ������ b
 */
void b()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;
    mpath += "b";

    /* �������� */
    string field = "YEARS";
    string value = "30";

    /* ��������� �� */
    openBD(&mainBD, mpath);

	cout << "�������� ���� ������� ���: " << field << " = " << value << "\n";

    /* ������� ������ */
    mainBD.del(field, value);

    /* ���������� ���� */
    mainBD.write_buffer();

    cout << "\n��������� � �����: " << mpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
    
    /* ������������� ��������� */
    system("pause");
}

/**
 * @fn void c()
 * @brief ������������ ������ c
 */
void c()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* ������� ���� ������ */
    string selpath = DBPATH;
    selpath += SELECTBDNAME;

    /* �������� */
    string field = "FIRSTNAME";

    /* ��������� �� */
    openBD(&mainBD, mpath);
    
    cout << "���������� �� ����: " << field << "\n";

    /* ��������� */
    mainBD.sort(field);

    /* ���������� ���� */
    mainBD.write_buffer(selpath);

    cout << "\n��������� � �����: " << selpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
    
    /* ������������� ��������� */
    system("pause");
}

/**
 * @fn void d()
 * @brief ������������ ������ d
 */
void d()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* ������� ���� ������ */
    string selpath = DBPATH;
    selpath += SELECTBDNAME;

    /* �������� */
    string field = "FIRSTNAME";
    string value = "�����";

    /* ��������� �� */
    openBD(&mainBD, mpath);
    
    cout << "������� ���� ������� ���: " << field << " = " << value << "\n";

    /* ������� ������ */
    mainBD.select(field, value);

    /* ���������� ���� */
    mainBD.write_buffer(selpath);

    cout << "\n��������� � �����: " << selpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
    
    /* ������������� ��������� */
    system("pause");
}

/**
 * @fn void e()
 * @brief ������������ ������ e
 */
void e()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;
    mpath += "e";

    /* ��������� �� */
    openBD(&mainBD, mpath);

    /* ��������� �� �� ������� */
    /* �������� */
    string field = "FIRSTNAME";
    cout << "���������� �� ����: " << field << "\n";
    mainBD.sort(field);

    string new_entry = "124;������;�����;46;�������";
    cout << "���������� ������: " << new_entry << "\n";

    /* ������� ������ � ����� ����� */
    mainBD.insert(new_entry);

    /* ���������� ���� */
    mainBD.write_buffer();

    cout << "\n��������� � �����: " << mpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
    
    /* ������������� ��������� */
    system("pause");
}

/**
 * @fn void ex1()
 * @brief ������������ ������ 1
 */
void ex1()
{
    unsigned int i; // �������

    /* �������� ����� */
    system("cls");

    /* ������� ��������� ������ */
    KursBDClass mainBD, scnDB;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    string spath = DBPATH;
    spath += SECONDDBNAME;

    /* ������� ���� ������ */
    string sopath = DBPATH;
    sopath += SORTBDNAME;

    cout << "������� 1:\n\n\n";

    string sort_field = "FIRSTNAME"; // ���� ��� ����������

    /* ��������� ���� ������ */
    openBD(&mainBD, mpath);
    openBD(&scnDB, spath);

    /* ��������� ���� ������ (��������� ����, �� �������, ������ ���� �������������) */
    mainBD.sort(sort_field);

    /* ��������� � ��������� ���� ������ */
    cout << "���������� ��� ������\n";
    for (i = 0; i < scnDB.get_length(); i++)
        mainBD.add(scnDB.get_entry(i));

    sort_field = mainBD.get_sort_field();
    cout << "���������� �� ����: " << sort_field << "\n";
    mainBD.sort(mainBD.get_sort_field());

    /* ���������� ������ � ���� */
    mainBD.write_buffer(sopath);

    cout << "\n��������� � �����: " << sopath << "\n\n\n";
    scnDB.close();
    mainBD.close();

    /* ��������� ��������� */
    system("pause");
}

/**
 * @fn void ex2()
 * @brief ������������ ������ 2
 */
void ex2()
{

    /* �������� ����� */
    system("cls");

    /* ������� ��������� ������ */
    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* ������� ���� ������ */
    string selpath = DBPATH;
    selpath += SELECTBDNAME;

    cout << "������� 2:\n\n\n";

    openBD(&mainBD, mpath);

    string field1 = "YEARS";
    string value1 = "30";

    string field2 = "FIRSTNAME";
    string value2 = "�����";

    string field3 = "POSITION";
    string value3 = "�������������";

    /* ��������� ������ ������� */
    cout << "���������� �� ����: " << field1 << "; �� ���������: " << value1 << "\n";
    /* ��������� ������ �������*/
    mainBD.select(field1, value1);
    cout << "���������� �� ����: " << field2 << "; �� ���������: " << value2 << "\n";
    /* ��������� ������ ������� */
    mainBD.next_select(field2, value2);
    cout << "���������� �� ����: " << field3 << "; �� ���������: " << value3 << "\n";
    mainBD.next_select(field3, value3);
    
    cout << "\n��������� � �����: " << selpath << "\n\n\n";

    /* ���������� ������ � ���� */
    mainBD.write_buffer(selpath);
    mainBD.close();

    /* ��������� ��������� */
    system("pause");
}
