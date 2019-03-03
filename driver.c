#include "lexerDef.h"
// #include "lexer.h"
int main(){
	hashInit(15);
	printTable();

	// printf("%s\n", tokenMap[lookup("parameters")]);

	char file[] = "source_code";
	char outFile[] = "output.txt";

	//removeComments(file,outFile);

	printTokens(file);

	return 0;
}
