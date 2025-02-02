/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include "hashTable.h"
struct hashTable* keywordTable = NULL;
char *tokenMap[] = {
    "EPS",
    "TK_MAIN",
    "TK_END",
    "TK_FUNID",
    "TK_SEM",
    "TK_INPUT",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_OUTPUT",
    "TK_ID",
    "TK_INT",
    "TK_REAL",
    "TK_RECORD",
    "TK_RECORDID",
    "TK_COMMA",
    "TK_ENDRECORD",
    "TK_TYPE",
    "TK_COLON",
    "TK_FIELDID",
    "TK_GLOBAL",
    "TK_ASSIGNOP",
    "TK_DOT",
    "TK_CALL",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_WHILE",
    "TK_OP",
    "TK_CL",
    "TK_ENDWHILE",
    "TK_IF",
    "TK_THEN",
    "TK_ELSE",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_MUL",
    "TK_DIV",
    "TK_PLUS",
    "TK_MINUS",
    "TK_NUM",
    "TK_RNUM",
    "TK_NOT",
    "TK_AND",
    "TK_OR",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "TK_RETURN",
    "TK_ERROR",
    "TK_ERROR2",
    "TK_ERROR3"
};


void hashInit(int no){
	keywordTable = (struct hashTable*)malloc(sizeof(struct hashTable));
	keywordTable->no_rows = no;
	keywordTable->rows = (List)malloc(sizeof(struct list)*no);
	int i = 0;
	for(i;i<no;i++)
	{
		keywordTable->rows[i].head = NULL;
	}

	// adding all keywords
	addEntry("with",TK_WITH);
	addEntry("parameters",TK_PARAMETERS);
	addEntry("end",TK_END);
	addEntry("while",TK_WHILE);
	addEntry("type",TK_TYPE);
	addEntry("global",TK_GLOBAL);
	addEntry("parameter",TK_PARAMETER);
	addEntry("list",TK_LIST);
	addEntry("input",TK_INPUT);
	addEntry("output",TK_OUTPUT);
	addEntry("int",TK_INT);
	addEntry("real",TK_REAL);
	addEntry("endwhile",TK_ENDWHILE);
	addEntry("if",TK_IF);
	addEntry("then",TK_THEN);
	addEntry("endif",TK_ENDIF);
	addEntry("read",TK_READ);
	addEntry("write",TK_WRITE);
	addEntry("return",TK_RETURN);
	addEntry("call",TK_CALL);
	addEntry("record",TK_RECORD);
	addEntry("endrecord",TK_ENDRECORD);
	addEntry("else",TK_ELSE);

}

void addEntry(char* key,tokenType token)
{
	int index = hashFunc(key,keywordTable->no_rows);
	//printf("index while adding is %d \n",index);

	Node tmp = (Node)malloc(sizeof(struct node));
	tmp->key = (char*)malloc(sizeof(char)*(strlen(key)+1));
	strcpy(tmp->key,key);
	tmp->key[strlen(key)] = '\0';
	tmp->token = token;
	tmp->next = keywordTable->rows[index].head;
	keywordTable->rows[index].head = tmp;

}

int hashFunc(char* key,int no){
	int length = 0;
	while(key[length] != '\0')
	length++;

	int i = 0;
	long long value = 0;
	for(i;i<length;i++)
	{
		value = 53*value + key[i];
	}
	value = value % no;

	return value;
}

int lookup(char* key)
{
	int index = hashFunc(key,keywordTable->no_rows);
	Node temp = keywordTable->rows[index].head;
	while(temp != NULL)
	{
		if(strcmp(temp->key,key) == 0){
			return temp->token;
		}
		temp = temp->next;
	}
	return -1;
}
void printList(struct list ls){
	Node tmp = ls.head;

	while(tmp != NULL){
		printf("%-20s\t\t%-20s\n",tmp->key,tokenMap[tmp->token] );
		tmp = tmp->next;
	}
}

void printTable(){
	int i = 0;
	int n = keywordTable->no_rows;

	for (i =0;i<n;i++){
        printf("Bucket %d\n", i+1);
		printList(keywordTable->rows[i]);
		printf("\n");
		printf("\n");
	}
}

// int main(int argc, char const *argv[]) {
//     hashInit(23);
//     printTable();
//     return 0;
// }
