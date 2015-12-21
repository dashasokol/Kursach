#ifndef KURSBDCLASS_H
#define KURSBDCLASS_H

#define TABLELINES 50      // ������������ ���������� ������� � �������
#define LINELEN 255         // ������������ ������ ������
#define SYM_SEPARATOR ";"       // ������ ����������� �����
#define SYM_HEADER "#"
#define SYM_HEADER_SEPARATOR "|"
#define MAXLEN 10

/* ��������� ������� */
struct table
{
    unsigned int number; // �������������
    std::string field;    // ���
    std::string value;    // �������
};

/* ������������ �������� */
enum
{
    END_WRONG_FORMAT = -2,  // �������� ������
    END_NOT_FOUND,          // ����� ���������� ��������
    END_OK,                 // ������� ����������� ��� ������
    END_EXIT,               // ������� �����������, ��������� �� ���������
    END_OPEN_FAIL           // �������� � ��������� �����
};

/*
 * ����� ��� ������ � ��
 */
class KursBDClass
{
    struct table tb[MAXLEN * TABLELINES];               // ��������� � �������
    std::string string_for_write;                       // ������ ������
    std::string table_header[MAXLEN];                   // ����� �������
    unsigned int header_col;                            // ���������� ����� �������
    int sort_field;                                     // ���� ����������
    unsigned int order_len;                             // ������ �������, �� ��������
                                                        // ������������ ������� ������
    unsigned int order[TABLELINES];                     // ������, �� ��������
                                                        // ������������ ������� ������
    unsigned int table_length;                          // ���������� ������� � ��
    FILE *bd_out_file;                                  // ���������� ����� ��
    int getHeader(std::string string_to_parse);         // ������� �������� ����� �������
    void getValue(std::string *var, std::string *val);  // �������, ���������� ��������� ��������
    int clean_db(FILE *bd);                             // ������� �������� ���� ��
    void stringInsert(unsigned int number);             // ������� ��������� ������ ���������� ������
                                                        // ����� ������ �� �� � ������ ������
    void get_order_string();                            // ������� ������ ������ � ������ ������
    void order_clear(unsigned int length);              // �������� �������, �� ������� ������������ �������
    int add_to_bd(FILE *bd, const char *string);        // �������� ��� ������ ������ � ���� ��
    int isOrder(unsigned int number);                   // ������� ��������� ��������� ����� � ������ order
    int sort_table(std::string field);                  // ������� ���������� ������ ������� ������������
                                                        // �������� �������
public:
    KursBDClass();                                      // �����������
    ~KursBDClass();                                     // ����������
    int open(std::string BD_file_name);                 // ������� ��������� � ������ �� �� �����
    void close();                                       // ������� �������� ��
    int write_buffer(std::string s_file_name);          // ������� ���������� ������ ������ � ��������� ����
    int write_buffer();                                 // ������� ���������� ������ ������ � �������� ����
    void select(std::string field,                      // ������� ���� ������ � ������� �� �����, �����������
                std::string value);                     // ��������� ��������
    void next_select(std::string field,                 // ������� ���������� ������ ������ � �������
                     std::string value);                // �� ����, ����������� ��������
    int add(std::string string_to_parse);              // ������� ��������� ����� �������� � ��
    void del(std::string field,                         // �������� ���� �������, �� ���� �������������
             std::string value);                        // ���������� ��������
    int sort(std::string field);                        // �������� ��� ������� ���������� �� ����
    void insert(std::string value);                     // ������� � ��������������� ��
    unsigned int get_length();                          // ������� ���������� ������ ��
    std::string get_entry(unsigned int number);         // ������� ���������� ��������� ������ ��
};

#endif // KURSBDCLASS_H

