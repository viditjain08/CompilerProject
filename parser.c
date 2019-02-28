#include "parser.h"

int check_nont(char* token, GRAMMAR g, int nont_count) {
    if(nont_count==0) {
        return -1;
    }
    for(int i=0;i<nont_count;i++) {
        if(strcmp((g->nonterminals)[i].name,token)==0) {
            return i;
        }
    }
    return -1;
}

int check_t(char* token, GRAMMAR g, int t_count) {
    if(t_count==0) {
        return -1;
    }
    for(int i=0;i<t_count;i++) {
        if(strcmp((g->terminals)[i],token)==0) {
            return i;
        }
    }
    return -1;
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
    g->nonterminals=(NONT_BLOCK)malloc(sizeof(nont_block)*20);
    g->terminals=(char**)malloc(sizeof(char*)*20);
    (g->terminals)[0]=(char*)malloc(sizeof(char)*4);
    strcpy((g->terminals)[0], "eps");
    int nont_count=0,t_count=1,nont_max=20,t_max=20;
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
        int h_temp;
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
                    h_temp = check_nont(token, g, nont_count);
                    if(h_temp==-1) {
                        if(nont_count==nont_max) {
                            nont_max+=10;
                            g->nonterminals=(NONT_BLOCK)realloc(g->nonterminals,sizeof(nont_block)*nont_max);
                        }
                        (g->nonterminals)[nont_count].name = (char*)malloc(sizeof(char)*strlen(token)+1);
                        strcpy((g->nonterminals)[nont_count].name,token);
                        (g->nonterminals)[nont_count].r=(RULE)malloc(sizeof(rule));
                        r_temp = (g->nonterminals)[nont_count].r;

                        nont_count++;
                    } else {
                        if((g->nonterminals)[h_temp].r==NULL) {

                            (g->nonterminals)[h_temp].r=(RULE)malloc(sizeof(rule));
                            r_temp = (g->nonterminals)[h_temp].r;

                        } else {

                            RULE r_temp2=(g->nonterminals)[h_temp].r;
                            while(r_temp2->next!=NULL) {
                                r_temp2 = r_temp2->next;
                            }
                            r_temp = (RULE)malloc(sizeof(rule));
                            r_temp2->next=r_temp;
                        }

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
                        int t_ind = check_t(token, g,t_count);
                        if(t_ind==-1) {
                            if(t_count==t_max) {
                                t_max+=10;
                                g->terminals=(char**)realloc(g->terminals,sizeof(char*)*t_max);
                            }
                            (g->terminals)[t_count] = (char*)malloc(sizeof(char)*strlen(token)+1);
                            strcpy((g->terminals)[t_count],token);
                            t_ind=t_count;
                            t_count++;
                        }
                        if(tk_temp==NULL) {

                            r_temp->start=(TK_NODE)malloc(sizeof(tk_node));
                            r_temp->start->next=NULL;
                            (r_temp->start->info).term_index = t_ind;
                            r_temp->start->type = T;

                            tk_temp = r_temp->start;
                        } else {
                            tk_temp->next=(TK_NODE)malloc(sizeof(tk_node));
                            tk_temp=tk_temp->next;
                            tk_temp->next=NULL;
                            (tk_temp->info).term_index = t_ind;
                            tk_temp->type = T;

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

                        h_temp = check_nont(token, g, nont_count);
                        if(h_temp==-1) {

                            if(nont_count==nont_max) {
                                nont_max+=10;
                                printf("%d-%d\n",nont_count,nont_max);
                                g->nonterminals=(NONT_BLOCK)realloc(g->nonterminals,sizeof(nont_block)*nont_max);
                            }
                            (g->nonterminals)[nont_count].name = (char*)malloc(sizeof(char)*strlen(token)+1);
                            strcpy((g->nonterminals)[nont_count].name,token);
                            // (g->nonterminals)[nont_count].r=(RULE)malloc(sizeof(rule));
                            h_temp=nont_count;
                            nont_count++;
                        }

                        if(tk_temp==NULL) {
                            r_temp->start=(TK_NODE)malloc(sizeof(tk_node));
                            r_temp->start->next=NULL;

                            (r_temp->start->info).non_term=&(g->nonterminals)[nont_count];
                            r_temp->start->type = NT;
                            tk_temp = r_temp->start;
                        } else {
                            tk_temp->next=(TK_NODE)malloc(sizeof(tk_node));
                            tk_temp=tk_temp->next;
                            tk_temp->next=NULL;
                            (tk_temp->info).non_term=&(g->nonterminals)[nont_count];
                            tk_temp->type=NT;
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

    g->non_t_count=nont_count;
    g->t_count=t_count;
    fclose(fp);
    return g;
}

FirstFollow ComputeFirstAndFollowSets(GRAMMAR g) {
    FirstFollow f = (FirstFollow)malloc(sizeof(firstfollow));
    f->first = (int**)malloc(sizeof(int*)*g->non_t_count);
    f->follow = (int**)malloc(sizeof(int*)*g->non_t_count);
    for(int i=0;i<g->non_t_count;i++) {
        (f->first)[i] = (int*)malloc(sizeof(int)*g->t_count);
        (f->follow)[i] = (int*)malloc(sizeof(int)*g->t_count);
        memset((f->first)[i], 0, g->t_count*sizeof(int));
        memset((f->follow)[i], 0, g->t_count*sizeof(int));
        RULE temp = ((g->nonterminals)+i)->r;
        while(temp!=NULL) {
            printf("%d.%s\n",i,(g->nonterminals)[i].name);
            TK_NODE temp_node = temp->start;
            while(temp_node!=NULL) {
                if(temp_node->type==T) {
                    (f->first)[i][(temp_node->info).term_index]=1;
                    break;
                }
                temp_node = temp_node->next;
            }
            temp=temp->next;
        }
    }
}

void main() {
    GRAMMAR g = populateGrammar("grammar.txt");

    FirstFollow f = ComputeFirstAndFollowSets(g);
}
