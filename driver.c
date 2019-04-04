#include "parser.h"
#include <time.h>
// #include "lexer.h"
#define HASHSIZE 300

int main(int argc, char *argv[]) {

    char* file = argv[1];
    char* parsetreefile = argv[2];

    // char* file = "testcases/testcase4.txt";
    // char* parsetreefile = "parsetreeOutFile.txt";
    clock_t    start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

	printf("-------FIRST AND FOLLOW SET AUTOMATED----------\n");
	printf("------Both lexical and syntax modules implemented--------\n");


    hashInit(15);

    Hashtable tb_nt = hashTableInit(HASHSIZE);
    Hashtable tb_t = hashTableInit(HASHSIZE);
	GRAMMAR g = populateGrammar("grammar.txt", tb_nt, tb_t);
	FirstFollow f = ComputeFirstAndFollowSets(g, tb_nt, tb_t);
	PARSETABLE t;

	t = createParseTable(f, g, t, tb_nt, tb_t);
while(1)
{

	int choice;
	printf("\nEnter choice -\n 0-Exit \n 1-Remove comments \n 2-Print tokens generated\n 3-Parse \n 4-Time taken\n");
	scanf("%d", &choice);
	if(choice == 0)
	{
		exit(0);
	}
	else if(choice == 1)
	{
//	printTable();

//	printf("%s\n", tokenMap[lookup("parameters")]);

	removeComments(file);
    printf("Comments removed\n");
	}
	else if (choice == 2)
	{

		printTokens(file);
	}
	else if (choice ==3)
	{

        FILE *parsetree = fopen(parsetreefile, "w+");

		TREE_NODE x = parseInputSourceCode(file, t, f, g, tb_nt, tb_t);

        if(invalid_prog==0) {
            printf("Program is syntactically correct\n");
        }
        invalid_prog=0;
        traversal(parsetree, g, x, tb_nt, tb_t);
        printf("Inorder Traversal printed in %s\n",parsetreefile);
        fclose(parsetree);
	}
	else if(choice ==4)
	{

        start_time = clock();

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

}
    return 0;
}
