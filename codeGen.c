#include "codeGenDef.h"

extern HASHSYMBOL h;
extern int hash_size;
extern FN_ENTRY functions;
extern int fn_size;
extern SYMBOLTABLE record_table;
extern SYMBOLTABLE global_table;

int global_count=0;
void booleanTrav(NODE_AstTree temp){
  if(temp->child != NULL){
     booleanTrav(temp->child);
 }
 else{
   if(temp->tokens!=NULL){
     printf("push\t%s\n",temp->tokens->tk->lexeme);

     // if(temp->tokens->tk->lexeme == "<"){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("cmp\tax,bx\n");
     // }
     // else if(temp->tokens->tk->lexeme == "<="){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("cmp\tax,bx\n");
     // }
     // else if(temp->tokens->tk->lexeme == "=="){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("cmp\tax,bx\n");
     // }
     // else if(temp->tokens->tk->lexeme == ">"){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("cmp\tax,bx\n");
     // }
     // else if(temp->tokens->tk->lexeme == ">="){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("cmp\tax,bx\n");
     // }
     // else if(temp->tokens->tk->lexeme == "!="){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("cmp\tax,bx\n");
     // }
     // else if(temp->tokens->tk->lexeme == "@@@"){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("or\tax,bx\n");
     // }
     // else if(temp->tokens->tk->lexeme == "&&&"){
     //   printf("pop\tax\npop\tbx\n");
     //   printf("and\tax,bx\n");
     // }
   }
   NODE_AstTree temp2=temp->sibling;
   if(temp2 != NULL){
       booleanTrav(temp2);
       temp2 = temp2->sibling;
   }
 }
 if(temp->child != NULL){
   if(temp->tokens!=NULL){
     printf("   %s\n",temp->tokens->tk->lexeme);
     if(strcmp(temp->tokens->tk->lexeme,"<")==0){
       printf("pop\tbx\npop\tax\n");
       printf("cmp\tax,bx\n");
       printf("jge\tlabel%d\n",global_count++);
       printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
       printf("label%d:\nmov\tax,0\npush\tax\nlabel%d\n",global_count-2,global_count-1);
     }
     else if(strcmp(temp->tokens->tk->lexeme,"<=")==0){
       printf("pop\tbx\npop\tax\n");
       printf("cmp\tax,bx\n");
       printf("jg\tlabel%d\n",global_count++);
       printf("mov\tax,1\npush\tax\njmp\tlabel%d\n",global_count++);
       printf("label%d:\nmov\tax,0\npush\tax\nlabel%d:\n",global_count-2,global_count-1);

     }
     else if(strcmp(temp->tokens->tk->lexeme,"==")==0){
       printf("pop\tax\npop\tbx\n");
       printf("cmp\tax,bx\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme,">")==0){
       printf("pop\tax\npop\tbx\n");
       printf("cmp\tax,bx\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme,">=")==0){
       printf("pop\tax\npop\tbx\n");
       printf("cmp\tax,bx\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme,"!=")==0){
       printf("pop\tax\npop\tbx\n");
       printf("cmp\tax,bx\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme,"@@@")==0){
       printf("pop\tax\npop\tbx\n");
       printf("or\tax,bx\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme,"&&&")==0){
       printf("pop\tax\npop\tbx\n");
       printf("and\tax,bx\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme, "read")==0){
       printf("mov\t[%s],ax\n",temp->tokens->tk->lexeme);
       printf("call\tread\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme, "write")==0){
       printf("mov\tax,[%s]\n",temp->tokens->tk->lexeme);
       printf("call\twrite\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme, "<---")==0){
       printf("pop\tax\npop\tbx\n");
       printf("mov\tax,bx\n");
       printf("push\tax\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme, "+")==0){
       printf("pop\tax\npop\tbx\n");
       printf("add\tax,bx\n");
       printf("push\tax\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme, "-")==0){
       printf("pop\tax\npop\tbx\n");
       printf("sub\tax,bx\n");
       printf("push\tax\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme, "*")==0){
       printf("pop\tax\npop\tbx\n");
       printf("mul\tax,bx\n");
       printf("push\tax\n");
     }
     else if(strcmp(temp->tokens->tk->lexeme, "/")==0){
       printf("pop\tax\npop\tbx\n");
       printf("div\tax,bx\n");
       printf("push\tax\n");
     }
   }
 }
 if(temp->child!=NULL){
   // temp=temp->sibling;
   if(temp->sibling != NULL){

       booleanTrav(temp->sibling);
       temp = temp->sibling;
   }
 }

 return ;

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

  FILE *fp = fopen("code.asm","a+");
  printf( "SECTION\t.bss\n\n");
  SYMBOLENTRY se = record_table->head;
  while(se!=NULL){
    FIELD fe = se->record;
    char *id = se->id;
    char *id2 = id;
    id2++;
    if(fe!=NULL){
      printf( "struc\t%s\n", id2);
    }
    while(fe!=NULL){
      char *res;
      if(fe->dType == INT){
        res="resw";
      }
      else{
        res="resd";
      }
      printf( "%s\t%s 1\n",fe->fieldname,res);
      fe=fe->next;
    }
    if(id!=NULL){
        printf( "endstruc\n\n");
    }

    se =se->next;
  }

  SYMBOLENTRY se2 = global_table->head;
  while(se2!=NULL){
    FIELD fe = se2->record;
    char *rec = se2->record_name, *rec2;
    if(rec!=NULL){

      rec2 = rec;
      rec2++;
      if(fe!=NULL ){
        printf( "\n%s\tistruc\t%s\n", se2->id, rec2);
      }
      while(fe!=NULL){
        char *res;
        if(fe->dType == INT){
          res="resw";
        }
        else{
          res="resd";
        }
        printf( "at\t%s,\t%s 1\n",fe->fieldname,res);
        fe=fe->next;
      }
      if(rec!=NULL){
          printf( "iend\n\n");
      }
    }
    else if(rec==NULL){

      while(fe!=NULL){
        char *res;
        if(fe->dType == INT){
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
  while(se2!=NULL){
    FIELD fe = se2->record;
    char *rec = se2->record_name, *rec2;
    if(rec!=NULL){

      rec2 = rec;
      rec2++;
      if(fe!=NULL ){
        printf( "\n%s\tistruc\t%s\n", se2->id, rec2);
      }
      while(fe!=NULL){
        char *res;
        if(fe->dType == INT){
          res="resw";
        }
        else{
          res="resd";
        }
        printf( "at\t%s,\t%s 1\n",fe->fieldname,res);
        fe=fe->next;
      }
      if(rec!=NULL){
          printf( "iend\n\n");
      }
    }
    else if(rec==NULL){

      while(fe!=NULL){
        char *res;
        if(fe->dType == INT){
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


	printf("\nSECTION\t.data\n");

	printf("\nglobal\t_start\n");
	printf("_start:\n");

  NODE_AstTree stmt = nt->child->sibling->sibling->child;
  while(stmt!=NULL){
    booleanTrav(stmt->child);

    if(strcmp(stmt->tokens->tk->lexeme, "read")==0){
      printf("mov\t[%s],ax\n",stmt->child->tokens->tk->lexeme);
      printf("call\tread\n");
    }
    else if(strcmp(stmt->tokens->tk->lexeme, "write")==0){
      // printf("%d  %s \n",stmt->child->tokens->tk->dataType, stmt->child->tokens->tk->token);
      printf("mov\tax,[%s]\n",stmt->child->tokens->tk->lexeme);
      printf("call\twrite\n");
    }
    else if(strcmp(stmt->tokens->tk->lexeme, "while")==0){
      printf("while\n");
      booleanTrav(stmt->child);
    }
    else if(strcmp(stmt->tokens->tk->lexeme, "if")==0){
      printf("if\n");
    }
    stmt = stmt->sibling;
  }


	fclose(fp);
}
