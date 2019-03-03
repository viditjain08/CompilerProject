#include <stdio.h>
#include <stdlib.h>

void getStream(FILE *fp);

void removeComments(const char* testCase, const char* outfile);

TOKENINFO getNextToken(FILE *fp);
TOKENINFO getTKinfo(int token,char* lexeme,int lineNo);
void printTokens(const char *source_Code);
