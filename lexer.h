#include <stdio.h>
#include <stdlib.h>

void getStream();
tokenInfo* getNextToken();
void removeComments(const char* testCase, const char* outfile);
void printTokens(char* testCase);
