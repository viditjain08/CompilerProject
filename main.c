// #include"astDef.h"
#include"semanticAnalyzerDef.h"
// #include "parserDef.h"

#define HASHSIZE 300

extern Hashtable non_termainals;
extern Hashtable terminals;
int no_of_lines=0;
char** errors;
HASHSYMBOL h;
int hash_size;
FN_ENTRY functions;
int fn_size;

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
	TREE_NODE x = parseInputSourceCode("semantic/testcase3.txt",t,f,g);
	// traversal(parsetree, g, x);
	fclose(parsetree);

	NODE_AstTree tree = buildAST(x);
	printf("\n" );
	printf("\n" );
	printf("------AST------\n" );
	printf("\n" );
	traverseAST(tree);
	printf("\n" );
	printf("\n" );

	int nAst = countNodesAST(tree);
	int nPt = countNodesParseTree(x);
	errors = (char**)malloc(sizeof(char*)*no_of_lines);
	int i;
	for ( i = 0; i < no_of_lines; i++) {
		errors[i] = NULL;
	}
	printf("\n");
	printf("Number of Nodes in Ast: %d\n",nAst);
	printf("Number of Nodes in parsetree: %d\n",nPt);
	printf("Compression Ratio: %.2f\n",(1-((float)nAst/nPt))*100 );
	HASHSYMBOL h = populateSymbolTable(tree);

	nAst = countNodesAST(tree);

	printf("Number of Nodes in Ast: %d\n",nAst);
	printf("Number of Nodes in parsetree: %d\n",nPt);
	printf("Compression Ratio: %.2f\n",(1-((float)nAst/nPt))*100 );

	printf("\n" );
	printf("\n" );
	printf("\n" );



	semAnalyze(tree);

	printf("\t----------Semantic Errors---------\n" );
	printf("\n" );
	printErrors(no_of_lines);
	printf("\n" );
	printf("\n" );

	return 0;
}
