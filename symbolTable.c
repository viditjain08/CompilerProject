#include "symbolTabledef.h"
// #include "astDef.h"

int hash_jump=0;
extern SYMBOLTABLE record_table;
extern SYMBOLTABLE global_table;


FN_STACK stack_init(char* var_name, char* fieldname, SYMBOLTABLE s) {
	FN_STACK f = (FN_STACK)malloc(sizeof(fn_stack));
	f->var_name = (char*)malloc(sizeof(char)*(1+strlen(var_name)));
	strcpy(f->var_name,var_name);
	if(fieldname!=NULL) {
		f->field_name = (char*)malloc(sizeof(char)*(1+strlen(fieldname)));
		strcpy(f->field_name,fieldname);
	}

	f->next = NULL;
	f->symboltable = s;
	return f;
}

FN_STACK push(FN_STACK f, char* var_name, char* fieldname, SYMBOLTABLE s) {
	FN_STACK new = (FN_STACK)malloc(sizeof(fn_stack));
	new->var_name = (char*)malloc(sizeof(char)*(1+strlen(var_name)));
	strcpy(new->var_name,var_name);
	if(fieldname!=NULL) {
		new->field_name = (char*)malloc(sizeof(char)*(1+strlen(fieldname)));
		strcpy(new->field_name,fieldname);
	}
	new->next = f;
	new->symboltable = s;
	return new;
}

FN_STACK pop(FN_STACK f) {
	if(f==NULL) {
		return NULL;
	}
	FN_STACK temp = f->next;
	free(f);
	return temp;
}

void functionsInit() {
	functions = (FN_ENTRY)malloc(sizeof(fn_entry)*fn_size);
	for(int i=0; i<fn_size; i++) {
		functions[i].function_name = NULL;
		functions[i].ptr = NULL;
	}
}

int enterFunction(char* function, int index, SYMBOLTABLE st, NODE_AstTree fn_node) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = (hashVal + 7*(int)function[i])%fn_size;
		i++;
	}
	hashVal = hashVal%fn_size;
	for(int hv=1; hv<=10; hv++) {
		if(functions[hashVal].ptr==NULL) {
			functions[hashVal].function_name = (char*)malloc(sizeof(char)*(strlen(function)+1));
			strcpy(functions[hashVal].function_name,function);
			functions[hashVal].ptr = fn_node;
			functions[hashVal].index = index;
			functions[hashVal].st = st;
			return hashVal;
		}
		hashVal += (hv*hv);
		hashVal = hashVal%fn_size;


	}
	return -1;
}

int getFunction(char* function) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = (hashVal + 7*(int)function[i])%fn_size;
		i++;
	}
	hashVal = hashVal%fn_size;
	for(int hv=1; hv<=10; hv++) {
		if(functions[hashVal].ptr==NULL) {
			return -1;
		}
		if(strcmp(functions[hashVal].function_name, function)==0) {
			return hashVal;
		}
		hashVal += (hv*hv);
		hashVal = hashVal%fn_size;

	}
	return -1;
}

void hashSymbolInit() {
	h = (HASHSYMBOL)malloc(sizeof(hashsymbol) * hash_size);
	for(int i=0; i<hash_size; i++) {
		h[i].function = NULL;
		h[i].fieldid = NULL;
		h[i].id = NULL;
		h[i].entry_ptr = NULL;
		h[i].table_ptr = NULL;
		h[i].field_ptr = NULL;
		h[i].flag = 0;
	}
}

SYMBOLTABLE symbolTableinit(char* name) {
	SYMBOLTABLE s = (SYMBOLTABLE)malloc(sizeof(symboltable));
	s->head = NULL;
	s->no_entries = 0;
	s->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
	strcpy(s->name,name);
	return s;
}

SYMBOLENTRY symbolEntryinit(int width, int offset, char* lexeme, int int_no, int real_no, FIELD record) {
	SYMBOLENTRY s = (SYMBOLENTRY)malloc(sizeof(symbolentry));
	s->width = width;
	s->offset = offset;
	s->id = (char*)malloc(sizeof(char)*(strlen(lexeme)+1));
	strcpy(s->id,lexeme);
	s->int_no = int_no;
	s->real_no = real_no;
	s->record = record;
	s->next = NULL;
	s->init = 0;
	s->record_name = NULL;
	return s;
}

FIELD fieldinit(tokenType tt, VAL value, char* name) {
	FIELD f = (FIELD)malloc(sizeof(field));
	f->set = 0;
	if(tt==TK_INT) {
		f->dType = INT;
		f->value.ival = value.ival;
	} else {
		f->dType = REAL;
		f->value.rval = value.rval;
	}
	f->next = NULL;
	f->fieldname = (char*)malloc(sizeof(char)*(strlen(name)+1));
	strcpy(f->fieldname,name);
	return f;
}
int computeHash(char* function, char* id, char* fieldid) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = hashVal + 7*(int)function[i];
		i++;
	}
	hashVal = hashVal%hash_size;
	i=0;
	while(id[i]!='\0') {
		hashVal = hashVal + 17*(int)id[i];
		i++;
	}
	hashVal = hashVal%hash_size;
	i=0;
	if(fieldid!=NULL) {
		while(fieldid[i]!='\0') {
			hashVal = hashVal + 37*(int)fieldid[i];
			i++;
		}
		hashVal = hashVal%hash_size;
	}
	return hashVal;
}

int hashSymbolEntry(SYMBOLTABLE t, SYMBOLENTRY e, FIELD f, char* function, char* id, char* fieldid){

	long long int hashval = computeHash(function, id, fieldid), hv=1;

	while(h[hashval].flag==1) {
		// if(strcmp(h[hashval].function,function)==0 && strcmp(h[hashval].id,id)==0) {
		//  if((fieldid!=NULL && strcmp(h[hashval].fieldid,fieldid)==0)||(fieldid==NULL && h[hashval].fieldid==NULL)) {
		//      return hashval;
		//  }
		// }
		hashval = (hashval + (hv*hv))%hash_size;
		hv++;
	}
	h[hashval].function = (char*)malloc(sizeof(char) * (strlen(function)+1));
	h[hashval].id = (char*)malloc(sizeof(char) * (strlen(id)+1));
	strcpy(h[hashval].function,function);
	strcpy(h[hashval].id, id);
	h[hashval].entry_ptr = e;
	h[hashval].table_ptr = t;
	h[hashval].flag = 1;
	if(fieldid!=NULL) {
		h[hashval].fieldid = (char*)malloc(sizeof(char) * (strlen(fieldid)+1));
		h[hashval].field_ptr = f;
		strcpy(h[hashval].fieldid,fieldid);
	} else {
		h[hashval].fieldid = NULL;
		h[hashval].field_ptr = NULL;
	}
	hash_jump = (hash_jump >= hv-1) ? hash_jump : hv-1;

	return hashval;
}
int lookupEntry(char *function, char *id, char *fieldid) {

	long long int hashval = computeHash(function, id, fieldid), hv=1;

	while(h[hashval].flag!=0) {

		if(h[hashval].flag==-1) {
			hashval = (hashval + (hv*hv))%hash_size;
			hv++;
			if(hv > hash_jump+1) {
				break;
			}
			continue;
		}

		if(strcmp(h[hashval].function,function)==0 && strcmp(h[hashval].id,id)==0) {
			if((fieldid!=NULL && strcmp(h[hashval].fieldid,fieldid)==0)||(fieldid==NULL && h[hashval].fieldid==NULL)) {
				return hashval;
			}
		}
		hashval = (hashval + (hv*hv))%hash_size;
		hv++;
		if(hv > hash_jump+1) {
			break;
		}
	}

	return -1;
}

int lookupRecord(char *record_name) {

	long long int hashval = computeHash("record", record_name, NULL), hv=1;

	while(h[hashval].flag!=0) {
		if(h[hashval].flag==-1) {
			hashval = (hashval + (hv*hv))%hash_size;
			hv++;
			if(hv > hash_jump+1) {
				break;
			}
			continue;
		}
		if(strcmp(h[hashval].function,"record")==0 && strcmp(h[hashval].entry_ptr->record_name,record_name)==0) {
			return hashval;
		}
		hashval = (hashval + (hv*hv))%hash_size;
		hv++;
		if(hv > hash_jump+1) {
			break;
		}
	}

	return -1;
}


int deallocate(NODE_AstTree x) {
	NODE_AstTree child = x->child;
	int y=0;
	while(child!=NULL) {
		NODE_AstTree next = child->sibling;
		int z = deallocate(child);
		y = (y>z) ? y : z;
		child = next;
	}
	if(x->tokens!=NULL) {
		y = (y>x->tokens->tk->lineNo) ? y : x->tokens->tk->lineNo;
	}
	free(x);
	return y;
}

void printSymbolTable(SYMBOLTABLE s) {
	SYMBOLENTRY temp = s->head;
	printf("Printing SYMBOLTABLE for %s with entries %d\n",s->name, s->no_entries);
	while(temp!=NULL) {
		// if(temp->record_name!=NULL){
		// 	printf("%s",temp->record_name);
		// }
		printf("%s %s %d\n",temp->id, s->name, temp->offset);
		temp=temp->next;
	}
}

SYMBOLENTRY addDeclarations(NODE_AstTree parent, SYMBOLTABLE st, int offset, SYMBOLENTRY y, int flag) {

	NODE_AstTree input_par = parent->child;
	NODE_AstTree prev = NULL;
	while(input_par!=NULL) {
		if((flag==0 && input_par->tokens->next->next!=NULL)||(flag==1 && input_par->tokens->next->next==NULL)) {
			printf("%s \n",input_par->tokens->next->tk->lexeme);

			prev = input_par;
			input_par = input_par->sibling;
			continue;
		}
		int hashval = lookupEntry(st->name, input_par->tokens->next->tk->lexeme, NULL);

		if(hashval!=-1) {
			if(prev==NULL) {
				parent->child=input_par->sibling;
			} else {
				prev->sibling = input_par->sibling;
			}
			input_par->sibling=NULL;
			char* name = input_par->tokens->next->tk->lexeme;
			int Line = input_par->tokens->tk->lineNo;
			int temp = deallocate(input_par);
			char e[200];
			sprintf(e, "Line %d: Variable %s already declared.\n", Line, name);
			printf("%s",e);
			errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
			strcpy(errors[Line],e);
			if(prev==NULL) {
				input_par = parent->child;
				prev = NULL;
			} else {
				// printf("%s \n",prev->sibling->tokens->next->tk->lexeme);
				input_par = prev->sibling;
			}

			continue;
		}


		SYMBOLENTRY s;
		if(input_par->tokens->tk->token==TK_INT) {
			TOKENINFO t = input_par->tokens->next->tk;
			VAL v;
			v.ival = -1;
			FIELD f = fieldinit(TK_INT, v, t->lexeme);
			s = symbolEntryinit(2, offset, t->lexeme, 1, 0, f);
			s->record = f;
			offset+=2;
			int hval = hashSymbolEntry(st, s, NULL, st->name, t->lexeme, NULL);

		} else if(input_par->tokens->tk->token==TK_REAL) {
			VAL v;
			v.rval = -1;
			TOKENINFO t = input_par->tokens->next->tk;
			FIELD f = fieldinit(TK_REAL, v,t->lexeme);
			s = symbolEntryinit(4, offset, t->lexeme, 0, 1, f);
			offset+=4;
			int hval = hashSymbolEntry(st, s, NULL, st->name, t->lexeme, NULL);

		} else {
			TOKENINFO t = input_par->tokens->next->tk;
			s = symbolEntryinit(0, offset, t->lexeme, 0, 0, NULL);
			FIELD temp_field=NULL;

			int hval = lookupRecord(input_par->tokens->tk->lexeme);
			if(hval==-1) {
				if(prev==NULL) {
					parent->child=input_par->sibling;
				} else {
					prev->sibling = input_par->sibling;
				}

				input_par->sibling=NULL;
				char* name = input_par->tokens->tk->lexeme;
				int Line = input_par->tokens->tk->lineNo;
				int temp = deallocate(input_par);
				char e[200];
				sprintf(e, "Line %d: No record %s found.\n", Line, name);
				printf("%s",e);
				errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
				strcpy(errors[Line],e);
				if(prev==NULL) {
					input_par = parent->child;
					prev = NULL;
				} else {
					input_par = prev->sibling;
				}
				free(s);
				continue;
			}
			int val = hashSymbolEntry(st, s, NULL, st->name, s->id, NULL);
			s->record_name = (char*)malloc(sizeof(char)*(strlen(input_par->tokens->tk->lexeme)+1));
			strcpy(s->record_name,input_par->tokens->tk->lexeme);

			FIELD f = h[hval].entry_ptr->record;
			int i=0,r=0;
			while(f!=NULL) {
				VAL v;
				v.ival = -1;
				FIELD temp;
				if(f->dType==INT) {
					temp = fieldinit(TK_INT, v, f->fieldname);
				} else {
					temp = fieldinit(TK_REAL, v, f->fieldname);
				}
				if(s->record==NULL) {
					s->record = temp;
					temp_field = temp;
				} else {
					temp_field->next = temp;
					temp_field = temp;
				}
				if(temp->dType==INT) {
					i++;
					offset+=2;
				} else {
					r++;
					offset+=4;
				}
				int hashval = hashSymbolEntry(st, s, temp, st->name, s->id, temp->fieldname);
				f=f->next;
			}

			s->int_no = i;
			s->real_no = r;
			s->width = 2*i+4*r;


		}
		if(st->head==NULL) {
			st->head = s;
			y = s;
			printf("Added to %s table\n",st->name);
		} else {
			y->next=s;
			y=s;
			printf("Added to previous %s entry\n",st->name);
		}
		st->no_entries++;
		prev = input_par;
		input_par=input_par->sibling;
	}
	return y;
}

void functionSymbolTable(NODE_AstTree function, SYMBOLTABLE st) {

	printf("Function: %s\n",st->name);

	int offset=0;
	NODE_AstTree stmts = function;
	SYMBOLENTRY y = NULL;
	if(strcmp(st->name,"_main")!=0) {
		stmts = function->child->sibling->sibling->sibling;
		NODE_AstTree node = function->child;
		node = node->sibling;
		y = addDeclarations(node, st, offset, y, -1);
		offset = y->offset+y->width;

		node = node->sibling;

		y = addDeclarations(node, st, offset, y, -1);
		offset = y->offset+y->width;
	}


	NODE_AstTree node = stmts->child->sibling;
	y = addDeclarations(node, st, offset, y, 0);

	printSymbolTable(st);
}



HASHSYMBOL populateSymbolTable(NODE_AstTree ast) {
	hash_size = 500;
	fn_size = 200;
	hashSymbolInit();
	NODE_AstTree main_node = ast->child;
	NODE_AstTree record_temp = main_node->sibling, prev_record = main_node;
	NODE_AstTree function = main_node->sibling;
	record_table = symbolTableinit("record");
	global_table = symbolTableinit("global");
	int index=0;
	// printSymbolTable(global_table);
	functionsInit();

	SYMBOLENTRY x,y;
	int flag=0;
	if(record_temp==NULL){
		flag=1;
		record_temp = main_node;
	}
	while(record_temp!=NULL) {
		NODE_AstTree stmts;
		if(record_temp==main_node) {
			SYMBOLTABLE sym = symbolTableinit("_main");
			int temp = enterFunction("_main", index, sym, record_temp);
			index++;
			printf("Function _main for records\n");
			stmts = record_temp;
		} else {
			if(getFunction(record_temp->child->tokens->tk->lexeme)!=-1) {
				prev_record->sibling = record_temp->sibling;
				char* name = record_temp->child->tokens->tk->lexeme;
				int startLine = record_temp->child->tokens->tk->lineNo;
				int endLine = deallocate(record_temp)+1;
				char e[200];
				sprintf(e, "Line %d-%d: Function %s defined more than once\n", startLine,endLine, name);
				printf("%s",e);
				errors[startLine] = (char*)malloc(sizeof(char)*(strlen(e)+1));
				strcpy(errors[startLine],e);
				record_temp = prev_record->sibling;
				continue;
			}
			SYMBOLTABLE sym = symbolTableinit(record_temp->child->tokens->tk->lexeme);
			int temp = enterFunction(record_temp->child->tokens->tk->lexeme, index, sym, record_temp);
			index++;
			printf("Function %s for records\n",record_temp->child->tokens->tk->lexeme);
			stmts = record_temp->child->sibling->sibling->sibling;
		}
		NODE_AstTree records = stmts->child->child;
		NODE_AstTree prev = NULL;
		for(; records!=NULL;) {
			printf("--%s--\n",records->child->tokens->next->tk->lexeme);
			NODE_AstTree record_def = records->child;
			int hval = lookupRecord(records->child->tokens->next->tk->lexeme);
			// printf("%u \n",records->sibling);
			if(hval!=-1) {
				if(prev==NULL) {
					stmts->child->child=records->sibling;
				} else {
					prev->sibling = records->sibling;
				}
				// printf("%u \n",stmts->child->child);
				records->sibling=NULL;
				char* name = records->child->tokens->next->tk->lexeme;
				int startLine = records->child->tokens->tk->lineNo;
				int endLine = deallocate(records)+1;
				char e[200];
				sprintf(e, "Line %d-%d: Record %s already defined\n", startLine,endLine, name);
				printf("%s",e);
				errors[startLine] = (char*)malloc(sizeof(char)*(strlen(e)+1));
				strcpy(errors[startLine],e);
				if(prev==NULL) {
					records = stmts->child->child;
					prev = NULL;
				} else {
					records = prev->sibling;
				}
				continue;
			}

			SYMBOLENTRY s = symbolEntryinit(0, -1, record_def->tokens->next->tk->lexeme, 0, 0, NULL);
			s->record_name = (char*)malloc(sizeof(char)*(strlen(record_def->tokens->next->tk->lexeme)+1));
			strcpy(s->record_name,record_def->tokens->next->tk->lexeme);
			hval = hashSymbolEntry(record_table, s, NULL, "record", s->record_name, NULL);
			printf("%d \n",hval);
			if(record_table->head==NULL) {
				record_table->head = s;
				x = s;
				printf("Added to record table\n");
			} else {
				x->next=s;
				x=s;
				printf("Added to previous record entry\n");
			}
			record_table->no_entries++;
			record_def=record_def->sibling;
			FIELD temp_field;
			int i=0,r=0;
			for(; record_def!=NULL; record_def=record_def->sibling) {
				printf("%s %s \n", record_def->tokens->tk->lexeme,record_def->tokens->next->tk->lexeme);
				VAL v;
				v.ival = -1;
				FIELD f = fieldinit(record_def->tokens->tk->token, v, record_def->tokens->next->tk->lexeme);
				if(s->record==NULL) {
					s->record = f;
					temp_field = f;
					printf("Added field to record entry\n");
				} else {
					temp_field->next = f;
					temp_field = f;
					printf("Added field to previous record field\n");
				}
				if(f->dType==INT) {
					i++;
				} else {
					r++;
				}
				// int hashval = hashSymbolEntry(h, record_table, s, f, "record", s->record_name, f->fieldname, hash_size);
			}
			s->int_no = i;
			s->real_no = r;
			printf("%d %d\n",i,r);
			s->offset = 2*i + 4*r;
			prev=records;
			records=records->sibling;
		}
		prev_record=record_temp;
		record_temp=record_temp->sibling;
		if(flag==1) {
			break;
		}
		if(record_temp==NULL) {
			prev_record = NULL;
			record_temp = main_node;
			flag=1;
		}
	}
	prev_record = NULL;
	flag = 0;
	record_temp = main_node->sibling;
	if(record_temp==NULL){
		flag=1;
		record_temp = main_node;
	}
	while(record_temp!=NULL) {
		NODE_AstTree stmts;
		if(record_temp==main_node) {
			stmts = main_node;
		} else {
			stmts = record_temp->child->sibling->sibling->sibling;
		}
		NODE_AstTree decls = stmts->child->sibling;
		y = addDeclarations(decls, global_table, 0, y, 1);
		prev_record=record_temp;
		record_temp=record_temp->sibling;
		if(flag==1) {
			break;
		}
		if(record_temp==NULL) {
			prev_record = NULL;
			record_temp = main_node;
			flag=1;
		}
	}
	printSymbolTable(record_table);

	printSymbolTable(global_table);
	NODE_AstTree f_temp = main_node;
	// SYMBOLTABLE main_table = symbolTableinit("_main");
	// FN_STACK stack= stack_init("_main", main_table);
	// printf("%s \n",stack->function_name);
	while(f_temp!=NULL) {
		NODE_AstTree fn;
		char* name;
		if(f_temp==main_node) {
			fn = main_node;
			name = (char*)malloc(sizeof(char)*6);
			strcpy(name,"_main");
		} else {
			fn = record_temp;
			name = (char*)malloc(sizeof(char)*(strlen(record_temp->child->tokens->tk->lexeme)+1));
			strcpy(name,record_temp->child->tokens->tk->lexeme);
		}
		int xyz = getFunction(name);
		SYMBOLTABLE sym = functions[xyz].st;
		functionSymbolTable(fn, sym);
		f_temp = f_temp->sibling;
	}

	// functionSymbolTable(main_node, h, hash_size, SYMBOLTABLE global_table)
}
