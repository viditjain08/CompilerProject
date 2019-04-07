#include"astDef.h"
// #include "parserDef.h"

#define HASHSIZE 300

extern Hashtable non_termainals;
extern Hashtable terminals;

int main (int argc, char* argv[]){
	// char *file = argv[1];
	// char *parsetreefile = argv[2];
	// printf("%s\n", file);
	hashInit(15);
	hashTableInit(HASHSIZE);
	GRAMMAR g = populateGrammar("grammar.txt");
	FirstFollow f = ComputeFirstAndFollowSets(g);
	PARSETABLE t = createParseTable(f,g,t);

	FILE *parsetree = fopen("parsetreeOutFile.txt","w+");
	TREE_NODE x = parseInputSourceCode("t1.txt",t,f,g);
	traversal(parsetree, g, x);
	fclose(parsetree);
	NODE_AstTree tree = buildAST(x);
	return 0;
}
