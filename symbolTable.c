#include "symbolTabledef.h"
// #include "astDef.h"

int hash_jump=0;

char** functionsInit(int size) {
	char** x;
	x = (char**)malloc(sizeof(char*)*size);
	for(int i=0;i<size;i++) {
		x[i]=NULL;
	}
	return x;
}

int enterFunction(char** x, char* function, int size) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = hashVal + 7*(int)function[i];
		i++;
	}
	hashVal = hashVal%size;
	for(int h=1;h<=10;h++) {
		if(x[hashVal]==NULL) {
			x[hashVal] = (char*)malloc(sizeof(char)*(strlen(function)+1));
			strcpy(x[hashVal],function);
			return hashVal;
		}
		hashVal += (h*h);
		hashVal = hashVal%size;


	}
	return -1;
}

int getFunction(char** x, char* function, int size) {
	int i=0;
	long long hashVal=0;
	while(function[i]!='\0') {
		hashVal = hashVal + 7*(int)function[i];
		i++;
	}
	hashVal = hashVal%size;
	for(int h=1;h<=10;h++) {
		if(x[hashVal]==NULL) {
			return -1;
		}
		if(strcmp(x[hashVal], function)==0) {
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

int duplicateLookup(HASHSYMBOL h, SYMBOLTABLE t, SYMBOLENTRY e, FIELD f, char* function, char* id, char* fieldid, int size, char* error){
	int hval = lookupEntry(h, function, id, fieldid, size);
	if(hval!=-1) {
		printf("%s\n",error);
	} else {
		hval = hashSymbolEntry(h, t, e, f, function, id, fieldid, size);
	}
	return hval;
}
void functionSymbolTable(NODE_AstTree function, HASHSYMBOL h, int hash_size, SYMBOLTABLE global_table) {
	int offset=0;
	NODE_AstTree node = function->child;
	SYMBOLTABLE st = symbolTableinit(node->tokens->tk->lexeme);
	node = node->sibling;


	NODE_AstTree input_par = node->child;
	for(; input_par!=NULL; input_par=input_par->sibling) {

		if(input_par->tokens->tk->token==TK_INT) {

			TOKENINFO t = input_par->tokens->next->tk;
			VAL v;
			v.ival = -1;
			FIELD f = fieldinit(INT, v, t->lexeme);
			SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 1, 0, f);
			offset+=2;
			int hashval = duplicateLookup(h, st, s, f, node->tokens->tk->lexeme, t->lexeme, t->lexeme, hash_size, "Duplicate Integer\n");

		} else if(input_par->tokens->tk->token==TK_REAL) {
			VAL v;
			v.rval = -1;
			TOKENINFO t = input_par->tokens->next->tk;
			FIELD f = fieldinit(REAL, v,t->lexeme);
			SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 0, 1, f);
			offset+=4;
			int hashval = duplicateLookup(h, st, s, f, node->tokens->tk->lexeme, t->lexeme, t->lexeme, hash_size, "Duplicate Real\n");

		} else {
			TOKENINFO t = input_par->tokens->next->tk;
			SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 0, 0, NULL);
			FIELD temp_field=NULL;
			int hashval = lookupEntry(h, "record", input_par->tokens->tk->lexeme, NULL, hash_size);
			if(hashval==-1) {
				printf("Error: Record undefined");
			} else {
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
						offset+=2;
					} else {
						r++;
						offset+=4;
					}
					hashval = duplicateLookup(h, global_table, s, temp, st->name, s->id, temp->fieldname, hash_size, "Duplicate Record Field\n");
					f=f->next;
				}

				s->int_no = i;
				s->real_no = r;
			}


		}
	}


	NODE_AstTree output_par = node->child->sibling;
	for(; output_par!=NULL; output_par=output_par->sibling) {

		if(output_par->tokens->tk->token==TK_INT) {

			TOKENINFO t = output_par->tokens->next->tk;
			VAL v;
			v.ival = -1;
			FIELD f = fieldinit(INT, v, t->lexeme);
			SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 1, 0, f);
			offset+=2;
			int hashval = duplicateLookup(h, st, s, f, node->tokens->tk->lexeme, t->lexeme, t->lexeme, hash_size, "Duplicate Integer\n");

		} else if(input_par->tokens->tk->token==TK_REAL) {

			TOKENINFO t = output_par->tokens->next->tk;
			VAL v;
			v.rval = -1;
			FIELD f = fieldinit(REAL, v, t->lexeme);
			SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 0, 1, f);
			offset+=4;
			int hashval = duplicateLookup(h, st, s, f, node->tokens->tk->lexeme, t->lexeme, t->lexeme, hash_size, "Duplicate Real\n");

		} else {
			TOKENINFO t = output_par->tokens->next->tk;
			SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 0, 0, NULL);
			FIELD temp_field=NULL;
			int hashval = lookupEntry(h, "record", output_par->tokens->tk->lexeme, NULL, hash_size);
			if(hashval==-1) {
				printf("Error: Record undefined");
			} else {
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
						offset+=2;
					} else {
						r++;
						offset+=4;
					}
					hashval = duplicateLookup(h, global_table, s, temp, st->name, s->id, temp->fieldname, hash_size, "Duplicate Record Field\n");
					f=f->next;
				}

				s->int_no = i;
				s->real_no = r;
			}


		}
	}


	NODE_AstTree declarations = node->child->sibling->sibling->child->sibling->child;
	for(; declarations!=NULL; declarations=declarations->sibling) {
		if(declarations->tokens->next->next==NULL) {
			if(output_par->tokens->tk->token==TK_INT) {

				TOKENINFO t = declarations->tokens->next->tk;
				VAL v;
				v.ival = -1;
				FIELD f = fieldinit(INT, v, t->lexeme);
				SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 1, 0, f);
				offset+=2;
				int hashval = duplicateLookup(h, st, s, f, node->tokens->tk->lexeme, t->lexeme, t->lexeme, hash_size, "Duplicate Integer\n");

			} else if(input_par->tokens->tk->token==TK_REAL) {

				TOKENINFO t = declarations->tokens->next->tk;
				VAL v;
				v.rval = -1;
				FIELD f = fieldinit(REAL, v, t->lexeme);
				SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 0, 1, f);
				offset+=4;
				int hashval = duplicateLookup(h, st, s, f, node->tokens->tk->lexeme, t->lexeme, t->lexeme, hash_size, "Duplicate Real\n");

			} else {
				TOKENINFO t = declarations->tokens->next->tk;
				SYMBOLENTRY s = symbolEntryinit(0, offset, t->lexeme, 0, 0, NULL);
				FIELD temp_field=NULL;
				int hashval = lookupEntry(h, "record", declarations->tokens->tk->lexeme, NULL, hash_size);
				if(hashval==-1) {
					printf("Error: Record undefined");
				} else {
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
							offset+=2;
						} else {
							r++;
							offset+=4;
						}
						hashval = duplicateLookup(h, global_table, s, temp, st->name, s->id, temp->fieldname, hash_size, "Duplicate Record Field\n");
						f=f->next;
					}
					s->int_no = i;
					s->real_no = r;
				}



			}
		}
	}

}

void printSymbolTable(SYMBOLTABLE s) {
	SYMBOLENTRY temp = s->head;
	printf("Printing SYMBOLTABLE for %s with entries %d\n",s->name, s->no_entries);
	while(temp!=NULL) {
		printf("%s\n",temp->id);
		temp=temp->next;
	}
}

int deallocate(NODE_AstTree x) {
	NODE_AstTree child = x->child;
	int y=0;
	while(child!=NULL) {
		NODE_AstTree next = child->sibling;
		int z = deallocate(child);
		y = (y>z)?y:z;
		child = next;
	}
	if(x->tokens!=NULL) {
		y = (y>x->tokens->tk->lineNo)?y:x->tokens->tk->lineNo;
	}
	free(x);
	return y;
}
HASHSYMBOL semanticAnalyzer(NODE_AstTree ast, char** errors) {
	int hash_size = 500;
	HASHSYMBOL h = hashSymbolInit(hash_size);
	NODE_AstTree main_node = ast->child;
	NODE_AstTree record_temp = main_node, prev_record = main_node;
	NODE_AstTree function = main_node->sibling;

	SYMBOLTABLE record_table = symbolTableinit("record");
	SYMBOLTABLE global_table = symbolTableinit("global");
	// printSymbolTable(global_table);
	char** functions = functionsInit(200);

	SYMBOLENTRY x,y;
	for(; record_temp!=NULL; prev_record=record_temp,record_temp=record_temp->sibling) {
		NODE_AstTree stmts;
		if(record_temp==main_node) {
			int temp = enterFunction(functions, "_main", 200);
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
				record_temp = prev_record;
				continue;
			}

			int temp = enterFunction(functions, record_temp->child->tokens->tk->lexeme, 200);
			printf("Function %s for records\n",record_temp->child->tokens->tk->lexeme);
			stmts = record_temp->child->sibling->sibling->sibling;
		}
		NODE_AstTree records = stmts->child->child;
		NODE_AstTree decls = stmts->child->sibling->child;
		NODE_AstTree prev = NULL;

		for(; records!=NULL; prev=records,records=records->sibling) {
			printf("%s\n",records->child->tokens->next->tk->lexeme);
			NODE_AstTree record_def = records->child;
			int hval = lookupEntry(h, "record", records->child->tokens->next->tk->lexeme, NULL, hash_size);

			if(hval!=-1) {
				if(prev==NULL) {
					stmts->child->child=records->sibling;
				} else {
					prev->sibling = records->sibling;
				}
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
				} else {
					records = prev;
				}
				continue;
			}
			SYMBOLENTRY s = symbolEntryinit(0, -1, record_def->tokens->next->tk->lexeme, 0, 0, NULL);
			s->record_name = (char*)malloc(sizeof(char)*(strlen(record_def->tokens->next->tk->lexeme)+1));
			strcpy(s->record_name,record_def->tokens->next->tk->lexeme);
			hval = hashSymbolEntry(h, record_table, s, NULL, "record", s->id, NULL, hash_size);
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
				int hashval = hashSymbolEntry(h, record_table, s, f, "record", s->id, f->fieldname, hash_size);
			}
			s->int_no = i;
			s->real_no = r;
			s->offset = 2*i + 4*r;
		}
		prev=NULL;
		for(; decls!=NULL; prev=decls, decls=decls->sibling) {
		    if(decls->tokens->next->next!=NULL) {
				printf("----------%s--------\n",decls->tokens->next->tk->lexeme);

				int hval = lookupEntry(h, "global", decls->tokens->next->tk->lexeme, decls->tokens->next->tk->lexeme, hash_size);

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
					} else {
						decls = prev;
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
		            int hashval = lookupEntry(h, "global", s->id, temp_field->fieldname, hash_size);
		            hashval = hashSymbolEntry(h, global_table, s, temp_field, "global", s->id, temp_field->fieldname, hash_size);


		        } else if(decls->tokens->tk->token==TK_REAL) {
		            s->real_no = 1;
		            VAL v;
		            v.rval = -1;
		            temp_field = fieldinit(REAL, v, decls->tokens->next->tk->lexeme);
		            s->record = temp_field;
		            int hashval = lookupEntry(h, "global", s->id, temp_field->fieldname, hash_size);
					hashval = hashSymbolEntry(h, global_table, s, temp_field, "global", s->id, temp_field->fieldname, hash_size);

		        } else {
					s->record_name = (char*)malloc(sizeof(char)*(strlen(decls->tokens->tk->lexeme)+1));
					strcpy(s->record_name,decls->tokens->tk->lexeme);
					printf("Record-%s \n",decls->tokens->tk->lexeme);
		            int hashval = lookupEntry(h, "record", decls->tokens->tk->lexeme, NULL, hash_size);
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
						} else {
							decls = prev;
						}
						free(s);
						continue;
		            }

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



		        }
		        s->offset = 2*s->int_no + 4*s->real_no;

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
		}
	}
	printSymbolTable(record_table);

	printSymbolTable(global_table);
	// NODE_AstTree f_temp = main_node;
	// while(f_temp!=NULL) {
	//  functionSymbolTable(f_temp, h, hash_size, global_table);
	//  f_temp=f_temp->sibling;
	// }

}
