#include "parserDef.h"

Hashtable hashTableInit(int size);
GRAMMAR populateGrammar(char* grammar_file, Hashtable tb_nt, Hashtable tb_t);
FirstFollow ComputeFirstAndFollowSets(GRAMMAR G, Hashtable tb_nt, Hashtable tb_t);
PARSETABLE createParseTable(FirstFollow F, GRAMMAR G, PARSETABLE PT, Hashtable tb_nt, Hashtable tb_t );
TREE_NODE parseInputSourceCode(char *testcaseFile, PARSETABLE pt, FirstFollow f, GRAMMAR g, Hashtable tb_nt, Hashtable tb_t);
// void traversal(TREE_NODE tree);
