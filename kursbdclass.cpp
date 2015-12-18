#include <iostream>         // printf, scanf, NULL
#include <cstdlib>          // malloc, free, rand
#include <cstring>          // strstr, str[n]cat, strlen
#include <string>
#include "helpfun.h"        // ������������ ���� helpfun
#include "kursbdclass.h"    // ������������ ���� ����� ������

/* ������������� ������ */
KursBDClass::KursBDClass()
{
    table_length = 0;   // �������� ���������� ���������
    order_len = 0;      // �������� ������ ������� ����������
    header_col = 0;
    sort_field = -1;
}

KursBDClass::~KursBDClass()
{
    table_length = 0;   // �������� ���������� ���������
    order_len = 0;      // �������� ������ ������� ����������
    header_col = 0;
    sort_field = -1;

    memset(tb, 0, sizeof(tb));
    memset(table_header, 0, sizeof(table_header));
    memset(order, 0, sizeof(order));
}

int KursBDClass::getHeader(std::string string_to_parse)
{
    unsigned int h_sep_pos;

    string_to_parse = string_to_parse.substr(1, string_to_parse.size() - 1);

    while ((h_sep_pos = string_to_parse.find(SYM_HEADER_SEPARATOR)) > 0 &&
            h_sep_pos < string_to_parse.size())
    {
        table_header[header_col++] = string_to_parse.substr(0, h_sep_pos);

        string_to_parse = string_to_parse.substr(h_sep_pos, string_to_parse.size() - (h_sep_pos + 1));
    }

    if (string_to_parse.size() == 0)
        return END_EXIT;

    table_header[header_col++] = string_to_parse.substr(0, string_to_parse.size());

    return END_OK;
}

/**
 * fn FILE *open_and_parse(char *BD_file_name, struct table *data_table, unsigned int *tb_len)
 * @brief ������� ��������� � ������ �� �� �����
 * @param BD_file_name - ������ ��� ����� ��
 * @param data_table - ��������� � �������
 * @param tb_len - ������ ��
 * @return ���������� �����
 */
int KursBDClass::open(std::string BD_file_name)
{
    char buff[LINELEN];        // �����
    int str_num = 0;           // ����� ������
    FILE *out_file;            // ���������� �����
    std::string string_to_parse;

    /* ��������� ���� */
    out_file = fmopen(BD_file_name.c_str(), "r+", "KursBDClass::open");

    /* �� ����� ������� ���� */
    if (!out_file)
        return END_OPEN_FAIL;

    /* ��������� ��������� ������ � ��������� */
    while(fgets(buff, LINELEN, out_file))
    {
        string_to_parse = buff;

        /* ���� ������ ���������, ���������� ������ */
        if (string_to_parse.find(SYM_HEADER) == 0)
        {
            if (header_col == 0)
                getHeader(string_to_parse); // ��������� ��������� � ��������, ��� �� ������
        }
        else
        {
            if (header_col == 0)
                return END_WRONG_FORMAT;

            /* ������ ������ */
            if (parse(string_to_parse) == END_WRONG_FORMAT)
            {
                /* ������ ����� ������������ ������ */
                fprintf(stderr, "Line %d: Wrong string format\n", str_num);
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
 * @fn int parse(char *string_to_parse, struct table *data_table, unsigned int *tb_len)
 * @brief ������� ��������� ������
 * @param string_to_parse - ������
 * @param data_table - ��������� �� ��������� ��
 * @param tb_len - ���������� ������� � �������
 * @return ��� �������� ���������� / ������
 */
int KursBDClass::parse(std::string string_to_parse)
{
    unsigned int pos = 0; // ������� ��� ������
    unsigned int i = 0;
    unsigned int el_num;

    for (i = 0; i<header_col; i++)
    {
        el_num = table_length * header_col + i;
        tb[el_num].number = table_length;

        tb[el_num].field = table_header[i];

        /* ��������� �������� */
        if ((pos = getValue(&tb[el_num].value, string_to_parse)) != (unsigned int) END_WRONG_FORMAT)
            string_to_parse = string_to_parse.substr(pos, string_to_parse.size() - pos);
    }

    table_length++;

    return END_OK;
}

/**
 * @fn int getValue(char *var, char *val)
 * @brief �������, ���������� ��������� ��������
 * @param var - ���������� ��� ������ ��������
 * @param val - ������ ��� ��������
 * @return ��� �������� ���������� / ������
 */
int KursBDClass::getValue(std::string *var, std::string val)
{
    int pos;    // ������� ��� ������

    /* ����� ������� ����������� */
    if (val.find(SYM_STRING_END) == 0)
    {
        /* ������� ������ � ��������� */
        val = val.substr(1, val.size()-1);
        pos = val.find(SYM_STRING_END);
    }
    else
        /* ������ ��� ������� */
        pos = val.find(SYM_STRING_END);

    /* ���� ������� ������ �������� */
    if (pos <= 0)
    {
        /* ���� ������ ��������, �������, ��� �������� "\0" */
        *var = "";
        return END_OK;
    }

    /* ������� �� ������ �������� */
    *var = val.substr(1, pos);

    return pos;
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
 * void stringInsert(char *string, struct table insert_value)
 * @brief ������� ��������� ������ ���������� ������ ����� ������ �� �� � ������ ������
 * @param string - ������ ������
 * @param insert_value - ��������� ���������� ���� ������ �� ������� ��
 */
void KursBDClass::stringInsert(unsigned int number)
{
    unsigned int i;

    for (i = 0; i < header_col; i++)
    {
        string_for_write += tb[(number * header_col + i)].value;
        string_for_write += i == header_col-1 ? ";" : "\n";
    }
}

/**
 * @fn int write_buffer(char *s_file_name)
 * @brief ������� ���������� ������ ������ � ��������� ����
 * @param s_file_name - ��� ����� ��� ������
 * @return ��� ������� ������ / ������
 */
int KursBDClass::write_buffer(std::string s_file_name)
{
    /* ��������� ���� ��� select */
    FILE *sel_file = fmopen(s_file_name.c_str(), "r+", "KursBDClass::select");

    /* ������� ������ ������ */
    clean_db(sel_file);

    /* ��������� ������ � ���� */
    return add_to_bd(sel_file, string_for_write.c_str());
}

/**
 * @fn int write_buffer()
 * @brief ������� ���������� ������ ������ � �������� ����
 * @return ��� ������� ������ / ������
 */
int KursBDClass::write_buffer()
{
    /* ������� ������ ������ */
    clean_db(bd_out_file);

    /* ��������� ������ � ���� */
    return add_to_bd(bd_out_file, string_for_write.c_str());
}
/**
 * @fn void select(char *field, char *value)
 * @brief ������� ���� ������ � ������� �� �����, ����������� ��������� ��������
 * @param field - ��� ���� ��� ������
 * @param value - �������� ��� ������
 */
void KursBDClass::select(std::string field, std::string value)
{
    unsigned int i; // �������
    int h_num = -1;

    /* �������� ������ �� �������� ������������ ������� ������ */
    for (i = 0; i < header_col; i++)
        if (field == value)
            h_num = i;

    order_clear(0);

    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[i*header_col+h_num].value == value)
                order[order_len++] = i;

    }

    /* �������� ��� ��������� ������ � ������ ������ */
    get_order_string();
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
    /* ���������� � ������ ������ �������� ������� */
    for (i = 0; i < header_col; i++)
    {
        string_for_write += table_header[i];
        string_for_write += i == header_col-1 ? "" : "|";
    }

    /* ��������� � ������ ������ ������ �� ������� �������� ������� ������� */
    for (i = 0; i < order_len; i++)
        stringInsert(i);
}

/**
 * @fn void order_clear(unsigned int length)
 * @brief �������� �������, �� ������� ������������ �������
 * @param length - �������� ������ �������, ������� ������ ����� ������
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
 * @fn void insert(struct table insert_value)
 * @brief ������� ��������� ����� �������� � ������ ������
 * @param insert_value - ������ ��� ���������� � �������
 */
void KursBDClass::add(std::string value)
{
    /* �������� ������ � ������� */
    parse(value);
}

/**
 * @fn int add_to_bd(FILE *bd, char *string)
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

int KursBDClass::isOrder(unsigned int number)
{
    unsigned int i;

    for (i = 0; i < order_len; i++)
        if (number == order[i])
            return END_OK;

    return END_EXIT;
}


/**
 * @fn void del(char *field, char *value)
 * @brief �������� ���� �������, �� ���� ������������� ���������� ���������� ��������
 * @param field - �������� ����
 * @param value - ��������
 */
void KursBDClass::del(std::string field, std::string value)
{
    unsigned int i, j; // �������
    int h_num = -1;
    unsigned int new_length = 0;

    /* �������� ������ �� �������� ������������ ������� ������ */
    for (i = 0; i < header_col; i++)
        if (field == value)
            h_num = i;

    /* �������� ������ �� �������� ������������ ������� ������ */
    order_clear(0);

    if (h_num >= 0)
    {
        for (i = 0; i < table_length; i++)
            if (tb[(i-1)*header_col+h_num].value != value)
                order[order_len++] = i;
    }

    for (i = 0; i < table_length; i++)
    {
        if (isOrder(i) == END_EXIT)
        {
            for (j = 0; j < header_col; j++)
                tb[i*header_col+j].number = new_length;

            new_length++;
        }
    }
}

/**
 * @fn int sort(char *field)
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

    /* �������� ��� ������ � ������ ������ */
    get_order_string();

    return ret;
}

/**
 * @fn int sort_table(int *ord, char *field, struct table *data_table, unsigned int tb_len)
 * @brief ������� ���������� ������ ������� ������������ �������� �������
 * @param ord - ����������� ������
 * @param field - ������, ������������ �������� ����������� ord
 * @param data_table - ��������� �� �������, ������� ����� �����������
 * @param tb_len - ������ �������
 * @return ��� ������� ���������� / ������
 */
int KursBDClass::sort_table(std::string field)
{
    unsigned int i; // �������
    char *values[TABLELINES];

    /* �������� ������ �� �������� ������������ ������� ������ */
    for (i = 0; i < header_col; i++)
        if (field == table_header[i])
            sort_field = i;

    if (sort_field >= 0)
    {
        for (i = 0; i < table_length; i++)
            strcpy(values[i], tb[i*header_col+sort_field].value.c_str());

        /* ��������� ������ */
        qsort_dmas((int *) order, values, table_length-1, T_CHAR);
    }
    else
    {
        /* ��� ���� �� ������� */
        fprintf(stderr, "Wrong field name %s\n", field.c_str());
        return END_NOT_FOUND;
    }

    return END_OK;
}

/**
 * @fn void insert_sort(struct table insert_value, char *field)
 * @brief ������� � ��������������� ��
 * @param insert_value - ������ ��� ���������� � �������
 * @param field - ��� ����, �� �������� ������������� �������
 * @return ��� ������� ���������� / ������
 */
void KursBDClass::insert(std::string value)
{
    unsigned int i, j;
    /* �������� ������ � ������� */
    parse(value);

    sort_table(table_header[sort_field]);

    for(i = 0; i < table_length; i++)
        for(j = 0; j < header_col; j++)
            tb[i*header_col+j].number = order[i];
}

