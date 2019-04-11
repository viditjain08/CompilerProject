#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "astDef.h"

typedef enum {INT, REAL, BOOL, RECORD, ERROR} dataType;
typedef struct field field;
typedef struct field* FIELD;
typedef struct symbolentry symbolentry;
typedef struct symbolentry* SYMBOLENTRY;
typedef struct symboltable symboltable;
typedef struct symboltable* SYMBOLTABLE;
typedef struct hashsymbol hashsymbol;
typedef struct hashsymbol* HASHSYMBOL;
typedef struct fn_entry fn_entry;
typedef struct fn_entry* FN_ENTRY;
typedef struct fn_stack fn_stack;
typedef struct fn_stack* FN_STACK;

typedef union {
	int ival;
	float rval;
} VAL;

struct field {
    dataType dType;
    int set:1;
	char* fieldname;
    VAL value;
	FIELD next;
};

struct symbolentry {
    int width;
    int offset;
	char* record_name;
    char* id;
    int int_no;
	int real_no;
    FIELD record;
    int init:1;
    SYMBOLENTRY next;
};

struct symboltable {
    int no_entries;
    char* name;
    SYMBOLENTRY head;
};

struct hashsymbol {
    char* function;
    char* id;
    char* fieldid;
    SYMBOLTABLE table_ptr;
    SYMBOLENTRY entry_ptr;
    FIELD field_ptr;
	int flag;
};

struct fn_entry{
	char* function_name;
	int index;
	NODE_AstTree ptr;
};

struct fn_stack {
	char* function_name;
	SYMBOLTABLE symboltable;
	FN_STACK next;
};
#include "symbolTable.h"
