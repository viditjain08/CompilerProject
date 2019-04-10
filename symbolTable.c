#include "symbolTabledef.h"
// #include "astDef.h"

int hash_jump=0;

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

HASHSYMBOL populateSymbolTable(NODE_AstTree ast) {
	int hash_size = 500;
	HASHSYMBOL h = hashSymbolInit(hash_size);
	NODE_AstTree main_node = ast->child;
	NODE_AstTree record_temp = main_node->sibling;
	NODE_AstTree function = main_node->sibling;

	SYMBOLTABLE record_table = symbolTableinit("record");
	SYMBOLTABLE global_table = symbolTableinit("global");
	// printSymbolTable(global_table);

	SYMBOLENTRY x,y;
	for(; record_temp!=NULL; record_temp=record_temp->sibling) {
		printf("Function %s for records\n",record_temp->child->tokens->tk->lexeme);
	//  NODE_AstTree stmts = record_temp->child->sibling->sibling->sibling;
	//  NODE_AstTree records = stmts->child->child;
	//  NODE_AstTree decls = stmts->child->sibling->child;
	//
	//  for(; records!=NULL; records=records->sibling) {
	     // NODE_AstTree record_def = records->child;
	     // SYMBOLENTRY s = symbolEntryinit(0, -1, record_def->tokens->next->tk->lexeme, 0, 0, NULL);
	     // int hval = duplicateLookup(h, record_table, s, NULL, "record", s->id, NULL, hash_size, "Duplicate record\n");
		 // printf("%d ",hval);
	     // if(record_table->head==NULL) {
	     //     record_table->head = s;
	     //     x = s;
	     // } else {
	     //     x->next=s;
	     //     x=s;
	     // }
	     // record_table->no_entries++;
	     // record_def=record_def->sibling;
	     // FIELD temp_field;
	     // int i=0,r=0;
	     // for(; record_def!=NULL; record_def=record_def->sibling) {
	     //     VAL v;
	     //     v.ival = -1;
	     //     FIELD f = fieldinit(record_def->tokens->tk->dataType, v, record_def->tokens->next->tk->lexeme);
	     //     if(s->record==NULL) {
	     //         s->record = f;
	     //         temp_field = f;
	     //     } else {
	     //         temp_field->next = f;
	     //         temp_field = f;
	     //     }
	     //     if(f->dType==INT) {
	     //         i++;
	     //     } else {
	     //         r++;
	     //     }
	     //     hashval = duplicateLookup(h, record_table, s, f, "record", s->id, f->fieldname, hash_size, "Duplicate Record Field\n");
		 //
	     // }
	     // s->int_no = i;
	     // s->real_no = r;
	     // s->offset = 2*i + 4*r;
	 // }

	 // for(; decls!=NULL; decls=decls->sibling) {
	 //     if(decls->tokens->next->next->tk->token==TK_GLOBAL) {
	 //
	 //         SYMBOLENTRY s = symbolEntryinit(0, -1, decls->tokens->next->tk->lexeme, 0, 0, NULL);
	 //         if(global_table->head==NULL) {
	 //             global_table->head = s;
	 //             y = s;
	 //         } else {
	 //             y->next=s;
	 //             y=s;
	 //         }
	 //         global_table->no_entries++;
	 //         FIELD temp_field;
	 //         if(decls->tokens->tk->token==TK_INT) {
	 //             s->int_no = 1;
	 //             VAL v;
	 //             v.ival = -1;
	 //             temp_field = fieldinit(INT, v, decls->tokens->next->tk->lexeme);
	 //             s->record = temp_field;
	 //             int hashval = lookupEntry(h, "global", s->id, f->fieldname, hash_size);
	 //             hashval = duplicateLookup(h, global_table, s, temp_field, "global", s->id, f->fieldname, hash_size, "Duplicate Integer\n");
	 //
	 //
	 //         } else if(decls->tokens->tk->token==TK_REAL) {
	 //             s->real_no = 1;
	 //             VAL v;
	 //             v.rval = -1;
	 //             temp_field = fieldinit(REAL, v, decls->tokens->next->tk->lexeme);
	 //             s->record = temp_field;
	 //             hashval = duplicateLookup(h, global_table, s, temp_field, "global", s->id, f->fieldname, hash_size, "Duplicate Real\n");
	 //
	 //         } else {
	 //             int hashval = lookupEntry(h, "record", s->id, NULL, hash_size);
	 //             if(hashval==-1) {
	 //                 printf("Error: Record undefined");
	 //             } else {
	 //                 FIELD f = h[hashval].entry_ptr->record;
	 //                 int i=0,r=0;
	 //                 while(f!=NULL) {
	 //                     VAL v;
	 //                     v.ival = -1;
	 //                     FIELD temp = fieldinit(f->dType, v, f->fieldname);
	 //                     if(s->record==NULL) {
	 //                         s->record = temp;
	 //                         temp_field = temp;
	 //                     } else {
	 //                         temp_field->next = temp;
	 //                         temp_field = temp;
	 //                     }
	 //                     if(temp->dType==INT) {
	 //                         i++;
	 //                     } else {
	 //                         r++;
	 //                     }
	 //                     hashval = duplicateLookup(h, global_table, s, temp, "global", s->id, temp->fieldname, hash_size, "Duplicate Global Record");
	 //                     f=f->next;
	 //                 }
	 //             }
	 //             s->int_no = i;
	 //             s->real_no = r;
	 //         }
	 //         s->offset = 2*s->int_no + 4*s->real_no;
	 //     }
	 // }
	}
	// NODE_AstTree f_temp = main_node;
	// while(f_temp!=NULL) {
	//  functionSymbolTable(f_temp, h, hash_size, global_table);
	//  f_temp=f_temp->sibling;
	// }

}
