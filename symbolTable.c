#include "symbolTabledef.h"
// #include "astDef.h"

int hash_jump=0;
SYMBOLTABLE record_table;
SYMBOLTABLE global_table;
extern char** errors;

FN_STACK stack_init(char* name, SYMBOLTABLE s) {
	FN_STACK f = (FN_STACK)malloc(sizeof(fn_stack));
	f->function_name = (char*)malloc(sizeof(char)*(1+strlen(name)));
	strcpy(f->function_name,name);
	f->next = NULL;
	f->symboltable = s;
	return f;
}

FN_STACK push(FN_STACK f, char* name, SYMBOLTABLE s) {
	FN_STACK new = (FN_STACK)malloc(sizeof(fn_stack));
	new->function_name = (char*)malloc(sizeof(char)*(1+strlen(name)));
	strcpy(new->function_name,name);
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

FN_ENTRY functionsInit(int size) {
	FN_ENTRY x;
	x = (FN_ENTRY)malloc(sizeof(fn_entry)*size);
	for(int i=0; i<size; i++) {
		x[i].function_name = NULL;
		x[i].ptr = NULL;
	}
	return x;
}

int enterFunction(FN_ENTRY x, char* function, int index, SYMBOLTABLE st, NODE_AstTree fn_node, int size) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = hashVal + 7*(int)function[i];
		i++;
	}
	hashVal = hashVal%size;
	for(int h=1; h<=10; h++) {
		if(x[hashVal].ptr==NULL) {
			x[hashVal].function_name = (char*)malloc(sizeof(char)*(strlen(function)+1));
			strcpy(x[hashVal].function_name,function);
			x[hashVal].ptr = fn_node;
			x[hashVal].index = index;
			x[hashVal].st = st;
			return hashVal;
		}
		hashVal += (h*h);
		hashVal = hashVal%size;


	}
	return -1;
}

int getFunction(FN_ENTRY x, char* function, int size) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = hashVal + 7*(int)function[i];
		i++;
	}
	hashVal = hashVal%size;
	for(int h=1; h<=10; h++) {
		if(x[hashVal].ptr==NULL) {
			return -1;
		}
		if(strcmp(x[hashVal].function_name, function)==0) {
			return hashVal;
		}
		hashVal += (h*h);
		hashVal = hashVal%size;

	}
	return -1;
}

HASHSYMBOL hashSymbolInit(int size) {
	HASHSYMBOL h = (HASHSYMBOL)malloc(sizeof(hashsymbol) * size);
	for(int i=0; i<size; i++) {
		h[i].function = NULL;
		h[i].fieldid = NULL;
		h[i].id = NULL;
		h[i].entry_ptr = NULL;
		h[i].table_ptr = NULL;
		h[i].field_ptr = NULL;
		h[i].flag = 0;
	}
	return h;
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

FIELD fieldinit(dataType dt, VAL value, char* name) {
	FIELD f = (FIELD)malloc(sizeof(field));
	f->dType = dt;
	f->set = 0;
	if(dt==INT) {
		f->value.ival = value.ival;
	} else {
		f->value.rval = value.rval;
	}
	f->next = NULL;
	f->fieldname = (char*)malloc(sizeof(char)*(strlen(name)+1));
	strcpy(f->fieldname,name);
	return f;
}
int computeHash(char* function, char* id, char* fieldid, int size) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = hashVal + 7*(int)function[i];
		i++;
	}
	hashVal = hashVal%size;
	i=0;
	while(id[i]!='\0') {
		hashVal = hashVal + 17*(int)id[i];
		i++;
	}
	hashVal = hashVal%size;
	i=0;
	if(fieldid!=NULL) {
		while(fieldid[i]!='\0') {
			hashVal = hashVal + 37*(int)fieldid[i];
			i++;
		}
		hashVal = hashVal%size;
	}
	return hashVal;
}

int hashSymbolEntry(HASHSYMBOL hs, SYMBOLTABLE t, SYMBOLENTRY e, FIELD f, char* function, char* id, char* fieldid, int size){

	long long int hashval = computeHash(function, id, fieldid, size), h=1;

	while(hs[hashval].flag==1) {
		// if(strcmp(h[hashval].function,function)==0 && strcmp(h[hashval].id,id)==0) {
		//  if((fieldid!=NULL && strcmp(h[hashval].fieldid,fieldid)==0)||(fieldid==NULL && h[hashval].fieldid==NULL)) {
		//      return hashval;
		//  }
		// }
		hashval = (hashval + (h*h))%size;
		h++;
	}
	hs[hashval].function = (char*)malloc(sizeof(char) * (strlen(function)+1));
	hs[hashval].id = (char*)malloc(sizeof(char) * (strlen(id)+1));
	strcpy(hs[hashval].function,function);
	strcpy(hs[hashval].id, id);
	hs[hashval].entry_ptr = e;
	hs[hashval].table_ptr = t;
	hs[hashval].flag = 1;
	if(fieldid!=NULL) {
		hs[hashval].fieldid = (char*)malloc(sizeof(char) * (strlen(fieldid)+1));
		hs[hashval].field_ptr = f;
		strcpy(hs[hashval].fieldid,fieldid);
	} else {
		hs[hashval].fieldid = NULL;
		hs[hashval].field_ptr = NULL;
	}
	hash_jump = (hash_jump >= h-1) ? hash_jump : h-1;

	return hashval;
}
int lookupEntry(HASHSYMBOL hs, char *function, char *id, char *fieldid, int size) {

	long long int hashval = computeHash(function, id, fieldid, size), h=1;

	while(hs[hashval].flag!=0) {
		if(strcmp(hs[hashval].function,function)==0 && strcmp(hs[hashval].id,id)==0) {
			if((fieldid!=NULL && strcmp(hs[hashval].fieldid,fieldid)==0)||(fieldid==NULL && hs[hashval].fieldid==NULL)) {
				return hashval;
			}
		}
		hashval = (hashval + (h*h))%size;
		h++;
		if(h > hash_jump+1) {
			break;
		}
	}

	return -1;
}

int lookupRecord(HASHSYMBOL hs, char *record_name, int size) {

	long long int hashval = computeHash("record", record_name, NULL, size), h=1;

	while(hs[hashval].flag!=0) {
		if(hs[hashval].flag==-1) {
			hashval = (hashval + (h*h))%size;
			h++;
			if(h > hash_jump+1) {
				break;
			}
			continue;
		}
		if(strcmp(hs[hashval].function,"record")==0 && strcmp(hs[hashval].entry_ptr->record_name,record_name)==0) {
			return hashval;
		}
		hashval = (hashval + (h*h))%size;
		h++;
		if(h > hash_jump+1) {
			break;
		}
	}

	return -1;
}

int duplicateLookup(HASHSYMBOL h, SYMBOLTABLE t, SYMBOLENTRY e, FIELD f, char* function, char* id, char* fieldid, int size, char* error){
	int hval = lookupEntry(h, function, id, fieldid, size);
	if(hval!=-1) {
		printf("%s\n",error);
	} else {
		hval = hashSymbolEntry(h, t, e, f, function, id, fieldid, size);
	}
	return hval;
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
		printf("%s\n",temp->id);
		temp=temp->next;
	}
}

void functionSymbolTable(NODE_AstTree function, HASHSYMBOL h, int hash_size, SYMBOLTABLE st) {

	printf("Function: %s\n",st->name);

	int offset=0;
	NODE_AstTree stmts = function;
	SYMBOLENTRY y = NULL;
	if(strcmp(st->name,"_main")!=0) {
		stmts = function->child->sibling->sibling->sibling;
		NODE_AstTree node = function->child;
		node = node->sibling;
		NODE_AstTree input_par = node->child;
		NODE_AstTree prev = NULL;
		while(input_par!=NULL) {
			int hashval = lookupEntry(h, st->name, input_par->tokens->next->tk->lexeme, NULL, hash_size);

			if(hashval!=-1) {
				if(prev==NULL) {
					node->child=input_par->sibling;
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
					input_par = node->child;
					prev = NULL;
				} else {
					input_par = prev->sibling;
				}
				continue;
			}


			SYMBOLENTRY s;
			if(input_par->tokens->tk->token==TK_INT) {
				TOKENINFO t = input_par->tokens->next->tk;
				VAL v;
				v.ival = -1;
				FIELD f = fieldinit(INT, v, t->lexeme);
				s = symbolEntryinit(2, offset, t->lexeme, 1, 0, f);
				s->record = f;
				offset+=2;
				int hval = hashSymbolEntry(h, st, s, NULL, st->name, t->lexeme, NULL, hash_size);

			} else if(input_par->tokens->tk->token==TK_REAL) {
				VAL v;
				v.rval = -1;
				TOKENINFO t = input_par->tokens->next->tk;
				FIELD f = fieldinit(REAL, v,t->lexeme);
				s = symbolEntryinit(4, offset, t->lexeme, 0, 1, f);
				offset+=4;
				int hval = hashSymbolEntry(h, st, s, NULL, st->name, t->lexeme, NULL, hash_size);

			} else {
				TOKENINFO t = input_par->tokens->next->tk;
				s = symbolEntryinit(0, offset, t->lexeme, 0, 0, NULL);
				FIELD temp_field=NULL;

				int hval = lookupRecord(h, input_par->tokens->tk->lexeme, hash_size);
				if(hval==-1) {
					if(prev==NULL) {
						node->child=input_par->sibling;
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
						input_par = node->child;
						prev = NULL;
					} else {
						input_par = prev->sibling;
					}
					free(s);
					continue;
				}
				int val = hashSymbolEntry(h, st, s, NULL, st->name, s->id, NULL, hash_size);


				FIELD f = h[hval].entry_ptr->record;
				int i=0,r=0;
				while(f!=NULL) {
					VAL v;
					v.ival = -1;
					FIELD temp = fieldinit(f->dType, v, f->fieldname);
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
					int hashval = hashSymbolEntry(h, st, s, temp, st->name, s->id, temp->fieldname, hash_size);
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


		node = node->sibling;
		NODE_AstTree output_par = node->child;
		prev = NULL;
		while(output_par!=NULL) {
			int hashval = lookupEntry(h, st->name, output_par->tokens->next->tk->lexeme, NULL, hash_size);

			if(hashval!=-1) {
				if(prev==NULL) {
					node->child=output_par->sibling;
				} else {
					prev->sibling = output_par->sibling;
				}
				output_par->sibling=NULL;
				char* name = output_par->tokens->next->tk->lexeme;
				int Line = output_par->tokens->tk->lineNo;
				int temp = deallocate(output_par);
				char e[200];
				sprintf(e, "Line %d: Variable %s already declared.\n", Line, name);
				printf("%s",e);
				errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
				strcpy(errors[Line],e);
				if(prev==NULL) {
					output_par = node->child;
					prev = NULL;
				} else {
					output_par = prev->sibling;
				}
				continue;
			}


			SYMBOLENTRY s;
			if(output_par->tokens->tk->token==TK_INT) {
				TOKENINFO t = output_par->tokens->next->tk;
				VAL v;
				v.ival = -1;
				FIELD f = fieldinit(INT, v, t->lexeme);
				s = symbolEntryinit(2, offset, t->lexeme, 1, 0, f);
				s->record = f;
				offset+=2;
				int hval = hashSymbolEntry(h, st, s, NULL, st->name, t->lexeme, NULL, hash_size);

			} else if(input_par->tokens->tk->token==TK_REAL) {
				VAL v;
				v.rval = -1;
				TOKENINFO t = output_par->tokens->next->tk;
				FIELD f = fieldinit(REAL, v,t->lexeme);
				s = symbolEntryinit(4, offset, t->lexeme, 0, 1, f);
				offset+=4;
				int hval = hashSymbolEntry(h, st, s, NULL, st->name, t->lexeme, NULL, hash_size);

			} else {
				TOKENINFO t = output_par->tokens->next->tk;
				s = symbolEntryinit(0, offset, t->lexeme, 0, 0, NULL);
				FIELD temp_field=NULL;

				int hval = lookupRecord(h, output_par->tokens->tk->lexeme, hash_size);
				if(hval==-1) {
					if(prev==NULL) {
						node->child=output_par->sibling;
					} else {
						prev->sibling = output_par->sibling;
					}
					output_par->sibling=NULL;
					char* name = output_par->tokens->tk->lexeme;
					int Line = output_par->tokens->tk->lineNo;
					int temp = deallocate(output_par);
					char e[200];
					sprintf(e, "Line %d: No record %s found.\n", Line, name);
					printf("%s",e);
					errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
					strcpy(errors[Line],e);
					if(prev==NULL) {
						output_par = node->child;
						prev = NULL;
					} else {
						output_par = prev->sibling;
					}
					free(s);
					continue;
				}
				int val = hashSymbolEntry(h, st, s, NULL, st->name, s->id, NULL, hash_size);


				FIELD f = h[hval].entry_ptr->record;
				int i=0,r=0;
				while(f!=NULL) {
					VAL v;
					v.ival = -1;
					FIELD temp = fieldinit(f->dType, v, f->fieldname);
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
					int hashval = hashSymbolEntry(h, st, s, temp, st->name, s->id, temp->fieldname, hash_size);
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
			prev = output_par;
			output_par=output_par->sibling;
		}
	}


	NODE_AstTree node = stmts->child->sibling;
	NODE_AstTree decls = node->child;
	NODE_AstTree prev = NULL;
	while(decls!=NULL) {
		if(decls->tokens->next->next!=NULL) {
			prev = decls;
			decls = decls->sibling;
			continue;
		}
		int hashval = lookupEntry(h, st->name, decls->tokens->next->tk->lexeme, NULL, hash_size);

		if(hashval!=-1) {
			if(prev==NULL) {
				node->child=decls->sibling;
			} else {
				prev->sibling = decls->sibling;
			}
			decls->sibling=NULL;
			char* name = decls->tokens->next->tk->lexeme;
			int Line = decls->tokens->tk->lineNo;
			int temp = deallocate(decls);
			char e[200];
			sprintf(e, "Line %d: Variable %s already declared.\n", Line, name);
			printf("%s",e);
			errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
			strcpy(errors[Line],e);
			if(prev==NULL) {
				decls = node->child;
				prev = NULL;
			} else {
				decls = prev->sibling;
			}
			continue;
		}


		SYMBOLENTRY s;
		if(decls->tokens->tk->token==TK_INT) {
			TOKENINFO t = decls->tokens->next->tk;
			VAL v;
			v.ival = -1;
			FIELD f = fieldinit(INT, v, t->lexeme);
			s = symbolEntryinit(2, offset, t->lexeme, 1, 0, f);
			s->record = f;
			offset+=2;
			int hval = hashSymbolEntry(h, st, s, NULL, st->name, t->lexeme, NULL, hash_size);

		} else if(decls->tokens->tk->token==TK_REAL) {
			VAL v;
			v.rval = -1;
			TOKENINFO t = decls->tokens->next->tk;
			FIELD f = fieldinit(REAL, v,t->lexeme);
			s = symbolEntryinit(4, offset, t->lexeme, 0, 1, f);
			offset+=4;
			int hval = hashSymbolEntry(h, st, s, NULL, st->name, t->lexeme, NULL, hash_size);

		} else {
			TOKENINFO t = decls->tokens->next->tk;
			s = symbolEntryinit(0, offset, t->lexeme, 0, 0, NULL);
			FIELD temp_field=NULL;

			int hval = lookupRecord(h, decls->tokens->tk->lexeme, hash_size);
			if(hval==-1) {
				if(prev==NULL) {
					node->child=decls->sibling;
				} else {
					prev->sibling = decls->sibling;
				}
				decls->sibling=NULL;
				char* name = decls->tokens->tk->lexeme;
				int Line = decls->tokens->tk->lineNo;
				int temp = deallocate(decls);
				char e[200];
				sprintf(e, "Line %d: No record %s found.\n", Line, name);
				printf("%s",e);
				errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
				strcpy(errors[Line],e);
				if(prev==NULL) {
					decls = node->child;
					prev = NULL;
				} else {
					decls = prev->sibling;
				}
				free(s);
				continue;
			}
			int val = hashSymbolEntry(h, st, s, NULL, st->name, s->id, NULL, hash_size);


			FIELD f = h[hval].entry_ptr->record;
			int i=0,r=0;
			while(f!=NULL) {
				VAL v;
				v.ival = -1;
				FIELD temp = fieldinit(f->dType, v, f->fieldname);
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
				int hashval = hashSymbolEntry(h, st, s, temp, st->name, s->id, temp->fieldname, hash_size);
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
		prev = decls;
		decls=decls->sibling;
	}

	printSymbolTable(st);
}



HASHSYMBOL semanticAnalyzer(NODE_AstTree ast) {
	int hash_size = 500;
	HASHSYMBOL h = hashSymbolInit(hash_size);
	NODE_AstTree main_node = ast->child;
	NODE_AstTree record_temp = main_node->sibling, prev_record = main_node;
	NODE_AstTree function = main_node->sibling;
	record_table = symbolTableinit("record");
	global_table = symbolTableinit("global");
	int index=0;
	// printSymbolTable(global_table);
	FN_ENTRY functions = functionsInit(200);

	SYMBOLENTRY x,y;
	int flag=0;
	while(record_temp!=NULL) {
		NODE_AstTree stmts;
		if(record_temp==main_node) {
			SYMBOLTABLE sym = symbolTableinit("_main");
			int temp = enterFunction(functions, "_main", index, sym, record_temp, 200);
			index++;
			printf("Function _main for records\n");
			stmts = record_temp;
		} else {
			if(getFunction(functions, record_temp->child->tokens->tk->lexeme, 200)!=-1) {
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
			int temp = enterFunction(functions, record_temp->child->tokens->tk->lexeme, index, sym, record_temp, 200);
			index++;
			printf("Function %s for records\n",record_temp->child->tokens->tk->lexeme);
			stmts = record_temp->child->sibling->sibling->sibling;
		}
		NODE_AstTree records = stmts->child->child;
		NODE_AstTree prev = NULL;
		for(; records!=NULL;) {
			printf("--%s--\n",records->child->tokens->next->tk->lexeme);
			NODE_AstTree record_def = records->child;
			int hval = lookupRecord(h, records->child->tokens->next->tk->lexeme, hash_size);
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
			hval = hashSymbolEntry(h, record_table, s, NULL, "record", s->record_name, NULL, hash_size);
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
				printf("%s \n",record_def->tokens->next->tk->lexeme);
				VAL v;
				v.ival = -1;
				FIELD f = fieldinit(record_def->tokens->tk->dataType, v, record_def->tokens->next->tk->lexeme);
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
	for(record_temp=main_node->sibling; record_temp!=NULL;) {
		NODE_AstTree stmts;
		if(record_temp==main_node) {
			stmts = main_node;
		} else {
			stmts = record_temp->child->sibling->sibling->sibling;
		}
		NODE_AstTree decls = stmts->child->sibling->child;
		NODE_AstTree prev = NULL;
		for(; decls!=NULL;) {
			if(decls->tokens->next->next!=NULL) {
				printf("----------%s--------\n",decls->tokens->next->tk->lexeme);

				int hval = lookupEntry(h, "global", decls->tokens->next->tk->lexeme, NULL, hash_size);

				if(hval!=-1) {
					if(prev==NULL) {
						stmts->child->sibling->child=decls->sibling;
					} else {
						prev->sibling = decls->sibling;
					}
					decls->sibling=NULL;
					char* name = decls->tokens->next->tk->lexeme;
					int Line = decls->tokens->tk->lineNo;
					int temp = deallocate(decls);
					char e[200];
					sprintf(e, "Line %d: Variable %s being a global variable, cannot be declared more than once.\n", Line, name);
					printf("%s",e);
					errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
					strcpy(errors[Line],e);
					if(prev==NULL) {
						decls = stmts->child->sibling->child;
						prev = NULL;
					} else {
						decls = prev->sibling;
					}
					continue;
				}


				SYMBOLENTRY s = symbolEntryinit(0, -1, decls->tokens->next->tk->lexeme, 0, 0, NULL);

				FIELD temp_field;
				if(decls->tokens->tk->token==TK_INT) {
					s->int_no = 1;
					VAL v;
					v.ival = -1;
					temp_field = fieldinit(INT, v, decls->tokens->next->tk->lexeme);
					s->record = temp_field;
					// int hashval = lookupEntry(h, "global", s->id, temp_field->fieldname, hash_size);
					int hashval = hashSymbolEntry(h, global_table, s, NULL, "global", s->id, NULL, hash_size);


				} else if(decls->tokens->tk->token==TK_REAL) {
					s->real_no = 1;
					VAL v;
					v.rval = -1;
					temp_field = fieldinit(REAL, v, decls->tokens->next->tk->lexeme);
					s->record = temp_field;
					// int hashval = lookupEntry(h, "global", s->id, temp_field->fieldname, hash_size);
					int hashval = hashSymbolEntry(h, global_table, s, NULL, "global", s->id, NULL, hash_size);

				} else {
					s->record_name = (char*)malloc(sizeof(char)*(strlen(decls->tokens->tk->lexeme)+1));
					strcpy(s->record_name,decls->tokens->tk->lexeme);
					printf("Record-%s \n",decls->tokens->tk->lexeme);
					int hashval = lookupRecord(h, decls->tokens->tk->lexeme, hash_size);
					if(hashval==-1) {
						if(prev==NULL) {
							stmts->child->sibling->child=decls->sibling;
						} else {
							prev->sibling = decls->sibling;
						}
						decls->sibling=NULL;
						char* name = decls->tokens->tk->lexeme;
						int Line = decls->tokens->tk->lineNo;
						int temp = deallocate(decls);
						char e[200];
						sprintf(e, "Line %d: No record %s found.\n", Line, name);
						printf("%s",e);
						errors[Line] = (char*)malloc(sizeof(char)*(strlen(e)+1));
						strcpy(errors[Line],e);
						if(prev==NULL) {
							decls = stmts->child->sibling->child;
							prev = NULL;
						} else {
							decls = prev->sibling;
						}
						free(s);
						continue;
					}
					int val = hashSymbolEntry(h, global_table, s, NULL, "global", s->id, NULL, hash_size);

					FIELD f = h[hashval].entry_ptr->record;
					int i=0,r=0;
					while(f!=NULL) {
						VAL v;
						v.ival = -1;
						FIELD temp = fieldinit(f->dType, v, f->fieldname);
						if(s->record==NULL) {
							s->record = temp;
							temp_field = temp;
						} else {
							temp_field->next = temp;
							temp_field = temp;
						}
						if(temp->dType==INT) {
							i++;
						} else {
							r++;
						}
						int hashval = hashSymbolEntry(h, global_table, s, temp, "global", s->id, temp->fieldname, hash_size);
						f=f->next;
					}
					s->int_no = i;
					s->real_no = r;
					s->width = 2*i+4*r;



				}
				s->offset = 2*s->int_no + 4*s->real_no;
				s->width = s->offset;
				if(global_table->head==NULL) {
					global_table->head = s;
					y = s;
					printf("Added to global table\n");
				} else {
					y->next=s;
					y=s;
					printf("Added to previous global entry\n");
				}
				global_table->no_entries++;
			}
			prev=decls;
			decls=decls->sibling;
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
		int xyz = getFunction(functions, name, 200);
		SYMBOLTABLE sym = functions[xyz].st;
		functionSymbolTable(fn, h, hash_size, sym);
		f_temp = f_temp->sibling;
	}

	// functionSymbolTable(main_node, h, hash_size, SYMBOLTABLE global_table)
}
