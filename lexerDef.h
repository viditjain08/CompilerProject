#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct tokenInfo * TOKENINFO;
typedef struct tokenInfo tokenInfo;

#define BUFFER_SIZE 32
#define MAX_LEXEME_SIZE 50

#ifndef TOKEN
#define TOKEN 23
typedef enum{
    EPS,
    TK_MAIN,
    TK_END,
    TK_FUNID,
    TK_SEM,
    TK_INPUT,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_OUTPUT,
    TK_ID,
    TK_INT,
    TK_REAL,
    TK_RECORD,
    TK_RECORDID,
    TK_COMMA,
    TK_ENDRECORD,
    TK_TYPE,
    TK_COLON,
    TK_FIELDID,
    TK_GLOBAL,
    TK_ASSIGNOP,
    TK_DOT,
    TK_CALL,
    TK_WITH,
    TK_PARAMETERS,
    TK_WHILE,
    TK_OP,
    TK_CL,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ELSE,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_MUL,
    TK_DIV,
    TK_PLUS,
    TK_MINUS,
    TK_NUM,
    TK_RNUM,
    TK_NOT,
    TK_AND,
    TK_OR,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_RETURN,
    TK_ERROR
}tokenType;
extern char *tokenMap[];
#endif


typedef union{
    int valI;
    float valF;
}Value;

struct tokenInfo{
    tokenType token;
    char* lexeme;
    Value* val;
    int lineNo;
    int dataType;   // 0 for int, 1 for float, 2 for identifier, 3 for reserved keyword, 4 for erorr
};

extern char* bufferCurr;
extern char* bufferPre;

extern int currChar;
extern int fileEnd;
extern int lineNo;

#include "lexer.h"
#include "hashTable.h"
