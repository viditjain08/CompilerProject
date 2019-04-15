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
