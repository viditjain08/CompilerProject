/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parserDef.h"

typedef struct list_TokenInfo List_TokenInfo;
typedef List_TokenInfo *LIST_TokenInfo;

struct list_TokenInfo{
    TOKENINFO tk;
    LIST_TokenInfo next;
};

typedef struct node_AstTree Node_AstTree;
typedef Node_AstTree *NODE_AstTree;

struct node_AstTree{
    LIST_TokenInfo tokens;
    NODE_AstTree parent;
    NODE_AstTree child;
    NODE_AstTree sibling;
};

#include "ast.h"
