#include<stdio.h>
#include <stdlib.h>

void hashTableInit(int size);
GRAMMAR populateGrammar(char* grammar_file);
FirstFollow ComputeFirstAndFollowSets(GRAMMAR G);
PARSETABLE createParseTable(FirstFollow F, GRAMMAR G, PARSETABLE PT);
TREE_NODE addRule(int r_index, TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g);
TREE_NODE buildParseTree(TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g);
TREE_NODE parseInputSourceCode(char *testcaseFile, PARSETABLE pt, FirstFollow f, GRAMMAR g);
void traversal(FILE* fp, GRAMMAR g, TREE_NODE tree);
