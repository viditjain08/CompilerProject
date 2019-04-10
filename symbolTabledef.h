#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "astDef.h"

typedef enum {INT, REAL} dataType;
typedef struct field field;
typedef struct field* FIELD;
typedef struct symbolentry symbolentry;
typedef struct symbolentry* SYMBOLENTRY;
typedef struct symboltable symboltable;
typedef struct symboltable* SYMBOLTABLE;
typedef struct hashsymbol hashsymbol;
typedef struct hashsymbol* HASHSYMBOL;

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

#include "symbolTable.h"
