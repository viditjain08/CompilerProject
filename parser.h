/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include<stdio.h>
#include <stdlib.h>

void hashTableInit(int size);
GRAMMAR populateGrammar(char* grammar_file);
FirstFollow ComputeFirstAndFollowSets(GRAMMAR G);
PARSETABLE createParseTable(FirstFollow F, GRAMMAR G, PARSETABLE PT);
TREE_NODE addRule(int r_index, TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g);
TREE_NODE buildParseTree(TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g);
TREE_NODE parseInputSourceCode(char *testcaseFile, PARSETABLE pt, FirstFollow f, GRAMMAR g);
void traversal(GRAMMAR g, TREE_NODE tree);
