#include "parserDef.h"

GRAMMAR populateGrammar(char* grammar_file);
FirstFollow ComputeFirstAndFollowSets(GRAMMAR G);
PARSETABLE createParseTable(FirstFollow F, GRAMMAR G, PARSETABLE PT);
TREE_NODE parseInputSourceCode(char *testcaseFile, PARSETABLE pt, FirstFollow f, GRAMMAR g);
