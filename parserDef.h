#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct tk_node * TK_NODE;
typedef struct rule * RULE;
typedef struct tk_node tk_node;
typedef struct rule rule;
typedef struct grammar * GRAMMAR;
typedef struct grammar grammar;
typedef struct hash_block * HASH_BLOCK;
typedef struct hash_block hash_block;

#define grammar_len 50
#define HASH_SIZE 300
// typedef enum{
// 	program,
// 	mainFunction,
// 	otherFunctions,
// 	function,
// 	input_par,
// 	output_par,
// 	parameter_list,
// 	dataType,
// 	primitiveDatatype,
// 	constructedDatatype,
// 	remaining_list,
// 	stmts,
// 	typeDefinitions,
// 	typeDefinition,
// 	fieldDefinitions,
// 	fieldDefinition,
// 	moreFields,
// 	declarations,
//     declaration,
// 	global_or_not,
// 	otherStmts,
// 	stmt,
// 	assignmentStmt,
// 	singleOrRecId,
//     new_24,
// 	funCallStmt,
// 	outputParameters,
// 	inputParameters,
// 	iterativeStmt,
// 	conditionalStmt,
//     elsePart,
// 	ioStmt,
// 	allVar,
// 	arithmeticExpression,
// 	expPrime,
//     term,
// 	termPrime,
//     factor,
//     highPrecedenceOperator,
//     lowPrecedenceOperators,
//     all,
//     temp,
// 	booleanExpression,
// 	var,
// 	logicalOp,
// 	relationalOp,
// 	returnStmt,
// 	optionalReturn,
// 	idList,
// 	more_ids,
// } non_term;

typedef union{
	char* term_name;
	HASH_BLOCK non_term;
} t_or_nt;

struct tk_node{
    t_or_nt info;
    TK_NODE next;
} ;

struct rule{
    TK_NODE start;
    RULE next;
};

struct hash_block{
		char* name;
		RULE r;
		HASH_BLOCK next;
};

struct grammar{
	HASH_BLOCK hash_table[HASH_SIZE];
};
