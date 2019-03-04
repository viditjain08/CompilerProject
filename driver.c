#include "parser.h"
#include <time.h>
// #include "lexer.h"
#define HASHSIZE 300

int main(int argc, char *argv[]) {

    char* file = argv[1];
    clock_t    start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
  // //

	printf("-------FIRST AND FOLLOW SET AUTOMATED----------");
	printf("\n------Both lexical and syntax modules implemented--------");


	// printf("%d\n",((g->follow)[2].size));
	// for(int i=0;i<50;i++) {
	//     printf("%s %d\n",(g->terminals)[i], i);
	// }
    Hashtable tb_nt = hashTableInit(HASHSIZE);
    Hashtable tb_t = hashTableInit(HASHSIZE);
	GRAMMAR g = populateGrammar("grammar.txt", tb_nt, tb_t);
	FirstFollow f = ComputeFirstAndFollowSets(g, tb_nt, tb_t);
	PARSETABLE t;

	t = createParseTable(f, g, t, tb_nt, tb_t);
    // traversal(t);
	// char file[] = "testcases/testcase2.txt";
	printf("\n\n\n");


	hashInit(15);
while(1)
{

	int choice;
	printf("Enter choice -\n 0-Exit \n 1-Remove comments \n2-Print tokens generated\n 3-Parse \n 4-Time taken\n\n\n");
	scanf("%d", &choice);
	if(choice == 0)
	{
		exit(0);
	}
	else if(choice == 1)
	{
//	printTable();

//	printf("%s\n", tokenMap[lookup("parameters")]);

	char outFile[] = "output.txt";
	removeComments(file,outFile);
	}
	else if (choice == 2)
	{

		printTokens(file);
	}
	else if (choice ==3)
	{
		TREE_NODE x = parseInputSourceCode(file, t, f, g, tb_nt, tb_t);
	}
	else if(choice ==4)
	{

        start_time = clock();

		printTokens(file);
		TREE_NODE x = parseInputSourceCode(file, t, f, g, tb_nt, tb_t);
		end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
             // Print both total_CPU_time and total_CPU_time_in_seconds
                printf(" Total_CPU_time_in_seconds is %.8lf\n",total_CPU_time_in_seconds);
		printf("Total_CPU_time is %f\n",total_CPU_time);

	}
	else
	{
		printf("\n Invalid Choice");
	}



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
  /*  for(int i=0;i<g->t_count;i++) {
        if(t[9][i]!=NULL) {
            printf("%d\n",i);
        }
    }*/
}
    return 0;
}
