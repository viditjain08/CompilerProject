#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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


typedef union{
	int term_index;
	int non_term_index;
} t_or_nt;

struct tk_node{
	enum {T, NT} type;
    t_or_nt info;
    TK_NODE next;
} ;

struct rule{
    TK_NODE start;
    RULE next;
};

struct nont_block{
	char* name;
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
	char** terminals;
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
