#include "codeGenDef.h"

extern HASHSYMBOL h;
extern int hash_size;
extern FN_ENTRY functions;
extern int fn_size;
extern SYMBOLTABLE record_table;
extern SYMBOLTABLE global_table;

void codeGeneration(NODE_AstTree nt){
  printf("%s  %d\n", global_table->name, global_table->no_entries);
  SYMBOLENTRY s = global_table->head;
  while(s!=NULL){
    printf("width:%d offset:%d int_no:%d real_no:%d ",s->width, s->offset, s->int_no, s->real_no);
    FIELD f = s->record;
    if(f!=NULL){
      printf("record:%s ",s->id);
    }
    while(f!=NULL){
      printf("field:%s ",f->fieldname);
      f=f->next;
    }
    s=s->next;
    printf("\n");
  }

  FILE *fp = fopen("code.asm","a+");
  printf( "SECTION\t.bss\n");
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
        printf( "endstruc\n");
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
        printf( "%s\tistruc\t%s\n", rec, rec2);
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
          printf( "iend\n");
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
  fclose(fp);
}
