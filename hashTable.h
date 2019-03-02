#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#ifndef TOKEN
#define TOKEN 23

typedef enum{
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RECORDID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_ERROR
}tokenType;
extern char *tokenMap[];

#endif

struct node{
	char* key;
	tokenType token;
	struct node* next;
};
typedef struct node* Node ;

struct list{
	Node head;
};
typedef struct list* List;

struct hashTable
{
	List rows;
	int no_rows;
};

void hashInit(int no);
void addEntry(char* key,tokenType token);
int hashFunc(char* key,int no);
int lookup(char* key);
void printTable();
void printList(struct list ls);

 extern struct hashTable* keywordTable;
