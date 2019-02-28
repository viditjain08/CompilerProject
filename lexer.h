#include <stdio.h>
#include <stdlib.h>

void getStream(FILE *fp);

void removeComments(const char* testCase, const char* outfile);

tokenType getNextToken(FILE *fp);
void printTokens(char* testCase);
