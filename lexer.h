//
// Created by W on 2024/11/3.
//

#ifndef LEXER_SUBPROGRAM_LEXER_H
#define LEXER_SUBPROGRAM_LEXER_H

struct token* next_token();
struct token* next_file();
struct token* next_file_all();
int next_stdin();
struct token* next_buffer(char** buffer, struct token** tokenList);
int lexer_analysis_file(char* address);

int start_lexer();
int identifier_1();
int identifier_2();
int identifier_3();
int identifier_4();
int identifier_5();
int decimal_number_1();
int decimal_number_2();
int decimal_number_3();
int decimal_number_4();
int decimal_number_5();
int octal_number_1();
int octal_number_2();
int octal_number_3();
int octal_number_4();
int octal_number_5();
int octal_number_6();
int hexadecimal_number_1();
int hexadecimal_number_2();
int hexadecimal_number_3();
int hexadecimal_number_4();
int hexadecimal_number_5();
int hexadecimal_number_6();
int hexadecimal_number_7();
int operator_1();
int operator_2();
int keyword_if_1();
int keyword_if_2();
int keyword_while_1();
int keyword_while_2();
int keyword_while_3();
int keyword_while_4();
int keyword_while_5();
int keyword_then_1();
int keyword_then_2();
int keyword_then_3();
int keyword_then_4();
int keyword_else_1();
int keyword_else_2();
int keyword_else_3();
int keyword_else_4();
int keyword_do_1();
int keyword_do_2();
int skip_to_next_valid_char();
int skip_to_next_space();
int print_token();
int add_token();
int print_token_std(struct token* token);
int next_token_a_new_file(FILE * fp);
int load_buffers(FILE *fp,struct token *tokenList1,struct token *tokenList2);

//预处理 不对，其实“预处理已经是外面的函数了”
int preprocess_char();
int add_space_beside_operator_separator();


#endif //LEXER_SUBPROGRAM_LEXER_H
