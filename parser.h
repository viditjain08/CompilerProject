#include "parserDef.h"
#include "lexerDef.h"

GRAMMAR populateGrammar(char* grammar_file);
FirstFollow ComputeFirstAndFollowSets(GRAMMAR G);
PARSETABLE createParseTable(FirstFollow F, GRAMMAR G, PARSETABLE PT);
