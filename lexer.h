#include <stdio.h>
#include <stdlib.h>

void getStream(FILE *fp);

void removeComments(const char* testCase, const char* outfile);

tokenInfo* getNextToken(FILE *fp);
tokenInfo* getTKinfo(int token,char* lexeme,int lineNo);
void printTokens(const char *source_Code);
char *createLexeme(int j, int flag);
