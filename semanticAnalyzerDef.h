/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/

#include <stdlib.h>
#include <stdio.h>

#include "typeChecker.h"

typedef struct vList varList;
typedef varList *VARLIST;

struct vList{
    NODE_AstTree v;
    VARLIST next;
};

#include "semanticAnalyzer.h"
