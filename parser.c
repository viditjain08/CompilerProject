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
    g->follow=(FOLLOWDS)malloc(sizeof(followds)*20);

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
        int ruleindex;
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
                            g->follow=(FOLLOWDS)realloc(g->follow, sizeof(followds)*nont_max);
                        }
                        (g->nonterminals)[nont_count].name = (char*)malloc(sizeof(char)*strlen(token)+1);
                        strcpy((g->nonterminals)[nont_count].name,token);
                        (g->nonterminals)[nont_count].r=(RULE)malloc(sizeof(rule));
                        ((g->nonterminals)[nont_count].r)->next=NULL;
                        r_temp = (g->nonterminals)[nont_count].r;
                        ruleindex=nont_count;
                        h_temp = nont_count;
                        nont_count++;
                    } else {
                        ruleindex=h_temp;
                        if((g->nonterminals)[h_temp].r==NULL) {

                            (g->nonterminals)[h_temp].r=(RULE)malloc(sizeof(rule));
                            ((g->nonterminals)[h_temp].r)->next=NULL;
                            r_temp = (g->nonterminals)[h_temp].r;

                        } else {

                            RULE r_temp2=(g->nonterminals)[h_temp].r;
                            while(r_temp2->next!=NULL) {
                                r_temp2 = r_temp2->next;
                            }
                            r_temp = (RULE)malloc(sizeof(rule));
                            r_temp->next = NULL;
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
                                // printf("%d-%d\n",nont_count,nont_max);
                                g->nonterminals=(NONT_BLOCK)realloc(g->nonterminals,sizeof(nont_block)*nont_max);
                                g->follow=(FOLLOWDS)realloc(g->follow, sizeof(followds)*nont_max);

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

                            (r_temp->start->info).non_term_index = h_temp;
                            r_temp->start->type = NT;
                            tk_temp = r_temp->start;
                        } else {
                            tk_temp->next=(TK_NODE)malloc(sizeof(tk_node));
                            tk_temp=tk_temp->next;
                            tk_temp->next=NULL;
                            (tk_temp->info).non_term_index = h_temp;
                            tk_temp->type=NT;
                        }
                        if((g->follow)[h_temp].f==NULL) {
                            (g->follow)[h_temp].f = (FOLLOWIND)malloc(sizeof(followind)*10);
                            (g->follow)[h_temp].size=0;
                        } else if((g->follow)[h_temp].size%10==0) {
                            (g->follow)[h_temp].f = (FOLLOWIND)realloc((g->follow)[h_temp].f, (sizeof(followind))*((g->follow)[h_temp].size+10));
                        }
                        (g->follow)[h_temp].f[(g->follow)[h_temp].size].tk=tk_temp;
                        (g->follow)[h_temp].f[(g->follow)[h_temp].size].index=ruleindex;

                        (g->follow)[h_temp].size++;
                        // if((g->follow)[h_temp].size)

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

void addFirstRows(FirstFollow f, int t_count, int i, int j) {
    for(int x=0;x<t_count;x++) {
        (f->first)[i][x]+=(f->first)[j][x];
    }
}

void addFollowRows(FirstFollow f, int t_count, int i, int j) {
    for(int x=0;x<t_count;x++) {
        (f->follow)[i][x]+=(f->follow)[j][x];
    }
}

void addFollowFirstRows(FirstFollow f, int t_count, int i, int j) {
    for(int x=0;x<t_count;x++) {
        (f->follow)[i][x]+=(f->first)[j][x];
    }
}

void computeFollow(FirstFollow f, GRAMMAR g, int i, int* global, int* local) {
    local[i]=1;
    for(int x=0;x<(g->follow)[i].size;x++) {
        TK_NODE temp_node = (((g->follow)[i].f[x]).tk)->next;
        while(temp_node!=NULL) {
            if(temp_node->type==T) {
                (f->follow)[i][(temp_node->info).term_index]=1;
                // printf("%s\n",g->terminals[(temp_node->info).term_index]);
                break;
            } else {
                int ind = (temp_node->info).non_term_index;
                // printf("%s.\n",g->nonterminals[ind].name);

                if((f->first)[ind][0]==0) {
                    addFollowFirstRows(f, g->t_count, i, ind);
                    break;
                } else {
                    (f->first)[ind][0]=0;
                    addFollowFirstRows(f, g->t_count, i, ind);
                    (f->first)[ind][0]=1;

                    temp_node=temp_node->next;
                    continue;
                }

            }
            temp_node = temp_node->next;
        }
        if(temp_node==NULL) {
            if(global[((g->follow)[i].f[x]).index]==1) {
                addFollowRows(f,g->t_count, i, ((g->follow)[i].f[x]).index);
            } else if(local[((g->follow)[i].f[x]).index]==0){
                computeFollow(f, g, ((g->follow)[i].f[x]).index, global, local);
                addFollowRows(f,g->t_count, i, ((g->follow)[i].f[x]).index);
            }
        }
    }
    if((g->follow)[i].size==0) {
        (f->follow)[i][0]=1;
    }
    global[i]=1;
}




void computeFirst(FirstFollow f, GRAMMAR g, int i, int* global, int* local) {
    local[i]=1;
    // printf("%s %d\n",((g->nonterminals)[i]).name, i);
    RULE temp = ((g->nonterminals)[i]).r;
    while(temp!=NULL) {
        TK_NODE temp_node = temp->start;
        while(temp_node!=NULL) {

            if(temp_node->type==T) {
            	// printf("%s\n",g->terminals[(temp_node->info).term_index]);
                (f->first)[i][(temp_node->info).term_index]=1;
                break;
            } else {
                int ind = (temp_node->info).non_term_index;
                if(global[ind]==1) {
                    if((f->first)[ind][0]==0) {
                        addFirstRows(f, g->t_count, i, ind);
                        break;
                    } else {
                        (f->first)[ind][0]=0;
                        addFirstRows(f, g->t_count, i, ind);
                        (f->first)[ind][0]=1;

                        temp_node=temp_node->next;
                        continue;
                    }
                }
                else if(local[ind]==0) {

                    computeFirst(f, g, ind, global, local);
                    if((f->first)[ind][0]==0) {
                    //	printf("qwertyu\n");
                        addFirstRows(f, g->t_count, i, ind);
                        break;
                    } else {
                    //	printf("asdfgh\n");
                        (f->first)[ind][0]=0;
                        addFirstRows(f, g->t_count, i, ind);
                        (f->first)[ind][0]=1;

                        temp_node=temp_node->next;
                        continue;
                    }
                }
                else {
                    continue;
                }
          //      printf("qazwwsx\n");
            }
            temp_node = temp_node->next;
        }
    //    printf("v\n");
        if(temp_node==NULL) {
            (f->first)[i][0]=1;
        }
        temp=temp->next;
    //    printf("i\n");
    }
    global[i]=1;
}

FirstFollow ComputeFirstAndFollowSets(GRAMMAR g) {
    FirstFollow f = (FirstFollow)malloc(sizeof(firstfollow));
    f->first = (int**)malloc(sizeof(int*)*g->non_t_count);
    f->follow = (int**)malloc(sizeof(int*)*g->non_t_count);
    int global[g->non_t_count];
    memset(global,0,g->non_t_count*sizeof(int));
    for(int i=0;i<g->non_t_count;i++) {
        (f->first)[i] = (int*)malloc(sizeof(int)*g->t_count);
        (f->follow)[i] = (int*)malloc(sizeof(int)*g->t_count);
        memset((f->first)[i], 0, g->t_count*sizeof(int));
        memset((f->follow)[i], 0, g->t_count*sizeof(int));
    }

            // printf("%d.%s\n",i,(g->nonterminals)[i].name);
            // TK_NODE temp_node = temp->start;
            // while(temp_node!=NULL) {
            //     if(temp_node->type==T) {
            //         (f->first)[i][(temp_node->info).term_index]=1;
            //         break;
            //     }
            //     temp_node = temp_node->next;
            // }
    for(int i=0;i<g->non_t_count;i++) {
        if(global[i]==1) {
            continue;
        }
        int local[g->non_t_count];
        memset(local,0,g->non_t_count*sizeof(int));
        computeFirst(f, g, i, global, local);
        // for(int i=0;i<50;i++) {
        // 	printf("%d ",global[i]);
        // }

    }
    int globalfollow[g->non_t_count];
    memset(globalfollow,0,g->non_t_count*sizeof(int));

    for(int i=0;i<g->non_t_count;i++) {
        if(globalfollow[i]==1) {
            continue;
        }
        int localfollow[g->non_t_count];
        memset(localfollow,0,g->non_t_count*sizeof(int));
        computeFollow(f, g, i, globalfollow, localfollow);
    }

    for(int x=0;x<50;x++) {
        printf("\n\n%s",(g->nonterminals)[x].name);
        printf("\nFIRST");
        for(int i=0;i<54;i++) {
            if((f->first)[x][i]>0) {
                printf("-%s",(g->terminals)[i]);
            }
        }
        printf("\nFOLLOW");
        for(int i=0;i<54;i++) {
            if((f->follow)[x][i]>=1) {
                printf("-%s",(g->terminals)[i]);
            }
        }
    }
    printf("\n");
}


void main() {
    GRAMMAR g = populateGrammar("grammar.txt");
    // printf("%d\n",((g->follow)[2].size));
    // for(int i=0;i<54;i++) {
    //     printf("%s %d\n",(g->terminals)[i], i);
    // }

    FirstFollow f = ComputeFirstAndFollowSets(g);

    // for(int i)
}
