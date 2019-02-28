#include "parser.h"

int Hash_function(char* s) {
    int count=0;
    for(int i=0;i<strlen(s);i++) {
        count=(count+s[i]-'A')%HASH_SIZE;
    }
    return count;
}

HASH_BLOCK checkexisting(char* name, GRAMMAR g) {
    int val = Hash_function(name);
    HASH_BLOCK t=(g->hash_table)[val];
    if(t==NULL) {
        // printf("%s\n",name);
        HASH_BLOCK temp;
        temp=(HASH_BLOCK)malloc(sizeof(hash_block));
        temp->name=(char*)malloc(sizeof(char)*(strlen(name)+1));
        strcpy(temp->name,name);
        temp->next=NULL;
        temp->r=NULL;
        (g->hash_table)[val]=temp;
        return temp;
    } else {
        if(strcmp(t->name,name)==0) {
            return t;
        } else {
            while(t->next!=NULL) {
                if(strcmp(t->next->name,name)==0) {
                    return t->next;
                }
                t=t->next;
            }
            HASH_BLOCK temp;
            temp=(HASH_BLOCK)malloc(sizeof(hash_block));
            temp->name=(char*)malloc(sizeof(char)*(strlen(name)+1));
            strcpy(temp->name,name);
            temp->next=NULL;
            temp->r=NULL;
            t->next=temp;
            return temp;
        }
    }
}

GRAMMAR populateGrammar(char* grammar_file) {
    FILE* fp = fopen(grammar_file, "r");
    if(fp==NULL){
		printf("Could not open Grammar file");
		return NULL;
    }
    char c;
    char line[255];

    GRAMMAR g;
    g = (GRAMMAR)malloc(sizeof(grammar));

    while(1) {
        fscanf(fp,"%[^\t\n]s\n",line);
        c = fgetc(fp);
        if(c==EOF) {
            break;
        }
        if(line[0]=='\0') {
            continue;
        }
        int left=1;
        char token[255];
        int token_ptr=0;
        HASH_BLOCK h_temp;
        RULE r_temp;
        TK_NODE tk_temp=NULL;
        int is_term=-1;
        line[strlen(line)+1]='\0';
        line[strlen(line)]=' ';
        for(int i=0;line[i]!='\0';i++) {
        //    printf("%c  ",line[i]);
            if(left==1 && (line[i]=='\t' || line[i]==' ')) {
                continue;
            }
            if(left==1) {
                if(line[i]=='<') {
                    token_ptr=0;
                } else if(line[i]=='>') {
                    token[token_ptr]='\0';
                    left=0;
                    h_temp = checkexisting(token, g);
                    if(h_temp->r==NULL) {
                        h_temp->r = (RULE)malloc(sizeof(rule));
                        r_temp=h_temp->r;
                    } else {
                        RULE r_temp2=h_temp->r;
                        while(r_temp2->next!=NULL) {
                            r_temp2 = r_temp2->next;
                        }
                        r_temp = (RULE)malloc(sizeof(rule));
                        r_temp2->next=r_temp;
                    }
                    token_ptr=0;

                } else {
                    token[token_ptr]=line[i];
                    token_ptr++;

                }
            } else {
                if(line[i]=='=') {
                    continue;
                }
                if(token_ptr==0 && line[i]=='>') {
                    continue;
                }
                if(is_term==1) {
                    if(line[i]=='<' || line[i]=='\t' || line[i]=='|' || line[i]==' ') {
                        token[token_ptr]='\0';
                        if(tk_temp==NULL) {
                            r_temp->start=(TK_NODE)malloc(sizeof(tk_node));
                            r_temp->start->next=NULL;
                            (r_temp->start->info).term_name = (char*)malloc(sizeof(char)*(strlen(token)+1));
                            strcpy((r_temp->start->info).term_name, token);
                            tk_temp = r_temp->start;
                        } else {
                            tk_temp->next=(TK_NODE)malloc(sizeof(tk_node));
                            tk_temp=tk_temp->next;
                            tk_temp->next=NULL;
                            (tk_temp->info).term_name = (char*)malloc(sizeof(char)*(strlen(token)+1));
                            strcpy((tk_temp->info).term_name, token);
                        }
                        token_ptr=0;
                        if(line[i]=='<') {
                            is_term=0;
                        } else if(line[i]=='\t' || line[i]==' ') {
                            if(line[i+1]=='\0') {
                                break;
                            } else {
                                is_term=-1;
                            }
                        } else {
                            tk_temp=NULL;
                            r_temp->next=(RULE)malloc(sizeof(rule));
                            r_temp=r_temp->next;
                            r_temp->next=NULL;
                            is_term=-1;
                        }
                    } else {
                        token[token_ptr]=line[i];
                        token_ptr++;
                    }
                } else if(is_term==0) {
                    if(line[i]=='>') {
                        token[token_ptr]='\0';
                        if(tk_temp==NULL) {
                            r_temp->start=(TK_NODE)malloc(sizeof(tk_node));
                            r_temp->start->next=NULL;
                            (r_temp->start->info).non_term=checkexisting(token, g);
                            tk_temp = r_temp->start;
                        } else {
                            tk_temp->next=(TK_NODE)malloc(sizeof(tk_node));
                            tk_temp=tk_temp->next;
                            tk_temp->next=NULL;
                            (tk_temp->info).non_term=checkexisting(token, g);
                        }
                        is_term=-1;
                        token_ptr=0;
                    } else {
                        token[token_ptr]=line[i];
                        token_ptr++;
                    }
                } else {
                    if(line[i]=='\t' || line[i]==' ') {
                        continue;
                    }
                    if(line[i]=='<') {
                        is_term=0;
                        token_ptr=0;
                    } else if(line[i]=='|') {
                        tk_temp=NULL;
                        r_temp->next=(RULE)malloc(sizeof(rule));
                        r_temp=r_temp->next;
                        r_temp->next=NULL;
                        is_term=-1;
                        token_ptr=0;
                    } else {
                        is_term=1;
                        token_ptr=0;
                        token[token_ptr]=line[i];
                        token_ptr++;
                    }
                }
            }

        }
        // printf("%s\n",token);
        // temp->index
        printf("%s\n",line);

        line[0]='\0';
    }


    fclose(fp);
    return g;
}

void main() {
    GRAMMAR g = populateGrammar("grammar.txt");

}
