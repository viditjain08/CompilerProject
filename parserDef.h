/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"

typedef struct tk_node * TK_NODE;
typedef struct tk_node tk_node;
typedef struct grammar * GRAMMAR;
typedef struct grammar grammar;
typedef struct rule * RULE;
typedef struct rule rule;
typedef struct firstfollow * FirstFollow;
typedef struct firstfollow firstfollow;
typedef struct followds * FOLLOWDS;
typedef struct followds followds;
typedef struct followind * FOLLOWIND;
typedef struct followind followind;
typedef int * parsetable;
typedef parsetable * PARSETABLE;
typedef struct tree_node * TREE_NODE;
typedef struct tree_node tree_node;
typedef struct hashT  *Hashtable;

typedef enum {T, NT} ntt;
struct tk_node {
	ntt type;
	int info;
	TK_NODE next;
};

struct rule {
	int key;
	TK_NODE start;
};


struct followind {
	TK_NODE tk;
	int index;
};

struct followds {
	FOLLOWIND f;
	int size;
};

struct grammar {
	FOLLOWDS follow;
	RULE nonterminals;
	int* terminals;
	int nonterminal_count;
	int terminal_count;
	int rules;
};

struct firstfollow {
	// first 0th column eps, rest terminals as in grammar
	int** first;
	// follow 0th column $, rest terminals as in grammar
	int** follow;
};

typedef union {
	TOKENINFO tk;
	int index;
} node_info;

struct tree_node {

	ntt type;
	node_info tk_info;
	int parent_index;
	TREE_NODE child;
	TREE_NODE next;
	int rule_index;
};

struct hashT {
	char *name;
	int value;
	int index;
};

#include "parser.h"
