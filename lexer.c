//
// Created by W on 2024/11/3.
//

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexer.h"

//不知道枚举变量能不能导致内联函数不能展开啊 状态全部用宏定义好了
//广义DFA啊。。。
//先有状态，再读下一个，
//移动指针先检查长度！
//我愿称这个程序为屎山

#define BUFFER_TAIL_LENGTH 7
#define BUFFER_END_OF_FILE '\0'
#define START_LEXER 1001
#define IDENTIFIER_1 1011
#define IDENTIFIER_2 1012
#define IDENTIFIER_3 1013
#define IDENTIFIER_4 1014
#define IDENTIFIER_5 1015
#define IDENTIFIER_ACCEPT 1016
#define DECIMAL_NUMBER_1 1021
#define DECIMAL_NUMBER_2 1022
#define DECIMAL_NUMBER_3 1023
#define DECIMAL_NUMBER_4 1024
#define DECIMAL_NUMBER_5 1025
#define DECIMAL_NUMBER_ACCEPT 1026
#define OCTAL_NUMBER_1 1031
#define OCTAL_NUMBER_2 1032
#define OCTAL_NUMBER_3 1033
#define OCTAL_NUMBER_4 1034
#define OCTAL_NUMBER_5 1035
#define OCTAL_NUMBER_6 1036
#define OCTAL_NUMBER_ACCEPT 1037
#define HEXADECIMAL_NUMBER_1 1041
#define HEXADECIMAL_NUMBER_2 1042
#define HEXADECIMAL_NUMBER_3 1043
#define HEXADECIMAL_NUMBER_4 1044
#define HEXADECIMAL_NUMBER_5 1045
#define HEXADECIMAL_NUMBER_6 1046
#define HEXADECIMAL_NUMBER_7 1047
#define HEXADECIMAL_NUMBER_ACCEPT 1048
#define OPERATOR_1 1051
#define OPERATOR_2 1052                /* 运算符和分隔符     + | - | * | / | > | < | = | ( | ) | ;   */
#define OPERATOR_ACCEPT 1053
#define KEYWORD_1 1061
#define KEYWORD_2 1062                 /* 关键字	if | then | else | while | do   */
#define KEYWORD_IF_1 1063
#define KEYWORD_IF_2 1064
#define KEYWORD_THEN_1 1065
#define KEYWORD_THEN_2 1066
#define KEYWORD_THEN_3 1067
#define KEYWORD_THEN_4 1068
#define KEYWORD_ELSE_1 1069
#define KEYWORD_ELSE_2 1070
#define KEYWORD_ELSE_3 1071
#define KEYWORD_ELSE_4 1072
#define KEYWORD_WHILE_1 1073
#define KEYWORD_WHILE_2 1074
#define KEYWORD_WHILE_3 1075
#define KEYWORD_WHILE_4 1076
#define KEYWORD_WHILE_5 1077
#define KEYWORD_DO_1 1078
#define KEYWORD_DO_2 1079
#define KEYWORD_ACCEPT 1080
#define ERROR_INVALID_CHARACTER 2000
#define ERROR_IDENTIFIER 2001
#define ERROR_DECIMAL_NUMBER 2002
#define ERROR_OCTAL_NUMBER 2003
#define ERROR_HEXADECIMAL_NUMBER 2004
#define END_OF_INPUT 2005
#define ERROR_OPERATOR 2006


//int next_stdin() 用到的全局变量
static int state_present_stdin;
static int state_last_stdin;
static char* word_present;
static char* token_start;
static int input_position;
static int input_length;

//struct token* next_token();


/*struct token* next_buffer()；
 int lexer_analysis_file();
 这两个函数会用到的全局变量
 */
#define MAX_BUFFER_LENGTH 110              //这个一定程度上说明了读一个buffer能识别出的token的上界...
static char buffer_1[MAX_BUFFER_LENGTH];
static char buffer_2[MAX_BUFFER_LENGTH];
static int buf_1_length;
static int buf_2_length;
static int state_present_buf;

//struct token* next_buffer()；
//int add_token
static struct token* write_pos;
static long long token_count;
static int is_end_of_file;


int next_stdin() {
    //因为不用把这个存到单词表里，就不用malloc了，写一个char buffer好了
    char input[MAX_TOKEN_LENGTH];
    char first;
    int  inputL = 0;
    while ((first = (char)getchar()) != '\n') {     //windows到底是不是\r\n? 这地方类型转换是正确的吗？
        input[inputL] = first;
        inputL++;
        if (inputL >= MAX_TOKEN_LENGTH - 1) {    //标准输入太长的情况，错误处理没有。但是也用不到这个函数，其实。
            //太长了！报错！ .长度应该是“buffer已经满了，但是buffer里面没有出现过\n”
            printf("Input Buffer Is Too Long!\n");
            return 0;
        }
    }
    if (inputL == 0) {
        exit(-1);
    }
    input[inputL] = ' ';
    inputL++;
    //读结束的时候，length应该指向最后一个字符的后面一个。
    input_length = inputL;
    input_position = -1;
    word_present = &input[0];
    word_present--;
    token_start = word_present;
    state_present_stdin = START_LEXER;
    while (1) {
        switch (state_present_stdin) {
            case START_LEXER:
                state_present_stdin = start_lexer();
                break;
            case IDENTIFIER_1:
                //                state_present_stdin=identifier_1();
                break;
            case IDENTIFIER_2:
                state_present_stdin = identifier_2();
                break;
            case IDENTIFIER_3:
                state_present_stdin = identifier_3();
                break;
            case IDENTIFIER_4:
                state_present_stdin = identifier_4();
                break;
            case IDENTIFIER_5:
                state_present_stdin = identifier_5();
                break;
            case IDENTIFIER_ACCEPT:
                state_present_stdin = print_token();
                break;
            case DECIMAL_NUMBER_1:
                break;
            case DECIMAL_NUMBER_2:
                state_present_stdin = decimal_number_2();
                break;
            case DECIMAL_NUMBER_3:
                state_present_stdin = decimal_number_3();
                break;
            case DECIMAL_NUMBER_4:
                state_present_stdin = decimal_number_4();
                break;
            case DECIMAL_NUMBER_5:
                state_present_stdin = decimal_number_5();
                break;
            case DECIMAL_NUMBER_ACCEPT:
                state_present_stdin = print_token();
                break;
            case OCTAL_NUMBER_1:
                break;
            case OCTAL_NUMBER_2:
                state_present_stdin = octal_number_2();
                break;
            case OCTAL_NUMBER_3:
                state_present_stdin = octal_number_3();
                break;
            case OCTAL_NUMBER_4:
                state_present_stdin = octal_number_4();
                break;
            case OCTAL_NUMBER_5:
                state_present_stdin = octal_number_5();
                break;
            case OCTAL_NUMBER_6:
                state_present_stdin = octal_number_6();
                break;
            case OCTAL_NUMBER_ACCEPT:
                state_present_stdin = print_token();
                break;
            case HEXADECIMAL_NUMBER_1:

                break;
            case HEXADECIMAL_NUMBER_2:
                break;
            case HEXADECIMAL_NUMBER_3:
                state_present_stdin = hexadecimal_number_3();
                break;
            case HEXADECIMAL_NUMBER_4:
                state_present_stdin = hexadecimal_number_4();
                break;
            case HEXADECIMAL_NUMBER_5:
                state_present_stdin = hexadecimal_number_5();
                break;
            case HEXADECIMAL_NUMBER_6:
                state_present_stdin = hexadecimal_number_6();
                break;
            case HEXADECIMAL_NUMBER_7:
                state_present_stdin = hexadecimal_number_7();
                break;
            case HEXADECIMAL_NUMBER_ACCEPT:
                state_present_stdin = print_token();
                break;
            case OPERATOR_1:
                break;
            case OPERATOR_2:
                state_present_stdin = operator_2();
                break;
            case OPERATOR_ACCEPT:
                state_present_stdin = print_token();
                break;
            case KEYWORD_1:

                break;
            case KEYWORD_2:
                break;
            case KEYWORD_IF_1:
                state_present_stdin = keyword_if_1();
                break;
            case KEYWORD_IF_2:
                state_present_stdin = keyword_if_2();
                break;
            case KEYWORD_THEN_1:
                state_present_stdin = keyword_then_1();
                break;
            case KEYWORD_THEN_2:
                state_present_stdin = keyword_then_2();
                break;
            case KEYWORD_THEN_3:
                state_present_stdin = keyword_then_3();
                break;
            case KEYWORD_THEN_4:
                state_present_stdin = keyword_then_4();
                break;
            case KEYWORD_ELSE_1:
                state_present_stdin = keyword_else_1();
                break;
            case KEYWORD_ELSE_2:
                state_present_stdin = keyword_else_2();
                break;
            case KEYWORD_ELSE_3:
                state_present_stdin = keyword_else_3();
                break;
            case KEYWORD_ELSE_4:
                state_present_stdin = keyword_else_4();
                break;
            case KEYWORD_WHILE_1:
                state_present_stdin = keyword_while_1();
                break;
            case KEYWORD_WHILE_2:
                state_present_stdin = keyword_while_2();
                break;
            case KEYWORD_WHILE_3:
                state_present_stdin = keyword_while_3();
                break;
            case KEYWORD_WHILE_4:
                state_present_stdin = keyword_while_4();
                break;
            case KEYWORD_WHILE_5:
                state_present_stdin = keyword_while_5();
                break;
            case KEYWORD_DO_1:
                state_present_stdin = keyword_do_1();
                break;
            case KEYWORD_DO_2:
                state_present_stdin = keyword_do_2();
                break;
            case KEYWORD_ACCEPT:
                state_present_stdin = print_token();
                break;
            case ERROR_INVALID_CHARACTER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = print_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_IDENTIFIER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = print_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_DECIMAL_NUMBER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = print_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_OCTAL_NUMBER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = print_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_HEXADECIMAL_NUMBER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = print_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case END_OF_INPUT:
                printf("Done!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                return 0;
                break;
            case ERROR_OPERATOR:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = print_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            default:
                return -1;
        }
    }
    printf("notice\n");
    return 0;
}


inline int start_lexer() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    while (*word_present == ' ' || *word_present == '\n') {
        //先检查下一个是不是结尾，再去读
        input_position++;
        if (input_position >= input_length) {
            return END_OF_INPUT;
        }
        word_present++;
    }
    token_start = word_present;
    if (*word_present == 'i') {
        return KEYWORD_IF_1;
    }
    if (*word_present == 'w') {
        return KEYWORD_WHILE_1;
    }
    if (*word_present == 't') {
        return KEYWORD_THEN_1;
    }
    if (*word_present == 'd') {
        return KEYWORD_DO_1;
    }
    if (*word_present == 'e') {
        return KEYWORD_ELSE_1;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z')) {
        return IDENTIFIER_2;
    }
    if (*word_present == '0') {
        return OCTAL_NUMBER_2;
    }
    if (*word_present >= '1' && *word_present <= '9') {
        return DECIMAL_NUMBER_2;
    }
    if ((*word_present == '+') || (*word_present == '-') || (*word_present == '*') || (*word_present == '/') || (*word_present == '>') || (*word_present == '<') || (*word_present == '=') || (*word_present == '(') || (*word_present == ')') || (*word_present == ';')) {
        return OPERATOR_2;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int identifier_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    if (*word_present == '_' || *word_present == '.') {
        return IDENTIFIER_4;
    }
    return ERROR_IDENTIFIER;
}

inline int identifier_3() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    if (*word_present == '_' || *word_present == '.') {
        return IDENTIFIER_4;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return IDENTIFIER_ACCEPT;
    }
    return ERROR_IDENTIFIER;
}

inline int identifier_4() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_5;
    }
    return ERROR_IDENTIFIER;
}

inline int identifier_5() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_5;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return IDENTIFIER_ACCEPT;
    }
    return ERROR_IDENTIFIER;
}

inline int decimal_number_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present >= '0' && *word_present <= '9') {
        return DECIMAL_NUMBER_2;
    }
    if (*word_present == '.') {
        return DECIMAL_NUMBER_3;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return DECIMAL_NUMBER_ACCEPT;
    }
    return ERROR_DECIMAL_NUMBER;
}

inline int decimal_number_3() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present >= '0' && *word_present <= '9') {
        return DECIMAL_NUMBER_4;
    }
    return ERROR_DECIMAL_NUMBER;
}

inline int decimal_number_4() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present >= '0' && *word_present <= '9') {
        return DECIMAL_NUMBER_5;
    }
    return ERROR_DECIMAL_NUMBER;
}

inline int decimal_number_5() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present >= '0' && *word_present <= '9') {
        return DECIMAL_NUMBER_5;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return DECIMAL_NUMBER_ACCEPT;
    }
    return ERROR_DECIMAL_NUMBER;
}

inline int octal_number_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == '0' || *word_present == '8' || *word_present == '9') {
        return DECIMAL_NUMBER_2;
    }
    if (*word_present == 'x') {
        return HEXADECIMAL_NUMBER_3;
    }
    if (*word_present >= '1' && *word_present <= '7') {
        return OCTAL_NUMBER_3;
    }
    return ERROR_OCTAL_NUMBER;
}

inline int octal_number_3() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == '8' || *word_present == '9') {
        return DECIMAL_NUMBER_2;
    }
    if (*word_present >= '0' && *word_present <= '7') {
        return OCTAL_NUMBER_3;
    }
    if (*word_present == '.') {
        return OCTAL_NUMBER_4;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return OCTAL_NUMBER_ACCEPT;
    }
    return ERROR_OCTAL_NUMBER;
}

inline int octal_number_4() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == '8' || *word_present == '9') {
        return DECIMAL_NUMBER_4;
    }
    if (*word_present >= '0' && *word_present <= '7') {
        return OCTAL_NUMBER_5;
    }
    return ERROR_OCTAL_NUMBER;
}

inline int octal_number_5() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == '8' || *word_present == '9') {
        return DECIMAL_NUMBER_5;
    }
    if (*word_present >= '0' && *word_present <= '7') {
        return OCTAL_NUMBER_6;
    }
    return ERROR_OCTAL_NUMBER;
}

inline int octal_number_6() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == '8' || *word_present == '9') {
        return DECIMAL_NUMBER_5;
    }
    if (*word_present >= '0' && *word_present <= '7') {
        return OCTAL_NUMBER_6;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return OCTAL_NUMBER_ACCEPT;
    }
    return ERROR_OCTAL_NUMBER;
}

inline int hexadecimal_number_3() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= '0' && *word_present <= '9') || (*word_present >= 'a' && *word_present <= 'f')) {
        return HEXADECIMAL_NUMBER_4;
    }
    return ERROR_HEXADECIMAL_NUMBER;
}

inline int hexadecimal_number_4() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= '0' && *word_present <= '9') || (*word_present >= 'a' && *word_present <= 'f')) {
        return HEXADECIMAL_NUMBER_4;
    }
    if (*word_present == '.') {
        return HEXADECIMAL_NUMBER_5;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return HEXADECIMAL_NUMBER_ACCEPT;
    }
    return ERROR_HEXADECIMAL_NUMBER;
}

inline int hexadecimal_number_5() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= '0' && *word_present <= '9') || (*word_present >= 'a' && *word_present <= 'f')) {
        return HEXADECIMAL_NUMBER_6;
    }
    return ERROR_HEXADECIMAL_NUMBER;
}

inline int hexadecimal_number_6() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= '0' && *word_present <= '9') || (*word_present >= 'a' && *word_present <= 'f')) {
        return HEXADECIMAL_NUMBER_7;
    }
    return ERROR_HEXADECIMAL_NUMBER;
}

inline int hexadecimal_number_7() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if ((*word_present >= '0' && *word_present <= '9') || (*word_present >= 'a' && *word_present <= 'f')) {
        return HEXADECIMAL_NUMBER_7;
    }
    if (*word_present == ' ' || *word_present == '\n') {
        return HEXADECIMAL_NUMBER_ACCEPT;
    }
    return ERROR_HEXADECIMAL_NUMBER;
}

inline int operator_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == ' ' || *word_present == '\n') {
        return OPERATOR_ACCEPT;
    }
    return ERROR_OPERATOR;
}

inline int keyword_if_1() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'f') {
        return KEYWORD_IF_2;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_if_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == ' ' || *word_present == '\n') {
        return KEYWORD_ACCEPT;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_then_1() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'h') {
        return KEYWORD_THEN_2;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_then_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'e') {
        return KEYWORD_THEN_3;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_then_3() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'n') {
        return KEYWORD_THEN_4;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_then_4() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == ' ' || *word_present == '\n') {
        return KEYWORD_ACCEPT;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_else_1() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'l') {
        return KEYWORD_ELSE_2;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_else_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 's') {
        return KEYWORD_ELSE_3;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_else_3() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'e') {
        return KEYWORD_ELSE_4;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_else_4() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == ' ' || *word_present == '\n') {
        return KEYWORD_ACCEPT;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_while_1() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'h') {
        return KEYWORD_WHILE_2;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_while_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'i') {
        return KEYWORD_WHILE_3;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_while_3() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'l') {
        return KEYWORD_WHILE_4;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_while_4() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'e') {
        return KEYWORD_WHILE_5;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_while_5() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == ' ' || *word_present == '\n') {
        return KEYWORD_ACCEPT;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_do_1() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == 'o') {
        return KEYWORD_DO_2;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}

inline int keyword_do_2() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    if (*word_present == ' ' || *word_present == '\n') {
        return KEYWORD_ACCEPT;
    }
    if ((*word_present >= 'a' && *word_present <= 'z') || (*word_present >= 'A' && *word_present <= 'Z') || (*word_present >= '0' && *word_present <= '9')) {
        return IDENTIFIER_3;
    }
    return ERROR_INVALID_CHARACTER;
}


//待修改
int skip_to_next_valid_char() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    while (!(*word_present == ' ' || *word_present == '\n')) {
        input_position++;
        if (input_position >= input_length) {
            return END_OF_INPUT;
        }
        word_present++;
    }
    return START_LEXER;
}

//执行之后，*word_present指向找到的第一个空格或换行符\n
int skip_to_next_space() {
    input_position++;
    if (input_position >= input_length) {
        return END_OF_INPUT;
    }
    word_present++;
    while (!(*word_present == ' ' || *word_present == '\n')) {
        input_position++;
        if (input_position >= input_length) {
            return END_OF_INPUT;
        }
        word_present++;
    }
    return START_LEXER;
}

int print_token() {
    char* c;
    c = token_start;
    printf("<");
    while (c != word_present) {
        putchar(*c);
        c++;
    }
    printf(">  ");
    switch (state_present_stdin) {
        case KEYWORD_ACCEPT:
            printf(" 关键字\n");
            break;
        case OPERATOR_ACCEPT:
            printf(" 运算符或分隔符\n");
            break;
        case HEXADECIMAL_NUMBER_ACCEPT:
            printf(" 十六进制数\n");
            break;
        case DECIMAL_NUMBER_ACCEPT:
            printf(" 十进制数\n");
            break;
        case OCTAL_NUMBER_ACCEPT:
            printf(" 八进制数\n");
            break;
        case IDENTIFIER_ACCEPT:
            printf(" 标识符\n");
            break;
        case ERROR_INVALID_CHARACTER:
            printf(" 错误的字符\n");
            break;
        case ERROR_OPERATOR:
            printf(" 可能的匹配: 运算符或分隔符\n");
            break;
        case ERROR_HEXADECIMAL_NUMBER:
            printf(" 可能的匹配: 十六进制数\n");
            break;
        case ERROR_OCTAL_NUMBER:
            printf(" 可能的匹配: 八进制数\n");
            break;
        case ERROR_DECIMAL_NUMBER:
            printf(" 可能的匹配: 十进制数\n");
            break;
        case ERROR_IDENTIFIER:
            printf(" 可能的匹配: 标识符\n");
            break;
        default:
            printf("ERROR OUTPUT\n");
    }
    return START_LEXER;
}

/*
 * 作为一个独立的程序，从文件中读取源程序，将其变换成相应的符号序列；
 * 这个函数自己不解析文件，不处理文件
 * 没必要存...也可以管理两个struct token[] 来解决"每被调用一次，返回当前的一个单词"
 * 那个函数应该就管理两个指针 读两个token[]好了...
 * struct token[]应该在int lexer_analysis_file()里面malloc，返回时释放
 * 这个函数会从buffer指向的数组里面读 往tokenList里面写token 然后在token结尾写一个endOfTokens
 * 要定义一个符号用来告诉这个函数 现在是文件处理完了，在token后面写EOF
 * 读完这个buffer,就把对应的下标清零
 * 调用之前注意初始化全局变量。
 */
struct token* next_buffer(char** buffer, struct token** tokenList) {
    // y commence.
    //从头开写tokenList
    write_pos = *tokenList;
    while (1) {
        switch (state_present_stdin) {
            case START_LEXER:
                state_present_stdin = start_lexer();
                break;
            case IDENTIFIER_1:
                //                state_present_stdin=identifier_1();
                break;
            case IDENTIFIER_2:
                state_present_stdin = identifier_2();
                break;
            case IDENTIFIER_3:
                state_present_stdin = identifier_3();
                break;
            case IDENTIFIER_4:
                state_present_stdin = identifier_4();
                break;
            case IDENTIFIER_5:
                state_present_stdin = identifier_5();
                break;
            case IDENTIFIER_ACCEPT:
                state_present_stdin = add_token();
                break;
            case DECIMAL_NUMBER_1:
                break;
            case DECIMAL_NUMBER_2:
                state_present_stdin = decimal_number_2();
                break;
            case DECIMAL_NUMBER_3:
                state_present_stdin = decimal_number_3();
                break;
            case DECIMAL_NUMBER_4:
                state_present_stdin = decimal_number_4();
                break;
            case DECIMAL_NUMBER_5:
                state_present_stdin = decimal_number_5();
                break;
            case DECIMAL_NUMBER_ACCEPT:
                state_present_stdin = add_token();
                break;
            case OCTAL_NUMBER_1:
                break;
            case OCTAL_NUMBER_2:
                state_present_stdin = octal_number_2();
                break;
            case OCTAL_NUMBER_3:
                state_present_stdin = octal_number_3();
                break;
            case OCTAL_NUMBER_4:
                state_present_stdin = octal_number_4();
                break;
            case OCTAL_NUMBER_5:
                state_present_stdin = octal_number_5();
                break;
            case OCTAL_NUMBER_6:
                state_present_stdin = octal_number_6();
                break;
            case OCTAL_NUMBER_ACCEPT:
                state_present_stdin = add_token();
                break;
            case HEXADECIMAL_NUMBER_1:

                break;
            case HEXADECIMAL_NUMBER_2:
                break;
            case HEXADECIMAL_NUMBER_3:
                state_present_stdin = hexadecimal_number_3();
                break;
            case HEXADECIMAL_NUMBER_4:
                state_present_stdin = hexadecimal_number_4();
                break;
            case HEXADECIMAL_NUMBER_5:
                state_present_stdin = hexadecimal_number_5();
                break;
            case HEXADECIMAL_NUMBER_6:
                state_present_stdin = hexadecimal_number_6();
                break;
            case HEXADECIMAL_NUMBER_7:
                state_present_stdin = hexadecimal_number_7();
                break;
            case HEXADECIMAL_NUMBER_ACCEPT:
                state_present_stdin = add_token();
                break;
            case OPERATOR_1:
                break;
            case OPERATOR_2:
                state_present_stdin = operator_2();
                break;
            case OPERATOR_ACCEPT:
                state_present_stdin = add_token();
                break;
            case KEYWORD_1:

                break;
            case KEYWORD_2:
                break;
            case KEYWORD_IF_1:
                state_present_stdin = keyword_if_1();
                break;
            case KEYWORD_IF_2:
                state_present_stdin = keyword_if_2();
                break;
            case KEYWORD_THEN_1:
                state_present_stdin = keyword_then_1();
                break;
            case KEYWORD_THEN_2:
                state_present_stdin = keyword_then_2();
                break;
            case KEYWORD_THEN_3:
                state_present_stdin = keyword_then_3();
                break;
            case KEYWORD_THEN_4:
                state_present_stdin = keyword_then_4();
                break;
            case KEYWORD_ELSE_1:
                state_present_stdin = keyword_else_1();
                break;
            case KEYWORD_ELSE_2:
                state_present_stdin = keyword_else_2();
                break;
            case KEYWORD_ELSE_3:
                state_present_stdin = keyword_else_3();
                break;
            case KEYWORD_ELSE_4:
                state_present_stdin = keyword_else_4();
                break;
            case KEYWORD_WHILE_1:
                state_present_stdin = keyword_while_1();
                break;
            case KEYWORD_WHILE_2:
                state_present_stdin = keyword_while_2();
                break;
            case KEYWORD_WHILE_3:
                state_present_stdin = keyword_while_3();
                break;
            case KEYWORD_WHILE_4:
                state_present_stdin = keyword_while_4();
                break;
            case KEYWORD_WHILE_5:
                state_present_stdin = keyword_while_5();
                break;
            case KEYWORD_DO_1:
                state_present_stdin = keyword_do_1();
                break;
            case KEYWORD_DO_2:
                state_present_stdin = keyword_do_2();
                break;
            case KEYWORD_ACCEPT:
                state_present_stdin = add_token();
                break;
            case ERROR_INVALID_CHARACTER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = add_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_IDENTIFIER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = add_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_DECIMAL_NUMBER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = add_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_OCTAL_NUMBER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = add_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case ERROR_HEXADECIMAL_NUMBER:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = add_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            case END_OF_INPUT:
                printf("DoneDone!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n!");
                goto endBuffer;
                break;
            case ERROR_OPERATOR:
                if (skip_to_next_space() == START_LEXER) {
                    state_present_stdin = add_token();
                }
                else {
                    state_present_stdin = END_OF_INPUT;
                }
                break;
            default:
                return NULL;
        }
    }
    endBuffer:
    if (is_end_of_file) {
//        write_pos--;
        write_pos->type = endOfFile;
    }
    else{
        if(!is_end_of_file){
//            write_pos--;
            write_pos->type=endOfTokens;
        }
    }
    //token写完了
    if (*buffer == buffer_1) {
        buf_1_length = 0;
    }
    else if (*buffer == buffer_2) {
        buf_2_length = 0;
    }
    return NULL;
}

/*
 * 作为一个独立的程序，从文件中读取源程序，将其变换成相应的符号序列；
 * 作为一个独立程序 这个应该接受命令行参数：解析的文件的地址
 * 会填写一个符号表 结构是 struct token[] 想写到一个新文件里面。二进制保存。
 */
int lexer_analysis_file(char* address) {
    //这个函数处理把文件写进缓冲区。 不想在状态机里面管理“读完这个缓冲区了，但是不知道当前匹配的单词是完整读进缓冲区了吗？”这种问题
    //写进去的是 (空格|换行）* 单词A (空格|换行）* 单词B (空格|换行）+  这种东西，同时记录这个buffer有效长度(最后一个无关字符)
    //可能需要处理一下 把各种需要跳过的无用字符替换成空格塞进缓冲区 要不检查太多余
    //也就是说，这个东西需要一个回溯功能，如果超过了缓冲区长度，就把已经写进去的东西抹掉，回退读文件指针?，直到最近的前一个无用字符
    //判断EOF来结束对一个文件的分析

    //假设写死一个地址： 理想情况是有main接受命令行参数，然后把那个文件地址传进这个函数里面。
    FILE* fp = NULL;
    fp = fopen(address, "r");
    if (fp == NULL) {
        printf("file not found\n");
        return 9;
    }
    /*坑:     字符模式：读: 遇到回车-换行(0D 0A)，就会转换为换行符0A
              字符模式：写：遇到换行符0A，就会转换为回车-换行(0D 0A)
     * */
    //全局变量初始化
    token_count = 0;
    is_end_of_file = 0;
    buf_1_length = 0;
    buf_2_length = 0;
    //创建token list
    struct token* tokenList1, * tokenList2;
    tokenList1 = (struct token*)malloc((MAX_BUFFER_LENGTH) * sizeof(struct token));
    tokenList2 = (struct token*)malloc((MAX_BUFFER_LENGTH) * sizeof(struct token));

    int lastBuffer = 2;   //"上一个用的"
    int nextBuffer = 1;   //"空的"

    char c;
    while (!is_end_of_file) {
        //选择然后写一个buffer 好像单线程这个是没有意义的？
        if (lastBuffer == 2) { //"现在1是空的"
            /*这里要实现一个简单的预处理器，把单词分隔开*/
            //如果要处理太长的标识符，也在这里。如果读到DFA里面会导致一直在重复倒两个缓冲区，丢掉之前的东西。
            lastBuffer = 1;
            //这个地方MAX_BUFFER_LENGTH得根据多写进去的字符改改...
            while (buf_1_length < MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH && ((c = fgetc(fp)) != EOF)) {
                if (c == '\n' || c == '\r') {
                    buffer_1[buf_1_length] = ' ';
                    buf_1_length++;
                }
                else {   //是有含义的字符
                    /*这里要实现一个简单的预处理器，把单词分隔开*/
                    //预处理，填入必要的空格和处理过长的字符串。设置一个“变量过长错误”，检查到两个空格之间太长了强制停止编译。
                    //只能在这里写预处理了
                    //<为运算符和分隔符两边加上空格>开始    感觉这个地方和判断c=EOF都得改...为了写预处理估计得把函数全部重写。
                    if((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '>') || (c == '<') || (c == '=') || (c == '(') || (c == ')') || (c == ';')){
                        buffer_1[buf_1_length]=' ';
                        buf_1_length++;
                        buffer_1[buf_1_length] = c;
                        buf_1_length++;
                        buffer_1[buf_1_length]=' ';
                        buf_1_length++;
                    }
                    else {//<为运算符和分隔符两边加上空格>结束
                        buffer_1[buf_1_length] = c;
                        buf_1_length++;
                    }
                }
            }
            buffer_1[buf_1_length] = ' ';
            //多的部分放进另一个缓冲区 这个分支说明还没读fp
            //如果这里检查buffer边界的条件不一致，会导致没法正常进入if
            // 空格 字符？ 人为写的空格
            //               buflength
            if (buf_1_length >= MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH) {
                int rewind = buf_1_length - 1;
                buf_2_length = 0;
                while (buffer_1[rewind] != ' ') {
                    rewind--;
                }
                buf_1_length = rewind;
                rewind++;
                while (buffer_1[rewind] != ' ') {
                    buffer_2[buf_2_length] = buffer_1[rewind];
                    rewind++;
                    buf_2_length++;
                }
            }
            if (c == EOF) {
                buf_1_length++;
                buffer_1[buf_1_length] = ' ';
                buf_1_length++;
                buffer_1[buf_1_length] = ' ';
                buffer_1[buf_1_length + 1] = ' ';
                is_end_of_file = 1;
            }
            buf_1_length++;//指向最后一个字符的后面一个位置
            //开始解析缓冲区
            //只能复用这些全局变量了
            input_length = buf_1_length;
            input_position = -1;
            word_present = &buffer_1[0];
            word_present--;
            token_start = word_present;
            state_present_stdin = START_LEXER;
            next_buffer((char**)&buffer_1, &tokenList1);
        }
        else {   //“现在2是空的”
            lastBuffer = 2;
            //清空记录
//            buf_2_length = 0;
            while (buf_2_length < MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH && ((c = fgetc(fp)) != EOF)) {
                if (c == '\n' || c == '\r') {
                    buffer_2[buf_2_length] = ' ';
                    buf_2_length++;
                }
                else {   //是有含义的字符
                    if((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '>') || (c == '<') || (c == '=') || (c == '(') || (c == ')') || (c == ';')){
                        buffer_2[buf_2_length]=' ';
                        buf_2_length++;
                        buffer_2[buf_2_length] = c;
                        buf_2_length++;
                        buffer_2[buf_2_length]=' ';
                        buf_2_length++;
                    }
                    else {//<为运算符和分隔符两边加上空格>结束
                        buffer_2[buf_2_length] = c;
                        buf_2_length++;
                    }
                }
            }
            buffer_2[buf_2_length] = ' ';
            buffer_2[buf_2_length + 1] = ' ';
            //多的部分放进另一个缓冲区 这个分支说明还没读fp
            // 空格 字符？ 人为写的空格
            //               buflength
            if (buf_2_length >= MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH) {
                int rewind = buf_2_length - 1;
                buf_1_length = 0;
                while (buffer_2[rewind] != ' ') {
                    rewind--;
                }
                buf_2_length = rewind;
                rewind++;
                while (buffer_2[rewind] != ' ') {
                    buffer_1[buf_1_length] = buffer_2[rewind];
                    rewind++;
                    buf_1_length++;
                }
            }

            if (c == EOF) {
                buf_2_length++;
                buffer_2[buf_2_length] = BUFFER_END_OF_FILE;
                buf_2_length++;
                buffer_2[buf_2_length] = ' ';
                buffer_2[buf_2_length + 1] = ' ';
                is_end_of_file = 1;
            }
            buf_2_length++;//指向最后一个字符的后面一个位置
            //开始解析缓冲区
            //只能复用这些全局变量了
            input_length = buf_2_length;
            input_position = -1;
            word_present = &buffer_2[0];
            word_present--;
            token_start = word_present;
            state_present_stdin = START_LEXER;
            next_buffer((char**)(&buffer_1), &tokenList2);
        }
    }
    printf("In total :%lld tokens\n", token_count);
    system("pause");
    free(tokenList1);
    free(tokenList2);
    return 0;
}


/*
 * 每被调用一次，返回当前的一个单词 这个单纯是个队列出队函数 所以这个必须有个全局变量表示
 * 队空了提醒别的函数去写新的
 * 发现EOF了提醒别的函数去把token列表free
 * 这个读的文件应该在别的地方被打开，这个使用一个外部变量（或者全局变量？）
 * 被调用的时候先检查文件到末尾了吗 返回一个格式是endOfFile的toke;n。
 * 被调用的时候先检查文件到末尾了吗：这个是读全剧变量指向的 token[]的成员实现的。
 * 什么 打开文件时就把flag设为1 然后调用这玩意就设为0之类的
 *
 * 被调用先检查：
 * buffer fp token* 都是别的函数管理的 打开新文件时处理
 *
 * 1 打开了新文件吗？ 信号量 “有新文件被打开”????
 * 2 使用的buffer被申请了吗？ 检查是否为NULL
 * 3 tokenList到末尾了吗？ 检查tokenList
 * 4 是否EOF? 检查tokenList
 * 5 没问题，出队一个。
 * */

//用到的"信号量"
static int next_token_new_file=1;
static int FrontTokenList=3;
static struct token *frontToken;
static FILE *frontfp;
static struct token *tokenList1;
static struct token *tokenList2;

struct token* next_token() {
/*    if(next_token_new_file){
        printf("Opened a new file\n");
    }*/
    if(FrontTokenList==3){
        FrontTokenList=load_buffers(frontfp,tokenList1,tokenList2);
    }

    if(frontToken->type==endOfTokens){
        load_buffers(frontfp,tokenList1,tokenList2);
    }
    if(frontToken->type==endOfFile){
        load_buffers(frontfp,tokenList1,tokenList2);    //这是去把申请的内存释放，然后请外部fclose。
        printf("EOF\n");
        return frontToken;
    }
    else{
        frontToken++;
        return frontToken-1;
    }
    printf("FATAL ERROR IN next_token(),return NULL\n");
    return NULL;
}


static int lastBuffer = 2;   //"上一个用的"
static int nextBuffer = 1;   //"空的"
//这在一个进程中只能同时打开一个文件，除非以后在调用（这个函数，next_token,next_buffer）的时候要维护一个栈保护全局变量现场
//既然这个也没考虑多线程，也没啥意义？
//别的函数调用此函数，开始词法分析

int next_token_a_new_file(FILE * fp) {
    if (fp == NULL) {
        printf("file not found\n");
        return 9;
    }
    if(!next_token_new_file){
        printf("已经打开了一个文件，还没有关闭！\n");
        return 0;
    }
    if(next_token_new_file){
        next_token_new_file=0;
    }
    //既然开始了，先写两个缓冲区。
    //我之前怎么管理的？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
    /*坑:     字符模式：读: 遇到回车-换行(0D 0A)，就会转换为换行符0A
              字符模式：写：遇到换行符0A，就会转换为回车-换行(0D 0A)
     * */
    //全局变量初始化
    token_count = 0;
    is_end_of_file = 0;
    buf_1_length = 0;
    buf_2_length = 0;
    //创建token list

    tokenList1 = (struct token*)malloc((MAX_BUFFER_LENGTH) * sizeof(struct token));
    tokenList2 = (struct token*)malloc((MAX_BUFFER_LENGTH) * sizeof(struct token));
    //
    frontfp=fp;
    return 0;
}

int add_token() {
    char* c;
    c = token_start;
    //    printf("<");
    //    while(c!=word_present){
    //        putchar(*c);
    //        c++;
    //    }
    //    printf(">  ");
    int thisTokenLength = 0;
    while (c != word_present) {
        write_pos->value[thisTokenLength] = *    c;
        thisTokenLength++;
        c++;
    }
    write_pos->value[thisTokenLength] = '\0';
    switch (state_present_stdin) {
        case KEYWORD_ACCEPT:
            write_pos->type = keyword;
            break;
        case OPERATOR_ACCEPT:
            write_pos->type = operatorr;
            break;
        case HEXADECIMAL_NUMBER_ACCEPT:
            write_pos->type = hexadecimalNumber;
            break;
        case DECIMAL_NUMBER_ACCEPT:
            write_pos->type = decimalNumber;
            break;
        case OCTAL_NUMBER_ACCEPT:
            write_pos->type = octalNumber;
            break;
        case IDENTIFIER_ACCEPT:
            write_pos->type = identifier;
            break;
        case ERROR_INVALID_CHARACTER:
            write_pos->type = errorType;
            break;
        case ERROR_OPERATOR:
            write_pos->type = errorType;
            break;
        case ERROR_HEXADECIMAL_NUMBER:
            write_pos->type = errorType;
            break;
        case ERROR_OCTAL_NUMBER:
            write_pos->type = errorType;
            break;
        case ERROR_DECIMAL_NUMBER:
            write_pos->type = errorType;
            break;
        case ERROR_IDENTIFIER:
            write_pos->type = errorType;
            break;
        default:
            printf("ERROR OUTPUT\n");
    }
//    print_token_std(write_pos);
    write_pos++;
    return START_LEXER;
}

int print_token_std(struct token* token) {
    if (token == NULL) {
        printf("Try to print *NULL");
        return -1;
    }
    int pos=0;
    printf("<");
    while (token->value[pos] != '\0') {
        putchar(token->value[pos]);
        pos++;
    }
    printf(">,%d\n", token->type);
    token_count++;
    return 0;
}

int load_buffers(FILE *fp,struct token *tokenListt1,struct token *tokenListt2){
    //全局变量初始化
    /*token_count = 0;
    is_end_of_file = 0;
    buf_1_length = 0;
    buf_2_length = 0;
    //创建token list
    struct token* tokenList1, * tokenList2;
    tokenList1 = (struct token*)malloc((MAX_BUFFER_LENGTH) * sizeof(struct token));
    tokenList2 = (struct token*)malloc((MAX_BUFFER_LENGTH) * sizeof(struct token));
    */

    char c;
    while (!is_end_of_file) {
        //选择然后写一个buffer 好像单线程这个是没有意义的？
        if (lastBuffer == 2) { //"现在1是空的"
            /*这里要实现一个简单的预处理器，把单词分隔开*/
            //如果要处理太长的标识符，也在这里。如果读到DFA里面会导致一直在重复倒两个缓冲区，丢掉之前的东西。
            lastBuffer = 1;
            //这个地方MAX_BUFFER_LENGTH得根据多写进去的字符改改...
            while (buf_1_length < MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH && ((c = fgetc(fp)) != EOF)) {
                if (c == '\n' || c == '\r') {
                    buffer_1[buf_1_length] = ' ';
                    buf_1_length++;
                }
                else {   //是有含义的字符
                    /*这里要实现一个简单的预处理器，把单词分隔开*/
                    //预处理，填入必要的空格和处理过长的字符串。设置一个“变量过长错误”，检查到两个空格之间太长了强制停止编译。
                    //只能在这里写预处理了
                    //<为运算符和分隔符两边加上空格>开始    感觉这个地方和判断c=EOF都得改...为了写预处理估计得把函数全部重写。
                    if((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '>') || (c == '<') || (c == '=') || (c == '(') || (c == ')') || (c == ';')){
                        buffer_1[buf_1_length]=' ';
                        buf_1_length++;
                        buffer_1[buf_1_length] = c;
                        buf_1_length++;
                        buffer_1[buf_1_length]=' ';
                        buf_1_length++;
                    }
                    else {//<为运算符和分隔符两边加上空格>结束
                        buffer_1[buf_1_length] = c;
                        buf_1_length++;
                    }
                }
            }
            buffer_1[buf_1_length] = ' ';
            //多的部分放进另一个缓冲区 这个分支说明还没读fp
            //如果这里检查buffer边界的条件不一致，会导致没法正常进入if
            // 空格 字符？ 人为写的空格
            //               buflength
            if (buf_1_length >= MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH) {
                int rewind = buf_1_length - 1;
                buf_2_length = 0;
                while (buffer_1[rewind] != ' ') {
                    rewind--;
                }
                buf_1_length = rewind;
                rewind++;
                while (buffer_1[rewind] != ' ') {
                    buffer_2[buf_2_length] = buffer_1[rewind];
                    rewind++;
                    buf_2_length++;
                }
            }
            if (c == EOF) {
                buf_1_length++;
                buffer_1[buf_1_length] = ' ';
                buf_1_length++;
                buffer_1[buf_1_length] = ' ';
                buffer_1[buf_1_length + 1] = ' ';
                is_end_of_file = 1;
            }
            buf_1_length++;//指向最后一个字符的后面一个位置
            //开始解析缓冲区
            //只能复用这些全局变量了
            input_length = buf_1_length;
            input_position = -1;
            word_present = &buffer_1[0];
            word_present--;
            token_start = word_present;
            state_present_stdin = START_LEXER;
            next_buffer((char**)&buffer_1, &tokenListt1);
            //现在应该写完一个tokenList1了。
            //现在应该停止词法分析，直到next_token读到end_of_tokens或者end_of_file
            frontToken=tokenList1;
            return 1;
        }
        else {   //“现在2是空的”
            lastBuffer = 2;
            //清空记录
//            buf_2_length = 0;
            while (buf_2_length < MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH && ((c = fgetc(fp)) != EOF)) {
                if (c == '\n' || c == '\r') {
                    buffer_2[buf_2_length] = ' ';
                    buf_2_length++;
                }
                else {   //是有含义的字符
                    if((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '>') || (c == '<') || (c == '=') || (c == '(') || (c == ')') || (c == ';')){
                        buffer_2[buf_2_length]=' ';
                        buf_2_length++;
                        buffer_2[buf_2_length] = c;
                        buf_2_length++;
                        buffer_2[buf_2_length]=' ';
                        buf_2_length++;
                    }
                    else {//<为运算符和分隔符两边加上空格>结束
                        buffer_2[buf_2_length] = c;
                        buf_2_length++;
                    }
                }
            }
            buffer_2[buf_2_length] = ' ';
            buffer_2[buf_2_length + 1] = ' ';
            //多的部分放进另一个缓冲区 这个分支说明还没读fp
            // 空格 字符？ 人为写的空格
            //               buflength
            if (buf_2_length >= MAX_BUFFER_LENGTH - BUFFER_TAIL_LENGTH) {
                int rewind = buf_2_length - 1;
                buf_1_length = 0;
                while (buffer_2[rewind] != ' ') {
                    rewind--;
                }
                buf_2_length = rewind;
                rewind++;
                while (buffer_2[rewind] != ' ') {
                    buffer_1[buf_1_length] = buffer_2[rewind];
                    rewind++;
                    buf_1_length++;
                }
            }

            if (c == EOF) {
                buf_2_length++;
                buffer_2[buf_2_length] = BUFFER_END_OF_FILE;
                buf_2_length++;
                buffer_2[buf_2_length] = ' ';
                buffer_2[buf_2_length + 1] = ' ';
                is_end_of_file = 1;
            }
            buf_2_length++;//指向最后一个字符的后面一个位置
            //开始解析缓冲区
            //只能复用这些全局变量了
            input_length = buf_2_length;
            input_position = -1;
            word_present = &buffer_2[0];
            word_present--;
            token_start = word_present;
            state_present_stdin = START_LEXER;
            next_buffer((char**)(&buffer_2), &tokenListt2);
            //现在应该写完一个tokenList2了。
            //现在应该停止词法分析，直到next_token读到end_of_tokens或者end_of_file
            frontToken=tokenList2;
            return 2;
        }
    }
    printf("In total :%lld tokens\n", token_count);
    system("pause");
    free(tokenList1);
    free(tokenList2);
    return 0;   //EOF了
}


