#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexerDef.h"

typedef struct tk_node * TK_NODE;
typedef struct rule * RULE;
typedef struct tk_node tk_node;
typedef struct rule rule;
typedef struct grammar * GRAMMAR;
typedef struct grammar grammar;
typedef struct nont_block * NONT_BLOCK;
typedef struct nont_block nont_block;
typedef struct firstfollow * FirstFollow;
typedef struct firstfollow firstfollow;
typedef struct followds * FOLLOWDS;
typedef struct followds followds;
typedef struct followind * FOLLOWIND;
typedef struct followind followind;
typedef RULE * parsetable;
typedef parsetable * PARSETABLE;
typedef struct tree_node * TREE_NODE;
typedef struct tree_node tree_node;
typedef struct hashT  *Hashtable;

typedef enum {T, NT} ntt;
struct tk_node{
	ntt type;
    int info;
    TK_NODE next;
} ;

struct rule{
    TK_NODE start;
    RULE next;
};

struct nont_block{
	int nt_index;
	RULE r;
};


struct followind {
	TK_NODE tk;
	int index;
};

struct followds {
	FOLLOWIND f;
	int size;
};

struct grammar{
	FOLLOWDS follow;
	int* terminals;
	NONT_BLOCK nonterminals;
	int non_t_count;
	int t_count;
};

struct firstfollow{
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
};

struct hashT{
 char *name;
 int index;
 int flag;
};
