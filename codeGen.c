/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include "codeGenDef.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
extern HASHSYMBOL h;
extern int hash_size;
extern FN_ENTRY functions;
extern int fn_size;
extern SYMBOLTABLE record_table;
extern SYMBOLTABLE global_table;
FILE *fp;
int global_count=0;
extern FN_STACK stack;
void arithmeticTrav(NODE_AstTree temp){

	if(temp->child==NULL) {
		int flag=0;
		int hashval=-1;
        if(temp->tokens->tk->token==TK_NUM) {
            printf("mov\tax,%s\n",temp->tokens->tk->lexeme);
            printf("push\tax\n");
            stack = push(stack, temp->tokens->tk->lexeme, NULL);
            return;
        }
		if(temp->tokens->next==NULL) {
			hashval = lookupEntry("_main", temp->tokens->tk->lexeme, NULL);
			if(hashval==-1) {
				hashval = lookupEntry("global", temp->tokens->tk->lexeme, NULL);
			}
			SYMBOLENTRY t = h[hashval].entry_ptr;
			if((t->real_no+t->int_no)>1) {
				flag=1;
			}
		} else {
			flag=2;
		}

		if(flag==0) {
			printf("mov\tax,[%s]\n",temp->tokens->tk->lexeme);
			printf("push\tax\n");
			stack = push(stack, temp->tokens->tk->lexeme, NULL);
		} else if(flag==1) {
			printf("mov\tax,%s\n",temp->tokens->tk->lexeme);
			printf("push\tax\n");
			stack = push(stack, temp->tokens->tk->lexeme, NULL);
		} else {
			printf("mov\tax,[%s+%s]\n",temp->tokens->tk->lexeme,temp->tokens->next->tk->lexeme);
			printf("push\tax\n");
			stack = push(stack, temp->tokens->tk->lexeme, temp->tokens->next->tk->lexeme);
		}

	} else {
		arithmeticTrav(temp->child);
		arithmeticTrav(temp->child->sibling);
		FN_STACK f1 = pop(stack);
		int flag1=0;
		int hashval1 = lookupEntry("global", f1->var_name, f1->field_name);
		if(hashval1==-1) {
			hashval1 = lookupEntry("_main", f1->var_name, f1->field_name);
		}
		if(hashval1==-1) {
			if(f1->var_name==NULL || f1->var_name[0]!='t') {
				flag1=0;
			} else {
				int right;
				sscanf( f1->var_name, "temp%d_%d", &hashval1,&right);
				flag1=1;
			}
		} else {
			SYMBOLENTRY t = h[hashval1].entry_ptr;
			if(f1->field_name==NULL) {
				if((t->int_no+t->real_no)>1) {
					flag1=1;
				}
			} else {
				flag1=2;
			}
		}

		FN_STACK f2 = pop(stack);
		int flag2=0;
		int hashval2 = lookupEntry("global", f2->var_name, f2->field_name);
		if(hashval2==-1) {
			hashval2 = lookupEntry("_main", f2->var_name, f2->field_name);
		}
		if(hashval2==-1) {
			if(f2->var_name==NULL || f2->var_name[0]!='t') {
				flag2=0;
			} else {
				int right;
				sscanf( f2->var_name, "temp%d_%d", &hashval2,&right);
				flag2=1;
			}
		} else {
			SYMBOLENTRY t = h[hashval2].entry_ptr;
			if(f2->field_name==NULL) {
				if((t->int_no+t->real_no)>1) {
					flag2=1;
				}
			} else {
				flag2=2;
			}
		}
		printf("pop\tbx\npop\tax\n");
		if(strcmp(temp->tokens->tk->lexeme, "+")==0) {
			if(flag1==1 && flag2==1) {

				SYMBOLENTRY t = h[hashval1].entry_ptr;
				int hashval = lookupEntry("record", t->record_name, NULL);
				int right=0;
				if(temp->sibling==NULL) {
					right=1;
				}
				FIELD f = h[hashval].entry_ptr->record;
				while(f!=NULL) {
					printf("mov\tcx,[ax+%s]\n",f->fieldname);
					printf("mov\tdx,[bx+%s]\n",f->fieldname);
					printf("add\tcx,dx\n");
					printf("mov\t[temp%d_%d+%s],cx\n",hashval,right,f->fieldname);
					f=f->next;
				}
				printf("mov\tax,temp%d_%d\n",hashval,right);
				printf("push\tax\n");
				char rec[20];
				sprintf(rec, "temp%d_%d", hashval,right);
				stack = push(stack, rec, NULL);

			} else {
				printf("add\tax,bx\n");
				printf("push\tax\n");
				stack = push(stack, NULL, NULL);
			}
		}
		else if(strcmp(temp->tokens->tk->lexeme, "-")==0) {
			if(flag1==1 && flag2==1) {

				SYMBOLENTRY t = h[hashval1].entry_ptr;
				int hashval = lookupEntry("record", t->record_name, NULL);
				int right=0;
				if(temp->sibling==NULL) {
					right=1;
				}
				FIELD f = h[hashval].entry_ptr->record;
				while(f!=NULL) {
					printf("mov\tcx,[ax+%s]\n",f->fieldname);
					printf("mov\tdx,[bx+%s]\n",f->fieldname);
					printf("sub\tcx,dx\n");
					printf("mov\t[temp%d_%d+%s],cx\n",hashval,right,f->fieldname);
					f=f->next;
				}
				printf("mov\tax,temp%d_%d\n",hashval,right);
				printf("push\tax\n");
				char* rec = (char*)malloc(sizeof(char)*20);
				sprintf(rec, "temp%d_%d", hashval,right);
				stack = push(stack, rec, NULL);
			} else {
				printf("sub\tax,bx\n");
				printf("push\tax\n");
				stack = push(stack, NULL, NULL);
			}
		}
		else if(strcmp(temp->tokens->tk->lexeme, "*")==0) {
			if(flag1==1 && flag2==0) {
				// bx record
				SYMBOLENTRY t = h[hashval1].entry_ptr;
				int hashval = lookupEntry("record", t->record_name, NULL);
				int right=0;
				if(temp->sibling==NULL) {
					right=1;
				}
				FIELD f = h[hashval].entry_ptr->record;
				while(f!=NULL) {
					printf("push\tax\n");
					printf("mov\tcx,[bx+%s]\n",f->fieldname);
					printf("mul\tcx\n");
					printf("mov\t[temp%d_%d+%s],ax\n",hashval,right,f->fieldname);
					printf("pop\tax\n");
					f=f->next;
				}
				printf("mov\tax,temp%d_%d\n",hashval,right);
				printf("push\tax\n");
				char* rec = (char*)malloc(sizeof(char)*20);
				sprintf(rec, "temp%d_%d", hashval,right);
				stack = push(stack, rec, NULL);
			} else if(flag1==0 && flag2==1) {
				// ax record
				SYMBOLENTRY t = h[hashval2].entry_ptr;
				int hashval = lookupEntry("record", t->record_name, NULL);
				int right=0;
				if(temp->sibling==NULL) {
					right=1;
				}
				FIELD f = h[hashval].entry_ptr->record;
				while(f!=NULL) {
					printf("push\tax\n");
					printf("mov\tax,[ax+%s]\n",f->fieldname);
					printf("mul\tbx\n");
					printf("mov\t[temp%d_%d+%s],ax\n",hashval,right,f->fieldname);
					printf("pop\tax\n");
					f=f->next;
				}
				printf("mov\tax,temp%d_%d\n",hashval,right);
				printf("push\tax\n");
				char* rec = (char*)malloc(sizeof(char)*20);
				sprintf(rec, "temp%d_%d", hashval,right);
				stack = push(stack, rec, NULL);
			} else {
				printf("mul\tbx\n");
				printf("push\tax\n");
				stack = push(stack, NULL, NULL);
			}
		}
		else if(strcmp(temp->tokens->tk->lexeme, "/")==0) {
			if(flag1==1 && flag2==0) {
				// bx record
				SYMBOLENTRY t = h[hashval1].entry_ptr;
				int hashval = lookupEntry("record", t->record_name, NULL);
				int right=0;
				if(temp->sibling==NULL) {
					right=1;
				}
				FIELD f = h[hashval].entry_ptr->record;
				while(f!=NULL) {
					printf("push\tax\n");
					printf("mov\tcx,[bx+%s]\n",f->fieldname);
					printf("div\tcx\n");
					printf("mov\t[temp%d_%d+%s],ax\n",hashval,right,f->fieldname);
					printf("pop\tax\n");
					f=f->next;
				}
				printf("mov\tax,temp%d_%d\n",hashval,right);
				printf("push\tax\n");
				char* rec = (char*)malloc(sizeof(char)*20);
				sprintf(rec, "temp%d_%d", hashval,right);
				stack = push(stack, rec, NULL);
			} else if(flag1==0 && flag2==1) {
				// ax record
				SYMBOLENTRY t = h[hashval2].entry_ptr;
				int hashval = lookupEntry("record", t->record_name, NULL);
				int right=0;
				if(temp->sibling==NULL) {
					right=1;
				}
				FIELD f = h[hashval].entry_ptr->record;
				while(f!=NULL) {
					printf("push\tax\n");
					printf("mov\tax,[ax+%s]\n",f->fieldname);
					printf("div\tbx\n");
					printf("mov\t[temp%d_%d+%s],ax\n",hashval,right,f->fieldname);
					printf("pop\tax\n");
					f=f->next;
				}
				printf("mov\tax,temp%d_%d\n",hashval,right);
				printf("push\tax\n");
				char* rec = (char*)malloc(sizeof(char)*20);
				sprintf(rec, "temp%d_%d", hashval,right);
				stack = push(stack, rec, NULL);
			} else {
				printf("div\tbx\n");
				printf("push\tax\n");
				stack = push(stack, NULL, NULL);
			}
		}
	}

	return;

}

void stmtrecursion(NODE_AstTree stmt) {
	while(stmt!=NULL) {
		// printf("%s\n",stmt->tokens->tk->lexeme);
		if(stmt->tokens->tk->token==TK_READ) {
			int hashval;
			int flag=0;
			// printf("%d\n",flag);

			if(stmt->child->tokens->next==NULL) {
				hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme, NULL);
                if(hashval==-1) {
                    hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme, NULL);
                }
				SYMBOLENTRY temp = h[hashval].entry_ptr;

				if((temp->real_no+temp->int_no)>1) {
					flag=1;
				}
			} else {
				hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme, stmt->child->tokens->next->tk->lexeme);
                if(hashval==-1) {
                    hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme, stmt->child->tokens->next->tk->lexeme);
                }
                flag=2;
			}
			if(flag==0) {
				printf("call\tread\n");
				printf("mov\t[%s],ax\n",stmt->child->tokens->tk->lexeme);
			} else if(flag==1) {
				FIELD temp = h[hashval].entry_ptr->record;
				while(temp!=NULL) {
					printf("call\tread\n");
					printf("mov\t[%s+%s],ax\n",stmt->child->tokens->tk->lexeme,temp->fieldname);
					temp=temp->next;
				}
			} else {
				printf("call\tread\n");
				printf("mov\t[%s+%s],ax\n",stmt->child->tokens->tk->lexeme,stmt->child->tokens->next->tk->lexeme);

			}

		}

		else if(strcmp(stmt->tokens->tk->lexeme, "write")==0) {

			int hashval;
			int flag=0;
			// printf("%d\n",flag);

			if(stmt->child->tokens->next==NULL) {
				hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme, NULL);
                if(hashval==-1) {
                    hashval = lookupEntry("global", stmt->child->tokens->tk->lexeme, NULL);
                }
				SYMBOLENTRY temp = h[hashval].entry_ptr;

				if((temp->real_no+temp->int_no)>1) {
					flag=1;
				}
			} else {
				hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme, stmt->child->tokens->next->tk->lexeme);
                if(hashval==-1) {
                    hashval = lookupEntry("global", stmt->child->tokens->tk->lexeme, stmt->child->tokens->next->tk->lexeme);
                }
                flag=2;
			}
			if(flag==0) {
				printf("mov\tax,[%s]\n",stmt->child->tokens->tk->lexeme);
				printf("call\twrite\n");

			} else if(flag==1) {
				FIELD temp = h[hashval].entry_ptr->record;
				while(temp!=NULL) {
					printf("mov\tax,[%s+%s]\n",stmt->child->tokens->tk->lexeme,temp->fieldname);
					printf("call\twrite\n");

					temp=temp->next;
				}
			} else {
				printf("mov\tax,[%s+%s]\n",stmt->child->tokens->tk->lexeme,stmt->child->tokens->next->tk->lexeme);
				printf("call\twrite\n");

			}

			// printf("%d  %s \n",stmt->child->tokens->tk->dataType, stmt->child->tokens->tk->token);
		}
		else if(strcmp(stmt->tokens->tk->lexeme, "while")==0) {
			// printf("while\n");
			int a = global_count;
			printf("label%d:\n",global_count++);

			booleanTrav(stmt->child);

			printf("pop\tax\ncmp\tax,1\n");
			int y = global_count;
			printf("jne\tlabel%d\n",global_count++);
			stmtrecursion(stmt->child->sibling->child);
			printf("jmp\tlabel%d\n",a);

			printf("label%d:\n",y);

		}
		else if(strcmp(stmt->tokens->tk->lexeme, "if")==0) {
			// printf("if\n");
			booleanTrav(stmt->child);
			// printf("ifend\n");

			printf("pop\tax\ncmp\tax,1\n");
			int x = global_count;
			printf("jne\tlabel%d\n",global_count++);
			stmtrecursion(stmt->child->sibling->child);
			int y = global_count;
			if(stmt->child->sibling->sibling!=NULL) {
				printf("jmp\tlabel%d\n",global_count++);
			}
			printf("label%d:\n",x);
			if(stmt->child->sibling->sibling!=NULL) {
				stmtrecursion(stmt->child->sibling->sibling->child);
				printf("label%d:\n",y);
			}

		} else {
            int hashval;
            int flag=0;
            if(stmt->child->tokens->next==NULL) {
                hashval = lookupEntry("global", stmt->child->tokens->tk->lexeme,NULL);
                if(hashval==-1) {
                    hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme,NULL);
                }
                SYMBOLENTRY se = h[hashval].entry_ptr;
                if(se->int_no+se->real_no>1) {
                    flag=1;
                }
            } else {
                hashval = lookupEntry("global", stmt->child->tokens->tk->lexeme,stmt->child->tokens->next->tk->lexeme);
                if(hashval==-1) {
                    hashval = lookupEntry("_main", stmt->child->tokens->tk->lexeme,stmt->child->tokens->next->tk->lexeme);
                }
                flag=2;
            }
            if(flag==0) {
                arithmeticTrav(stmt->child->sibling);
                printf("pop\tax\n");
                printf("mov\t[%s],ax\n",stmt->child->tokens->tk->lexeme);
            } else if(flag==1) {
                arithmeticTrav(stmt->child->sibling);
                printf("pop\tax\n");
                FIELD f = h[hashval].entry_ptr->record;
                while(f!=NULL) {
                    printf("mov\tbx,[ax+%s]\n",f->fieldname);
                    printf("mov\t[%s+%s],bx\n",stmt->child->tokens->tk->lexeme,f->fieldname);

                    f=f->next;
                }
            } else {
                arithmeticTrav(stmt->child->sibling);
                printf("pop\tax\n");
                printf("mov\t[%s+%s],ax\n",stmt->child->tokens->tk->lexeme,stmt->child->tokens->next->tk->lexeme);
            }

        }
		stmt = stmt->sibling;
	}

}

void booleanTrav(NODE_AstTree temp){
	if(temp->child==NULL) {

		if(temp->tokens->next==NULL) {
            if(temp->tokens->tk->token==TK_NUM) {
                printf("mov\tax,%s\n",temp->tokens->tk->lexeme);
                printf("push\tax\n");
            } else {
                printf("mov\tax,[%s]\n",temp->tokens->tk->lexeme);
    			printf("push\tax\n");
            }

		} else {
			printf("mov\tax,[%s+%s]\n",temp->tokens->tk->lexeme,temp->tokens->next->tk->lexeme);
			printf("push\tax\n");
		}
	} else {
		booleanTrav(temp->child);
		booleanTrav(temp->child->sibling);
		// printf("   %s\n",temp->tokens->tk->lexeme);
		if(strcmp(temp->tokens->tk->lexeme,"<")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("cmp\tax,bx\n");
			printf("jge\tlabel%d\n",global_count++);
			printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
			printf("label%d:\nmov\tax,0\npush\tax\nlabel%d:\n",global_count-2,global_count-1);
		}
		else if(strcmp(temp->tokens->tk->lexeme,"<=")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("cmp\tax,bx\n");
			printf("jg\tlabel%d\n",global_count++);
			printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
			printf("label%d:\nmov\tax,0\npush\tax\nlabel%d:\n",global_count-2,global_count-1);

		}
		else if(strcmp(temp->tokens->tk->lexeme,"==")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("cmp\tax,bx\n");
			printf("jne\tlabel%d\n",global_count++);
			printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
			printf("label%d:\nmov\tax,0\npush\tax\nlabel%d:\n",global_count-2,global_count-1);

		}
		else if(strcmp(temp->tokens->tk->lexeme,">")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("cmp\tax,bx\n");
			printf("jle\tlabel%d\n",global_count++);
			printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
			printf("label%d:\nmov\tax,0\npush\tax\nlabel%d:\n",global_count-2,global_count-1);

		}
		else if(strcmp(temp->tokens->tk->lexeme,">=")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("cmp\tax,bx\n");
			printf("jl\tlabel%d\n",global_count++);
			printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
			printf("label%d:\nmov\tax,0\npush\tax\nlabel%d:\n",global_count-2,global_count-1);

		}
		else if(strcmp(temp->tokens->tk->lexeme,"!=")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("cmp\tax,bx\n");
			printf("je\tlabel%d\n",global_count++);
			printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
			printf("label%d:\nmov\tax,0\npush\tax\nlabel%d:\n",global_count-2,global_count-1);

		}
		else if(strcmp(temp->tokens->tk->lexeme,"@@@")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("or\tax,bx\npush\tax\n");
		}
		else if(strcmp(temp->tokens->tk->lexeme,"&&&")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("and\tax,bx\npush\tax\n");
		}

		else if(temp->tokens->tk->token == TK_ASSIGNOP) {
			printf("pop\tbx\npop\tax\n");
			printf("mov\tax,bx\n");
			printf("push\tax\n");
		}
		else if(strcmp(temp->tokens->tk->lexeme, "+")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("add\tax,bx\n");
			printf("push\tax\n");
		}
		else if(strcmp(temp->tokens->tk->lexeme, "-")==0) {
			printf("pop\tax\npop\tax\n");
			printf("sub\tax,bx\n");
			printf("push\tax\n");
		}
		else if(strcmp(temp->tokens->tk->lexeme, "*")==0) {
			printf("pop\tbx\npop\tax\n");
			printf("mul\tbx\n");
			printf("push\tax\n");
		}
		else if(strcmp(temp->tokens->tk->lexeme, "/")==0) {
			printf("pop\tax\npop\tbx\n");
			printf("div\tbx\n");
			printf("push\tax\n");
		}
	}

	return;

}
void codeGeneration(NODE_AstTree nt){
	// printf("%s  %d\n", global_table->name, global_table->no_entries);
	// SYMBOLENTRY s = global_table->head;
	// // while(s!=NULL){
	// //   printf("width:%d offset:%d int_no:%d real_no:%d ",s->width, s->offset, s->int_no, s->real_no);
	// //   FIELD f = s->record;
	// //   if(f!=NULL){
	// //     printf("record:%s ",s->id);
	// //   }
	// //   while(f!=NULL){
	// //     printf("field:%s ",f->fieldname);
	// //     f=f->next;
	// //   }
	// //   s=s->next;
	// //   printf("\n");
	// // }

	int fd = open("code.asm", O_CREAT|O_RDWR, 0666);
	dup2(fd,1);
	printf( "SECTION\t.bss\n\n");
	SYMBOLENTRY se = record_table->head;
	while(se!=NULL) {
		FIELD fe = se->record;
		char *id = se->id;
		char *id2 = id;
		id2++;
		if(fe!=NULL) {
			printf( "struc\t%s\n", id2);
		}
		while(fe!=NULL) {
			char *res;
			if(fe->dType == INT) {
				res="resw";
			}
			else{
				res="resd";
			}
			printf( "%s\t%s 1\n",fe->fieldname,res);
			fe=fe->next;
		}
		if(id!=NULL) {
			printf( "endstruc\n\n");

		}

		int hashval = lookupEntry("record", se->id, NULL);

		int x=0;
		while(x<2) {
			printf("temp%d_%d\tistruc\t%s\n",hashval,x,id2);
			fe = se->record;
			char *rec = se->record_name, *rec2;
			if(rec!=NULL) {

				rec2 = rec;
				rec2++;

				while(fe!=NULL) {
					char *res;
					if(fe->dType == INT) {
						res="resw";
					}
					else{
						res="resd";
					}
					printf( "at\t%s,\t%s 1\n",fe->fieldname,res);
					fe=fe->next;
				}
				if(rec!=NULL) {
					printf( "iend\n\n");
				}
			}
			x++;
		}
		se =se->next;
	}

	SYMBOLENTRY se2 = global_table->head;
	while(se2!=NULL) {
		FIELD fe = se2->record;
		char *rec = se2->record_name, *rec2;
		if(rec!=NULL) {

			rec2 = rec;
			rec2++;
			if(fe!=NULL ) {
				printf( "\n%s\tistruc\t%s\n", se2->id, rec2);
			}
			while(fe!=NULL) {
				char *res;
				if(fe->dType == INT) {
					res="resw";
				}
				else{
					res="resd";
				}
				printf( "at\t%s,\t%s 1\n",fe->fieldname,res);
				fe=fe->next;
			}
			if(rec!=NULL) {
				printf( "iend\n\n");
			}
		}
		else if(rec==NULL) {

			while(fe!=NULL) {
				char *res;
				if(fe->dType == INT) {
					res="resw";
				}
				else{
					res="resd";
				}
				printf( "%s\t%s 1\n", se2->id, res);
				fe = fe->next;
			}
		}
		se2 =se2->next;
	}
	long long int hashval = getFunction("_main");
//printf("%lld\n",hashval);
	se2 = functions[hashval].st->head;
	while(se2!=NULL) {
		FIELD fe = se2->record;
		char *rec = se2->record_name, *rec2;
		if(rec!=NULL) {

			rec2 = rec;
			rec2++;
			if(fe!=NULL ) {
				printf( "\n%s\tistruc\t%s\n", se2->id, rec2);
			}
			while(fe!=NULL) {
				char *res;
				if(fe->dType == INT) {
					res="resw";
				}
				else{
					res="resd";
				}
				printf( "at\t%s,\t%s 1\n",fe->fieldname,res);
				fe=fe->next;
			}
			if(rec!=NULL) {
				printf( "iend\n\n");
			}
		}
		else if(rec==NULL) {

			while(fe!=NULL) {
				char *res;
				if(fe->dType == INT) {
					res="resw";
				}
				else{
					res="resd";
				}
				printf( "%s\t%s 1\n", se2->id, res);
				fe = fe->next;
			}
		}
		se2 =se2->next;
	}

    printf("sinput\tresb\t16");
	printf("\nSECTION\t.data\n");
    printf("negative\tdb\t'-'");
	printf("\nglobal\t_start\n");

//asm code for write
	printf("write:\n");
	printf("\tmov\t\tcx,ax\n");
	printf("\tmov\t\tax,0Ah\n");
	printf("\tpush\t\tax\n");
	printf("\tmov\t\tax,cx\n");
	printf("\tmov\t\tcx,1\n");
	printf("\tcmp\t\tax,0\n");
	printf("\tjge\t\tLabel1\n");
	printf("\tneg\t\tax\n");
	printf("\tpush\t\tcx\n");
	printf("\tpush\t\tax\n");
	printf("\tmov\t\tedx,1\n");
	printf("\tmov\t\tecx,negative\n");
	printf("\tmov\t\tebx,1\n");
	printf("\tmov\t\teax,4\n");
	printf("\tint\t\t80h\n");
	printf("\tpop\t\tax\n");
	printf("\tpop\t\tcx\n");

	printf("Label1:\n");
	printf("\tinc\t\tcx\n");
	printf("\tmov\t\tsi,10\n");
	printf("\tmov\t\tdx,0h\n");
	printf("\tidiv\t\tsi\n");
	printf("\tadd\t\tdx,48\n");
	printf("\tpush\t\tdx\n");
	printf("\tcmp\t\tax,0\n");
	printf("\tjnz\t\tLabel1\n");

	printf("Label2:\n");
	printf("\tmov\t\teax,esp\n");
	printf("\tpush\t\tcx\n");
	printf("\tmov\t\tedx,1\n");
	printf("\tmov\t\tecx,eax\n");
	printf("\tmov\t\tebx,1\n");
	printf("\tmov\t\teax,4\n");
	printf("\tint\t\t80h\n");
	printf("\tpop\t\tcx\n");
	printf("\tdec\t\tcx\n");
	printf("\tpop\t\tax\n");
	printf("\tcmp\t\tcx,0\n");
	printf("\tjnz\t\tLabel2\n");
	printf("\tret\n");

//asm code for read
	printf("read:\n");
	printf("\tmov\t\tax,0h\n");
	printf("\tmov\t\t[sinput],ax\n");
	printf("\tmov\t\tedx,16\n");
	printf("\tmov\t\tecx,sinput\n");
	printf("\tmov\t\tebx,0\n");
	printf("\tmov\t\teax,3\n");
	printf("\tint\t\t80h\n");
	printf("\tmov\t\tax,0h\n");
	printf("\tpush\t\tax\n");
	printf("\tmov\t\tsi,10\n");
	printf("\tmov\t\tbl,[sinput]\n");
	printf("\tmov\t\tecx,sinput\n");
	printf("\tcmp\t\tbl,[negative]\n");
	printf("\tjnz\t\tLabel6\n");
	printf("\tinc\t\tecx\n");
	printf("\tmov\t\tbl,[ecx]\n");
	printf("\tpop\t\tdx\n");
	printf("\tmov\t\tdx,1h\n");
	printf("\tpush\t\tdx\n");

	printf("Label6:\n");
	printf("\tsub\t\tbl,30h\n");
	printf("\tmov\t\tbh,0h\n");
	printf("\tmul\t\tsi\n");
	printf("\tadd\t\tax,bx\n");
	printf("\tinc\t\tecx\n");
	printf("\tmov\t\tbl,[ecx]\n");
	printf("\tcmp\t\tbl,0ah\n");
	printf("\tjnz\t\tLabel6\n");
	printf("\tpop\t\tdx\n");
	printf("\tcmp\t\tdx,0\n");
	printf("\tje\t\tLabel7\n");
	printf("\tneg\t\tax\n");

	printf("Label7:\n");
	printf("\tret\n\n");




	printf("_start:\n");

	NODE_AstTree stmt = nt->child->sibling->sibling->child;
	stmtrecursion(stmt);

    printf("mov\tebx,0\n");
    printf("mov\teax,1\n");
    printf("int\t80h\n");
	close(1);
}
