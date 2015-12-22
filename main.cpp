#include <iostream>         // printf, cout, cin, fprintf
#include <cstring>          // strstr, str[n]cat, strlen
#include <stdlib.h>         // system
#include <clocale>
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
            b();
            break;
        case '1':
            ex1();
        case '2':
            ex2();
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

void print_menu()
{
    /* �������� ����� */
    system("cls");
    cout << "���� ���������:\n\n�����:\na) ��������� ���� a \nb) ��������� ���� b\n" \
            "c) ��������� ���� c \nd) ��������� ���� d \ne) ��������� ���� e \n" \
            "\n�������\n1) ��������� ���� ������� 1 \n2) ��������� ���� ������� 2\n\n" \
            "������ ��������:\nq) �����\n\n\n��� �����: ";
}

void a()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* ��������� �� */
    openBD(&mainBD, mpath);

    /* ������� ������ � ����� ����� */
    mainBD.add("124;Galina;Zuyeva;46;Cleaner");

    /* ���������� ���� */
    mainBD.write_buffer();

    cout << "��������� � �����: " << mpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
}

void b()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* �������� */
    string field = "YEARS";
    string value = "30";

    /* ��������� �� */
    openBD(&mainBD, mpath);

    /* ������� ������ */
    mainBD.del(field, value);

    /* ���������� ���� */
    mainBD.write_buffer();

    cout << "��������� � �����: " << mpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
}

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

    /* ������� ������ */
    mainBD.sort(field);

    /* ���������� ���� */
    mainBD.write_buffer(selpath);

    cout << "��������� � �����: " << selpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
}

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
    string field = "YEARS";
    string value = "30";

    /* ��������� �� */
    openBD(&mainBD, mpath);

    /* ������� ������ */
    mainBD.select(field, value);

    /* ���������� ���� */
    mainBD.write_buffer(selpath);

    cout << "��������� � �����: " << selpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
}

void e()
{
    /* �������� ����� */
    system("cls");

    KursBDClass mainBD;

    /* ��������������� ���� ������ */
    string mpath = DBPATH;
    mpath += MAINDBNAME;

    /* ��������� �� */
    openBD(&mainBD, mpath);

    /* ������� ������ � ����� ����� */
    mainBD.insert("124;Galina;Zuyeva;46;Cleaner");

    /* ���������� ���� */
    mainBD.write_buffer();

    cout << "��������� � �����: " << mpath << "\n\n\n";

    /* ��������� �� */
    mainBD.close();
}

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
    cout << "���������� �� ����" << sort_field << "\n";
    mainBD.sort(mainBD.get_sort_field());

    /* ���������� ������ � ���� */
    mainBD.write_buffer(sopath);

    cout << "��������� � �����: " << sopath << "\n\n\n";
    scnDB.close();
    mainBD.close();

    /* ��������� ��������� */
    system("pause");
}

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
    string value2 = "Maria";

    string field3 = "POSITION";
    string value3 = "Admin";

    /* ��������� ������ ������� */
    cout << "���������� �� ����: " << field1 << "; �� ���������: " << value1 << "\n";
    /* ��������� ������ �������*/
    mainBD.select(field1, value1);
    cout << "���������� �� ����: " << field2 << "; �� ���������: " << value2 << "\n";
    /* ��������� ������ ������� */
    mainBD.next_select(field2, value2);
    cout << "���������� �� ����: " << field3 << "; �� ���������: " << value3 << "\n";
    mainBD.next_select(field3, value3);

    /* ���������� ������ � ���� */
    mainBD.write_buffer(selpath);
    mainBD.close();

    /* ��������� ��������� */
    system("pause");
}
