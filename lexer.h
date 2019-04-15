
/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include <stdio.h>
#include <stdlib.h>

void getStream(FILE *fp);

void removeComments(const char* testCase);

tokenInfo* getNextToken(FILE *fp);
tokenInfo* getTKinfo(int token,char* lexeme,int lineNo);
void printTokens(const char *source_Code);
char *createLexeme(int j, int flag);
