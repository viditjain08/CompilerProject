#include "parser.h"
// #include "lexer.h"
int main() {

	// GRAMMAR g = populateGrammar("grammar.txt");
	// // printf("%d\n",((g->follow)[2].size));
	// // for(int i=0;i<54;i++) {
	// //     printf("%s %d\n",(g->terminals)[i], i);
	// // }
	//
	// FirstFollow f = ComputeFirstAndFollowSets(g);
	// PARSETABLE t;
	// t = createParseTable(f, g, t);

    hashInit(15);
	printTable();

	printf("%s\n", tokenMap[lookup("parameters")]);

	char file[] = "source_code";
	char outFile[] = "output.txt";

	//removeComments(file,outFile);

	printTokens(file);

	// int i;
	// for(i=0;i<g->t_count;i++){
	// 	printf("%s\n", g->terminals[i]);
	// }

    // for(int i=0;i<g->non_t_count;i++) {
    //     printf("%s\n",(g->nonterminals)[i].name);
    //     for(int j=0;j<g->t_count;j++) {
    //         if(t[i][j]!=NULL) {
    //             printf("%s ",(g->terminals)[j]);
    //         }
    //     }
    //     printf("\n");
    //
    // }
    // for(int i)
    return 0;
}
