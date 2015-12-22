#include <iostream>         // printf, scanf, NULL
#include <cstdlib>          // malloc, free, rand
#include <cstring>          // strstr, str[n]cat, strlen
#include <string>           // std::string
#include "helpfun.h"        // ������������ ���� helpfun
#include "kursbdclass.h"    // ������������ ���� ����� ������

/**
 * fn KursBDClass()
 * @brief ����������� ������
 */
KursBDClass::KursBDClass()
{
    table_length = 0;   // �������� ���������� ���������
    order_len = 0;      // �������� ������ ������� ����������
    header_col = 0;     // �������� ���������� ����� �������
    sort_field = -1;    // �������� id ���� ����������
}

/**
 * fn KursBDClass()
 * @brief ���������� ������
 */
KursBDClass::~KursBDClass()
{
    table_length = 0;   // �������� ���������� ���������
    order_len = 0;      // �������� ������ ������� ����������
    header_col = 0;     // �������� ���������� ����� �������
    sort_field = -1;    // �������� id ���� ����������
}

/**
 * fn int getHeader(std::string string_to_parse)
 * @brief ������� �������� ����� �������
 * @param string_to_parse - ������ ��� ��������
 * @return ��� ���������� (END_EXIT - ������ ������ ������ "|")
 */
int KursBDClass::getHeader(std::string string_to_parse)
{
    unsigned int h_sep_pos; // ������� ������� �����������

    /* ������� �� ������ ������ "#" */
    string_to_parse = string_to_parse.substr(1, string_to_parse.size() - 1);

    /* ���� ���� ������ ����������� */
    while ((h_sep_pos = string_to_parse.find(SYM_HEADER_SEPARATOR)) < string_to_parse.size())
    {
        /* ��������� ����� �������� */
        table_header[header_col++] = string_to_parse.substr(0, h_sep_pos);

        /* �������� ������ */
        string_to_parse = string_to_parse.substr(h_sep_pos + 1, string_to_parse.size() - h_sep_pos);
    }

    /* ����� ���������� ������� "|" ��� ������ */
    if (string_to_parse.size() == 0)
        return END_EXIT;

    /* ���� ����� ������ */
    if ((h_sep_pos = string_to_parse.find("\n")) > string_to_parse.size())
        h_sep_pos = string_to_parse.size();

    /* ��������� ��������� �������� */
    table_header[header_col++] = string_to_parse.substr(0, h_sep_pos);

    return END_OK;
}

/**
 * fn FILE *open_and_parse(char *BD_file_name)
 * @brief ������� ��������� � ������ �� �� �����
 * @param BD_file_name - ������ ��� ����� ��
 * @return ��� ���������� / ������
 */
int KursBDClass::open(std::string BD_file_name)
{
    char buff[LINELEN];             // �����
    int str_num = 0;                // ����� ������
    std::string string_to_parse;    // ������ ��� �������

    /* ��������� ���� */
    bd_out_file = fmopen(BD_file_name.c_str(), "r+", "KursBDClass::open");

    /* �� ����� ������� ���� */
    if (!bd_out_file)
        return END_OPEN_FAIL;

    /* ��������� ��������� ������ � ��������� */
    while(fgets(buff, LINELEN, bd_out_file))
    {
        string_to_parse = buff;

        /* ���� ������ ��������� */
        if (string_to_parse.find(SYM_HEADER) == 0)
        {
            /* ��������� ��������� � ��������, ��� �� ������ */
            if (header_col == 0)
                getHeader(string_to_parse);
        }
        else
        {
            /* ���� ��� �����, �� ������ ������ ������ - ������� ������ */
            if (header_col == 0)
                return END_WRONG_FORMAT;

            /* ������ ������ */
            if (add(string_to_parse) == END_WRONG_FORMAT)
            {
                /* ������ ����� ������������ ������ */
                fprintf(stderr, "������ %d: ������������ ������ \n", str_num);
            }

            /* ����������� ����� ������ */
            str_num++;
        }
    }

    return END_OK;
}

/**
 * @fn void close()
 * @brief ������� �������� ��
 */
void KursBDClass::close()
{
    fclose(bd_out_file); // �������� �����
}

/**
 * @fn int add(std::string string_to_parse)
 * @brief ������� ��������� ������
 * @param string_to_parse - ������
 * @return ��� �������� ���������� / ������
 */
int KursBDClass::add(std::string string_to_parse)
{
    unsigned int i = 0;     // �������
    unsigned int el_num;    // ����� ������

    /* ��������� �������� ������ � ��������� ������ */
    for (i = 0; i<header_col; i++)
    {
        el_num = table_length * header_col + i;
        tb[el_num].number = table_length;

        tb[el_num].field = table_header[i];

        /* ��������� �������� */
        getValue(&tb[el_num].value, &string_to_parse);
    }

    /* ����������� ���������� ����������� ������� */
    table_length++;
    
    /* �������� ������ order */
    order_clear(table_length);

    return END_OK;
}

/**
 * @fn int getValue(std::string *var, std::string *val)
 * @brief ������� ��������� ��������
 * @param var - ���������� ��� ������ ��������
 * @param val - ������ ��� ��������
 * @return ��� �������� ���������� / ������
 */
void KursBDClass::getValue(std::string *var, std::string *val)
{
    unsigned int pos;    // ������� ��� ������

    /* ����� ������� ����������� */
    if ((pos = val->find(SYM_SEPARATOR)) > val->size())
        /* ������ ��������� ����� ������ */
        if (val->find("\n") < val->size())
            /* ������ ������� ������ */
            *var = val->substr(0, val->size() - 1);
        else
            /* ������� ������ �� ������ */
            *var = *val;
    else
    {
        /* ������� ��������� ����� ������ (�� �����) */
        *var = val->substr(0, pos);

        /* �������� ������ ��� �������� */
        *val = val->substr(pos+1, val->size() - pos);
    }
}

/**
 * @fn int clean_db(FILE *bd)
 * @brief ������� �������� ���� ��
 * @param bd - ���������� �����
 * @return ��� �������� ���������� / ������
 */
int KursBDClass::clean_db(FILE *bd)
{
    /* ��������� ���� ��������� */
    return fmclean(bd);
}

/**
 * void stringInsert(unsigned int number)
 * @brief ������� ��������� ������, ���������� ������ ����� ������ �� ��, � ������ ������
 * @param number - ����� ������
 */
void KursBDClass::stringInsert(unsigned int number)
{
    unsigned int i; // �������

    /* ��������� ������ � ������ ������ */
    for (i = 0; i < header_col; i++)
    {
        string_for_write += tb[(number * header_col + i)].value;
        string_for_write += i == header_col-1 ? "\n" : ";";
    }
}

/**
 * @fn int write_buffer(std::string s_file_name)
 * @brief ������� ���������� ������ ������ � ��������� ����
 * @param s_file_name - ��� ����� ��� ������
 * @return ��� ������� ������ / ������
 */
int KursBDClass::write_buffer(std::string s_file_name)
{
	/* �������� ��������� ������ � ������ ������ */
    get_order_string();
	
    /* ��������� ���� */
    FILE *file = fmopen(s_file_name.c_str(), "r+", "");

    /* ������� ������ ������ */
    clean_db(file);

    /* ��������� ������ � ���� */
    return add_to_bd(file, string_for_write.c_str());
}

/**
 * @fn int write_buffer()
 * @brief ������� ���������� ������ ������ � �������� ����
 * @return ��� ������� ������ / ������
 */
int KursBDClass::write_buffer()
{
	/* �������� ��������� ������ � ������ ������ */
    get_order_string();
	
    /* ������� ������ ������ */
    clean_db(bd_out_file);

    /* ��������� ������ � ���� */
    return add_to_bd(bd_out_file, string_for_write.c_str());
}

/**
 * @fn void select(std::string field, std::string value)
 * @brief ������� ���� ������ � �������, �� ����, ����������� ��������� ��������
 * @param field - ��� ���� ��� ������
 * @param value - �������� ��� ������
 */
void KursBDClass::select(std::string field, std::string value)
{
    unsigned int i; // �������
    int h_num = -1; // ����� ����

    /* ���� ����� ���� */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            h_num = i;

    /* �������� ������ order */
    order_clear(0);

    /* ���� ������, ��������������� ������� */
    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i*header_col+h_num].value == value)
                /* ��������� ��������� ����� ������ � order */
                order[order_len++] = i;
    }

    /* �������� ��� ��������� ������ � ������ ������ */
    get_order_string();
}
/**
 * @fn void next_select(std::string field, std::string value)
 * @brief ������� ���������� ������ ������ � �������, �� ����, ����������� ��������� ��������
 * @param field - ��� ���� ��� ������
 * @param value - �������� ��� ������
 */
void KursBDClass::next_select(std::string field, std::string value)
{
    unsigned int i; // �������
    int h_num = -1; // ����� ����
    unsigned int ord[TABLELINES]; // ������ ��� ����������� order
    unsigned int ord_len = order_len; // ���������� ��� ����������� order_len

    /* �������� ������ order � ord */
    for (i = 0; i < order_len; i++)
        ord[i] = order[i];

    /* ���� ����� ���� */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            h_num = i;

    /* �������� ������ order */
    order_clear(0);

    /* ���� ������, ��������������� ������� */
    if (h_num >= 0)
    {
        for (i = 0; i < ord_len; i++)
            if (tb[ord[i]*header_col+h_num].value == value)
                /* ��������� ��������� ����� ������ � order */
                order[order_len++] = ord[i];
    }
}

/**
 * @fn void get_order_string()
 * @brief ������� ������ ������ � ������ ������
 */
void KursBDClass::get_order_string()
{
    unsigned int i; // �������

    /* �������� ������ */
    string_for_write.clear();

    string_for_write = "#";
    /* ����������, � ������ ������, �������� ������� */
    for (i = 0; i < header_col; i++)
    {
        string_for_write += table_header[i];
        string_for_write += i == header_col-1 ? "" : "|";
    }
    string_for_write += "\n";

    /* ���������, � ������ ������, ������ �� �������, �������� ������� ������� */
    for (i = 0; i < order_len; i++)
        stringInsert(order[i]);
}

/**
 * @fn void order_clear(unsigned int length)
 * @brief �������� �������, �� �������� ������������ �������
 * @param length - �������� ������, ������� ������ ����� ������
 */
void KursBDClass::order_clear(unsigned int length)
{
    unsigned int i; //�������

    /* �����������  �������� ������� � ������� ����������� */
    for (i = 0; i < length; i++)
        order[i] = i;

    /* ��������� �������� ������ ������� */
    order_len = length;
}

/**
 * @fn int add_to_bd(FILE *bd, const char *string)
 * @brief �������� ��� ������ ������ � ���� ��
 * @param bd - ���������� �����
 * @param string - ������ ��� ������
 * @return ��� ������� ������ / ������ ������
 */
int KursBDClass::add_to_bd(FILE *bd, const char *string)
{
    /* ���������� ���� ������ �������� � ���� � ������� 0 */
    return fmwrite(bd, string, strlen(string));
}

/**
 * @fn int isOrder(unsigned int number)
 * @brief ������� ��������� ��������� ����� � ������ order
 * @param number - �����
 * @return END_OK - ������� ������ / END_EXIT - ������� �� ������
 */
int KursBDClass::isOrder(unsigned int number)
{
    unsigned int i; // �������

    /* ��������� ������ ������� order */
    for (i = 0; i < order_len; i++)
        if (number == order[i])
            /* ������� ������ */
            return END_OK;

    /* ������� �� ������ */
    return END_EXIT;
}


/**
 * @fn void del(std::string field, std::string value)
 * @brief �������� ���� �������, �� ���� ������������� ���������� ��������
 * @param field - �������� ����
 * @param value - ��������
 */
void KursBDClass::del(std::string field, std::string value)
{
    unsigned int i, j;              // �������
    int h_num = -1;                 // ����� ����
  //  unsigned int new_length = 0;    // ����� ������ ��

    /* ���� ����� ���� */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            h_num = i;

    /* �������� ������ �� �������� ������������ ������� ������ */
    order_clear(0);

    /* ���� ������ �� ������������� �������, �������� � � order */
    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i*header_col+h_num].value != value)
                order[order_len++] = i;
    }

    /* ��������� �������� number ��� ������ ������ */
    for (i = 0; i < order_len; i++)
    {
        for (j = 0; j < header_col; j++)
            tb[i*header_col+j] = tb[order[i]*header_col+j];

//        if (isOrder(i) == END_EXIT)
//        {
//            for (j = 0; j < header_col; j++)
//                tb[i*header_col+j].number = new_length;

//            new_length++;
//        }
    }

    /* �������� ������ �� */
    table_length = order_len;
}

/**
 * @fn int sort(std::string field)
 * @brief �������� ��� ������� ���������� �� ����
 * @param field - �������� ����
 * @return ��� ������� ���������� / ������
 */
int KursBDClass::sort(std::string field)
{
    /* �������������� ������ ������� */
    order_clear(table_length);

    /* ��������� ������ */
    int ret = sort_table(field);

    return ret;
}

/**
 * @fn int sort_table(std::string field)
 * @brief ������� ���������� ������ ������� ������������ �������� �������
 * @param field - ������, ������������ �������� ����������� ord
 * @return ��� ������� ���������� / ������
 */
int KursBDClass::sort_table(std::string field)
{
    unsigned int i;                 // �������
    const char *values[TABLELINES]; // ������

    sort_field = -1;

    /* ���������� ����� ���� ��� ���������� */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            sort_field = i;

    /* ���� ���� ������� */
    if (sort_field >= 0)
    {
        /* ��������� ������ values */
        for (i = 0; i < table_length; i++)
            values[i] = tb[i*header_col+sort_field].value.c_str();

        /* ��������� ������ */
        qsort_dmas((int *) order, values, table_length-1, T_CHAR);
    }
    else
    {
        /* ��� ���� �� ������� */
        fprintf(stderr, "�������� ��� ���� %s\n", field.c_str());
        return END_NOT_FOUND;
    }

    return END_OK;
}

/**
 * @fn void insert(std::string value)
 * @brief ������� � ��������������� ��
 * @param value - ������ ��� ���������� � �������
 */
void KursBDClass::insert(std::string value)
{
    unsigned int i, j; // ��������

    /* �������� ������ � ������� */
    add(value);

    if (sort_field > 0)
    {
        /* ���������� �� */
        sort_table(table_header[sort_field]);

        /* ��������� �������� number */
        for(i = 0; i < table_length; i++)
            for(j = 0; j < header_col; j++)
                tb[i*header_col+j].number = order[i];

        order_len = table_length;
    }
}

/**
 * @fn unsigned int get_length()
 * @brief ������� ���������� ������ ��
 * @return ������ ��
 */
unsigned int KursBDClass::get_length()
{
    return table_length;
}

/**
 * @fn std::string get_entry(unsigned int number)
 * @brief ������� ���������� ��������� ������ ��
 * @param number - ����� ������
 * @return ������ �� ��
 */
std::string KursBDClass::get_entry(unsigned int number)
{
    std::string tmp;    // ��������� ���������� ��� �������� ������
    unsigned int i;     // �������

    /* �������� ���������� */
    tmp.clear();

    /* ��������� ������ */
    for (i = 0; i < header_col; i++)
    {
        tmp += tb[(number * header_col + i)].value;
        tmp += i == header_col-1 ? "\n" : ";";
    }

    /* ���������� ������ � ��������� ������� */
    return tmp;
}

/**
 * @fn std::string get_sort_field()
 * @brief ������� ���������� ���� ����������
 * @return ���� ����������
 */
std::string KursBDClass::get_sort_field()
{
    return table_header[sort_field];
}
