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

typedef union{
	int term_index;
	NONT_BLOCK non_term;
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

struct grammar{
	char** terminals;
	NONT_BLOCK nonterminals;
	int non_t_count;
	int t_count;
};

struct firstfollow{
	int** first;
	int** follow;
};
