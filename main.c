#include <stdio.h>
#include "token.h"
#include "lexer.h"
//已知的没有改的BUG:
//1 这玩意其实跑在一个编译器特性(?)上 struct token的成员char[]默认会给初始化成'\0'
//  如果没有的话会导致缓冲区读完之后会写到数组溢出然后被操作系统kill
//  具体bug参见 int add_token()
int main() {
    char *c="D:\\Chez_Moi\\C\\Codefield\\ClionProjects\\Compiler\\lab01\\ceshi.txt";
    //先在这里打开文件 然后调用一次int next_token_a_new_file(FILE * fp);
    //然后调用next_token
    //直到返回EOF 在这里关闭文件
    FILE *fp;
    fp= fopen(c,"r");
    if (fp == NULL) {
        printf("file not found\n");
        return 0;
    }
    next_token_a_new_file(fp);
    struct token *receiveToken;
    int pos=0;
    while(1) {
        receiveToken = next_token();
        pos=0;
        if(receiveToken->type==endOfFile||receiveToken==NULL){
            break;
        }
        else{
            printf("<");
            while (receiveToken->value[pos] != '\0') {
                putchar(receiveToken->value[pos]);
                pos++;
            }
            printf(">,%d\n", receiveToken->type);

        }
    }
    return 0;
}
