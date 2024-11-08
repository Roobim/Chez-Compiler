//
// Created by W on 2024/11/3.
//

#ifndef LEXER_SUBPROGRAM_TOKEN_H
#define LEXER_SUBPROGRAM_TOKEN_H

#define MAX_TOKEN_LENGTH 217            //永远的解放路217号...今日份emo
#define MAX_WORD_LENGTH  200               //这个是一个token的最大长度
enum token_type {
    identifier = 0, decimalNumber, octalNumber, hexadecimalNumber, errorType, endOfTokens, endOfFile, keyword, operatorr
};

struct token;
struct token {
    enum token_type type;       //单词的类型。
    char value[MAX_WORD_LENGTH];                //识别的单词字面值。 这个可以写union来节省空间，但是有必要？
};



#endif //LEXER_SUBPROGRAM_TOKEN_H
