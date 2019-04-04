#include "parser.h"
#define HASHSIZE 300

TOKENINFO globaltk;
TOKENINFO nexttk;
int max_jump=0;
int invalid_token=0;
int invalid_prog=0;
int hashCode(char *name, int size){
  int len=strlen(name);
  if(strcmp(name,"eps")==0){
    return 0;
  }

  long long hashVal=0;
  for(int i=0;i<len;i++){
    hashVal = hashVal + (int)name[i];
  }
  hashVal = hashVal%size;

  // int i = hashVal, h=1;
  // while(n_block.nt_index == -1){
  //   i = (i + (h*h))%size;
  // }

  return hashVal;
}

Hashtable hashTableInit(int size){
  Hashtable tb = (Hashtable)malloc(sizeof(struct hashT) * size);
  for(int i=0;i<size;i++){
    tb[i].name = NULL;
    tb[i].flag=0;
  }
  return tb;
}

int hashEntry(Hashtable t, char *name, int size, int count){
  if(strcmp(name,"eps") == 0){
    t[0].name = (char*)malloc(sizeof(char) * (strlen(name)+1));
    t[0].flag=1;
    t[0].index = count;
    strcpy(t[0].name, name);
    return 0;
  }
  int hashval = hashCode(name, size), h=1;
  while(t[hashval].flag == 1){
    hashval = (hashval + (h*h))%size;
    h++;
  }
  t[hashval].name = (char*)malloc(sizeof(char) * (strlen(name)+1));
  max_jump = (max_jump >= h-1) ? max_jump : h-1;
  t[hashval].flag=1;
  t[hashval].index = count;
  strcpy(t[hashval].name, name);
  return hashval;
}

// returns the index of the nonterminal in grammar
int check_nont(char* token, GRAMMAR g, int nont_count, Hashtable tb_nt) {
    if(nont_count==0) {
        return -1;
    }
    int hashval = hashCode(token, HASHSIZE), h=1, count=0;

    while(count <= max_jump+1) {
        if(tb_nt[hashval].flag==0){
          return -1;
        }
        if(strcmp(tb_nt[hashval].name,token)==0) {
            return tb_nt[hashval].index;
        }
        hashval = (hashval + (h*h))%HASHSIZE;
        h++;
        count++;
    }
    return -1;
}

// returns the index of the terminal in grammar
int check_t(char* token, int t_count, Hashtable tb_t) {
    if(t_count==0) {
        return -1;
    }
    int hashval = hashCode(token, HASHSIZE), h=1, count=0;

    while(count <= max_jump+1) {
        if(tb_t[hashval].flag==0){
          return -1;
        }
        if(strcmp(tb_t[hashval].name,token)==0) {
            return tb_t[hashval].index;
        }
        hashval = (hashval + (h*h))%HASHSIZE;
        h++;
        count++;
    }
    return -1;
}

GRAMMAR populateGrammar(char* grammar_file, Hashtable tb_nt, Hashtable tb_t) {
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
    g->terminals=(int*)malloc(sizeof(int)*20);

    // Make 1st terminal eps
    (g->terminals)[0]=hashEntry(tb_t, "eps", HASHSIZE, 0);

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
        // Left = 1 if token read is on the left side of the rule
        int left=1;

        char token[255];
        int token_ptr=0;
        int nt_ind;

////////These 3 identify the current position of the previous token completely///////////
        int ruleindex;
        RULE cur_rule;
        TK_NODE tk_temp=NULL;
////////////////////////////////////////////////////////////////////////////////////////
        // terminal boolean------> -1 -> Dont Know, 0-> Non terminal 1-> Terminal
        int is_term=-1;
        // space added just before end of line
        line[strlen(line)+1]='\0';
        line[strlen(line)]=' ';
        // Current Index of non terminal of the rule
        for(int i=0;line[i]!='\0';i++) {
        //    printf("%c  ",line[i]);
            // skip spaces on left side
            if(left==1 && (line[i]=='\t' || line[i]==' ')) {
                continue;
            }
            if(left==1) {
                if(line[i]=='<') {
                    token_ptr=0;
                } else if(line[i]=='>') {
                    // add token to grammar
                    token[token_ptr]='\0';
                    left=0;
                    nt_ind = check_nont(token, g, nont_count, tb_nt);
                    if(nt_ind==-1) {
                        // Non Terminal not present in grammar
                        if(nont_count==nont_max) {
                            // Array full
                            nont_max+=10;
                            g->nonterminals=(NONT_BLOCK)realloc(g->nonterminals,sizeof(nont_block)*nont_max);
                            g->follow=(FOLLOWDS)realloc(g->follow, sizeof(followds)*nont_max);
                        }
                        NONT_BLOCK cur_non_term = (g->nonterminals)+nont_count;

                        // Add token name
                        //cur_non_term->name = (char*)malloc(sizeof(char)*strlen(token)+1);
                        //strcpy(cur_non_term->name,token);
                        cur_non_term->nt_index = hashEntry(tb_nt, token, HASHSIZE, nont_count);

                        // Allocate blank rule
                        cur_non_term->r=(RULE)malloc(sizeof(rule));
                        cur_non_term->r->next=NULL;

                        cur_rule = cur_non_term->r;

                        ruleindex=nont_count;
                        nont_count++;
                    } else {
                        // existing left side
                        ruleindex=nt_ind;

                        NONT_BLOCK cur_non_term = (g->nonterminals)+nt_ind;

                        if(cur_non_term->r==NULL) {

                            cur_non_term->r=(RULE)malloc(sizeof(rule));
                            cur_non_term->r->next=NULL;
                            cur_rule = cur_non_term->r;

                        } else {

                            // Go to last rule to add a new rule at the end
                            RULE last_rule=cur_non_term->r;
                            while(last_rule->next!=NULL) {
                                last_rule = last_rule->next;
                            }
                            cur_rule = (RULE)malloc(sizeof(rule));
                            cur_rule->next = NULL;
                            last_rule->next=cur_rule;
                        }

                    }
                    token_ptr=0;

                } else {
                    // add alphabet to token buffer
                    token[token_ptr]=line[i];
                    token_ptr++;

                }
            } else {
                // Right side of the rule
                // skip ===>
                if(line[i]=='=') {
                    continue;
                }
                if(token_ptr==0 && line[i]=='>') {
                    continue;
                }
                if(is_term==1) {
                    // Token is known to be a terminal
                    if(line[i]=='<' || line[i]=='\t' || line[i]=='|' || line[i]==' ') {
                        // store token
                        token[token_ptr]='\0';

                        // check if token exists in grammar's terminals
                        int t_ind = check_t(token, t_count, tb_t);
                        if(t_ind==-1) {
                            // token not found
                            if(t_count==t_max) {
                                t_max+=10;
                                g->terminals=(int*)realloc(g->terminals,sizeof(int)*t_max);
                            }
                            // (g->terminals)[t_count] = (char*)malloc(sizeof(char)*strlen(token)+1);
                            // strcpy((g->terminals)[t_count],token);
                            (g->terminals)[t_count] = hashEntry(tb_t, token, HASHSIZE, t_count);
                            t_ind=t_count;
                            t_count++;
                        }
                        // t_ind is the index of the current terminal read

                        TK_NODE temp;
                        temp=(TK_NODE)malloc(sizeof(tk_node));
                        temp->next=NULL;
                        temp->info = t_ind;
                        temp->type = T;

                        if(tk_temp==NULL) {
                            // The token is the first token of the rule
                            cur_rule->start = temp;
                            tk_temp = cur_rule->start;

                        } else {

                            tk_temp->next=temp;
                            tk_temp=tk_temp->next;

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
                            // Received | in buffer => New rule
                            tk_temp=NULL; // reset tk_temp

                            // Allocate New rule and update cur_rule
                            cur_rule->next=(RULE)malloc(sizeof(rule));
                            cur_rule=cur_rule->next;
                            cur_rule->next=NULL;
                            is_term=-1;
                        }
                    } else {
                        token[token_ptr]=line[i];
                        token_ptr++;
                    }
                } else if(is_term==0) {
                    // if non-terminal
                    if(line[i]=='>') {
                        // save non-terminal
                        token[token_ptr]='\0';

                        nt_ind = check_nont(token, g, nont_count, tb_nt);
                        if(nt_ind==-1) {

                            if(nont_count==nont_max) {
                                nont_max+=10;
                                // printf("%d-%d\n",nont_count,nont_max);
                                g->nonterminals=(NONT_BLOCK)realloc(g->nonterminals,sizeof(nont_block)*nont_max);
                                g->follow=(FOLLOWDS)realloc(g->follow, sizeof(followds)*nont_max);

                            }
                            // Insert token name to newly added non_terminal
                            // (g->nonterminals)[nont_count].name = (char*)malloc(sizeof(char)*strlen(token)+1);
                            // strcpy((g->nonterminals)[nont_count].name,token);
                            (g->nonterminals)[nont_count].nt_index = hashEntry(tb_nt, token, HASHSIZE, nont_count);
                            (g->nonterminals)[nont_count].r=NULL;
                            nt_ind=nont_count;
                            nont_count++;
                        }
                        // nt_ind = current index of non-terminal referred
                        TK_NODE temp = (TK_NODE)malloc(sizeof(tk_node));
                        temp->next = NULL;
                        temp->info = nt_ind;
                        temp->type = NT;
                        if(tk_temp==NULL) {
                            // First token in current rule
                            cur_rule->start = temp;
                            tk_temp = cur_rule->start;

                        } else {

                            tk_temp->next=temp;
                            tk_temp=tk_temp->next;

                        }
                        // follow pointer
                        FOLLOWDS follow_temp = (g->follow)+nt_ind;
                        if(follow_temp->f==NULL) {
                            follow_temp->f = (FOLLOWIND)malloc(sizeof(followind)*10);
                            follow_temp->size=0;
                        } else if(follow_temp->size%10==0) {
                            follow_temp->f = (FOLLOWIND)realloc(follow_temp->f, (sizeof(followind))*(follow_temp->size+10));
                        }

                        // tk is the pointer to the current token node (to find position of token directly)
                        follow_temp->f[follow_temp->size].tk=tk_temp;
                        // index stores the index number of the current rule for cases where follow(A) = follow(B)
                        follow_temp->f[follow_temp->size].index=ruleindex;
                        follow_temp->size++;
                        // if((g->follow)[nt_ind].size)

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
                        // Create new rule and update cur_rule
                        tk_temp=NULL;
                        cur_rule->next=(RULE)malloc(sizeof(rule));
                        cur_rule=cur_rule->next;
                        cur_rule->next=NULL;
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
        // printf("%s\n",line);

        line[0]='\0';
    }

    // set non terminal and terminal counts in grammar
    g->non_t_count=nont_count;
    g->t_count=t_count;
    fclose(fp);
    return g;
}


// Add jth row of First matrix to ith row of First matrix
void addFirstRows(FirstFollow f, int t_count, int i, int j) {
    for(int x=0;x<t_count;x++) {
        (f->first)[i][x]+=(f->first)[j][x];
    }
}

// Add jth row of Follow matrix to ith row of Follow matrix
void addFollowRows(FirstFollow f, int t_count, int i, int j) {
    for(int x=0;x<t_count;x++) {
        (f->follow)[i][x]+=(f->follow)[j][x];
    }
}

// Add jth row of First matrix to ith row of Follow matrix
void addFollowFirstRows(FirstFollow f, int t_count, int i, int j) {
    for(int x=0;x<t_count;x++) {
        (f->follow)[i][x]+=(f->first)[j][x];
    }
}

void computeFirst(FirstFollow f, GRAMMAR g, int i, int* global, int* local) {
    local[i]=1; // ith non-terminal visited in recursion tree

    // printf("%s %d\n",((g->nonterminals)[i]).name, i);
    RULE temp = ((g->nonterminals)[i]).r;
    while(temp!=NULL) {
        TK_NODE temp_node = temp->start;
        while(temp_node!=NULL) {

            if(temp_node->type==T) {
            	// printf("%s\n",g->terminals[(temp_node->info)]);
                // A->b
                (f->first)[i][temp_node->info]=1;
                break;
            } else {
                int ind = temp_node->info;
                if(global[ind]==1) {
                    // First of ind non terminal already computed
                    if((f->first)[ind][0]==0) {
                        // A->B.... and first(B)!=eps
                        addFirstRows(f, g->t_count, i, ind);
                        break;
                    } else {
                        // A->B.... and first(B) contains eps
                        (f->first)[ind][0]=0;
                        // copy all first set of B except eps and go to next token after B
                        addFirstRows(f, g->t_count, i, ind);
                        (f->first)[ind][0]=1;

                        temp_node=temp_node->next;
                        continue;
                    }
                }
                else if(local[ind]==0) {
                    // No interdependencies between ith and ind non terminal
                    computeFirst(f, g, ind, global, local);
                    if((f->first)[ind][0]==0) {
                        // A->B.... and first(B)!=eps
                        addFirstRows(f, g->t_count, i, ind);
                        break;
                    } else {
                        // A->B.... and first(B) contains eps
                        (f->first)[ind][0]=0;
                        // copy all first set of B except eps and go to next token after B
                        addFirstRows(f, g->t_count, i, ind);
                        (f->first)[ind][0]=1;

                        temp_node=temp_node->next;
                        continue;
                    }
                }
                else {
                    continue;
                }
            }
            temp_node = temp_node->next;
        }
    //    printf("v\n");
        if(temp_node==NULL) {
            // A->BCD and first(B),first(C),first(D) contains eps
            (f->first)[i][0]=1;
        }
        temp=temp->next;
    //    printf("i\n");
    }
    global[i]=1;
}

void computeFollow(FirstFollow f, GRAMMAR g, int i, int* global, int* local) {
    local[i]=1;
    for(int x=0;x<(g->follow)[i].size;x++) {
        // ith non terminal xth token position in grammar
        // get the token just next to the occurences of the ith non terminal
        TK_NODE temp_node = (((g->follow)[i].f[x]).tk)->next;
        while(temp_node!=NULL) {
            if(temp_node->type==T) {

                // X->Ab => follow(A)=first(b)=b
                (f->follow)[i][temp_node->info]=1;
                break;

            } else {
                int ind = temp_node->info;

                if((f->first)[ind][0]==0) {
                    // X->AB if first(B) does not contain eps, follow(A)+=first(B)
                    addFollowFirstRows(f, g->t_count, i, ind);
                    break;
                } else {
                    // X->ABC if first(B) contains eps, follow(A)+=(first(B)+first(C))
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
            // X->ABCD and first of B,C,D all contain eps => follow(A)=follow(X)
            if(global[((g->follow)[i].f[x]).index]==1) {
                // follow(X) known
                addFollowRows(f,g->t_count, i, ((g->follow)[i].f[x]).index);
            } else if(local[((g->follow)[i].f[x]).index]==0){
                // compute follow(X) and then add
                computeFollow(f, g, ((g->follow)[i].f[x]).index, global, local);
                addFollowRows(f,g->t_count, i, ((g->follow)[i].f[x]).index);
            }
        }
    }
    // If non-terminal not available on right hand side of any rule (program)
    if((g->follow)[i].size==0) {
        (f->follow)[i][0]=1;
    }
    global[i]=1;
}


FirstFollow ComputeFirstAndFollowSets(GRAMMAR g, Hashtable tb_nt, Hashtable tb_t) {

    // Create a 2d matrix for first and follow sets where 0 indicates absent and values>0 represent present
    FirstFollow f = (FirstFollow)malloc(sizeof(firstfollow));
    f->first = (int**)malloc(sizeof(int*)*g->non_t_count);
    f->follow = (int**)malloc(sizeof(int*)*g->non_t_count);
    int global[g->non_t_count]; // Checks if first already computed for ith non terminal (Dynamic Programming)
    memset(global,0,g->non_t_count*sizeof(int));
    for(int i=0;i<g->non_t_count;i++) {
        (f->first)[i] = (int*)malloc(sizeof(int)*g->t_count);
        (f->follow)[i] = (int*)malloc(sizeof(int)*g->t_count);
        memset((f->first)[i], 0, g->t_count*sizeof(int));
        memset((f->follow)[i], 0, g->t_count*sizeof(int));
    }

    for(int i=0;i<g->non_t_count;i++) {
        if(global[i]==1) {
            continue;
        }
        int local[g->non_t_count]; // Not used correctly yet, will be used for dependencies among two non terminals
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
// printing first and follow
    for(int x=0;x<g->non_t_count;x++) {
        // printf("\n\n%s",tb_nt[(g->nonterminals)[x].nt_index].name);
        // printf("\nFIRST");
        for(int i=0;i<g->t_count;i++) {
            if((f->first)[x][i]>0) {
                (f->first)[x][i]=1;
                // printf("-%s",tb_t[(g->terminals)[i]].name);
            }
        }
        // printf("\nFOLLOW");
        for(int i=0;i<g->t_count;i++) {
            if((f->follow)[x][i]>0) {
                (f->follow)[x][i]=1;
                // printf("-%s",tb_t[(g->terminals)[i]].name);
            }
        }
    }
    printf("\n");
    return f;
}

PARSETABLE createParseTable(FirstFollow F, GRAMMAR G, PARSETABLE PT, Hashtable tb_nt, Hashtable tb_t) {
    PT = (PARSETABLE)malloc(sizeof(RULE*)*(G->non_t_count));
    // iterate through every rule
    for(int i=0;i<(G->non_t_count);i++) {
        PT[i] = (RULE*)malloc(sizeof(RULE)*(G->t_count));
        for(int j=0;j<G->t_count;j++) {
            PT[i][j]=NULL;
        }
        if((G->nonterminals)[i].r->next==NULL) {
            // if single rule for ith non terminal
            RULE cur_rule = (G->nonterminals)[i].r;
            // Add this rule's pointer for every terminal in first(X) except eps
            for(int j=1;j<G->t_count;j++) {
                if((F->first)[i][j]==1) {
                    // if(PT[i][j]!=NULL) {
                    //     printf("sdfghj");
                    // }
                    PT[i][j]=cur_rule;

                }
            }
            if((F->first)[i][0]==1) {
                // if eps in first(X), since single rule, rule's pointer for every terminal in follow(X)
                RULE cur_rule = (G->nonterminals)[i].r;
                for(int j=0;j<G->t_count;j++) {
                    if((F->follow)[i][j]==1) {
                        // if(PT[i][j]!=NULL) {
                        //     printf("qwert");
                        // }
                        PT[i][j]=cur_rule;
                    }
                }
            }
        } else {
            // multiple rules
            RULE cur_rule = (G->nonterminals)[i].r;
            while(cur_rule!=NULL) {
                //iterate through each rule
                TK_NODE t_temp = cur_rule->start;

                if(t_temp->info==0 && t_temp->type==T) {
                    // printf("%d %d %s\n",i,t_temp->info,tb_nt[G->nonterminals[i].nt_index].name);
                    for(int j=0;j<G->t_count;j++) {
                        if((F->follow)[i][j]==1) {
                            // if(PT[i][j]!=NULL) {
                            //     printf("qwert");
                            // }
                            PT[i][j]=cur_rule;
                        }
                    }
                    cur_rule=cur_rule->next;
                    continue;
                }
                while(t_temp!=NULL) {
                    if(t_temp->type==T) {
                        // A->b.... rule will be added for [A,b]
                        if(PT[i][t_temp->info]!=NULL) {
                            printf("%d %d %s\n",i,t_temp->info,tb_nt[G->nonterminals[i].nt_index].name);
                        }
                        PT[i][t_temp->info]=cur_rule;
                        break;
                    } else {
                        // A->BCD.... rule will be added to first(B), first(C).... till first() does not contain eps
                        int ind = t_temp->info;
                        for(int j=1;j<G->t_count;j++) {
                            if((F->first)[ind][j]==1) {
                                // if(PT[i][j]!=NULL) {
                                //     printf("rtghj");
                                // }
                                PT[i][j]=cur_rule;
                            }
                        }
                        if(F->first[ind][0]==0) {
                            break;
                        }
                    }
                    t_temp=t_temp->next;
                }
                if(t_temp==NULL) {
                    // if A->BCD.... & first(BCD...) contains eps, add this rule for all follow(A) including $
                    for(int j=0;j<G->t_count;j++) {
                        if((F->follow)[i][j]==1) {
                            // if(PT[i][j]!=NULL) {
                            //     printf("zdf5rds");
                            // }
                            PT[i][j]=cur_rule;
                        }
                    }
                }
                cur_rule=cur_rule->next;
            }
        }
    }
    return PT;
}

TREE_NODE initialize(int t, int p_ind, int ind) {
    TREE_NODE s = (TREE_NODE)malloc(sizeof(tree_node));
    s->child=NULL;
    s->next=NULL;
    s->parent_index=p_ind;
    s->type = t;
    s->tk_info.index = ind;
    return s;
}

int check_token(TOKENINFO tk, GRAMMAR g) {
        if(tk->token==TK_ERROR) {
            invalid_prog++;
            printf("Line %d: Unknown pattern %s\n", tk->lineNo, tk->lexeme);
            return 0;
        } else if(tk->token==TK_ERROR2) {
            invalid_prog++;
            printf("Line %d: Identifier is longer than the prescribed length\n", tk->lineNo);
            return 0;
        } else if(tk->token==TK_ERROR3) {
            invalid_prog++;
            printf("Line %d: Unknown symbol %s\n", tk->lineNo, tk->lexeme);
            return 0;
        }

    return 1;
}

TREE_NODE addRule(RULE r, TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g, Hashtable tb_nt, Hashtable tb_t) {

    // printf("%s \n",tb_nt[g->nonterminals[s->tk_info.index].nt_index].name);

    TK_NODE temp = r->start;
    // TK_NODE x = temp;
    // while(x!=NULL) {
    //   printf("%d %d\n",x->type,x->info);
    //   x=x->next;
    // }
    // printf("\n\n\n");
    TREE_NODE tn = initialize(temp->type, s->tk_info.index, temp->info);
    s->child = tn;
    tn = buildParseTree(tn, fp, pt, f, g, tb_nt, tb_t);
    if(globaltk==NULL) {
        return s;
    }
    TREE_NODE prev=tn;
    temp=temp->next;
    while(temp!=NULL) {
        tn = initialize(temp->type, s->tk_info.index, temp->info);
        // printf("%d %d\n",invalid_token, (tn->tk_info).index);
        tn = buildParseTree(tn, fp, pt, f, g, tb_nt, tb_t);
        prev->next = tn;
        temp=temp->next;
        prev=prev->next;
        if(globaltk==NULL) {
            return s;
        }
    }

    return s;
}

TREE_NODE buildParseTree(TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g, Hashtable tb_nt, Hashtable tb_t) {
    if(globaltk==NULL) {
        return s;
    }
    if(s->type==T) {
        if(globaltk->token==(s->tk_info).index) {
            s->tk_info.tk = globaltk;
            globaltk = getNextToken(fp);
            if(globaltk==NULL) {
                return s;
            }
            while(check_token(globaltk, g)==0) {
                invalid_token++;
                globaltk = getNextToken(fp);
                if(globaltk==NULL) {
                    return s;
                }
                // printf("%s\n",globaltk->lexeme);
            }
            return s;
        } else if((s->tk_info).index==0) {
            TOKENINFO tk = (TOKENINFO)malloc(sizeof(tokenInfo));
            tk->dataType=-1;
            tk->lexeme = (char*)malloc(sizeof(char)*4);
            strcpy(tk->lexeme,"eps");
            tk->lineNo = -1;
            tk->token = 0;
            s->tk_info.tk = tk;
            return s;
        } else {

            if(invalid_token>0) {
                // printf("qwertyui\n");
                s->tk_info.tk=NULL;
                invalid_token--;
                return s;
            }


            invalid_prog++;
            printf("Line %d: The token %s for lexeme %s  does not match with the expected token %s \n",
                    globaltk->lineNo,tb_t[g->terminals[globaltk->token]].name,globaltk->lexeme,tb_t[g->terminals[(s->tk_info).index]].name);

            s->tk_info.tk=NULL;

            return s;
        }
        globaltk = getNextToken(fp);
        return s;
    } else {
        if(pt[s->tk_info.index][globaltk->token]!=NULL) {
            s = addRule(pt[s->tk_info.index][globaltk->token], s, fp, pt, f, g, tb_nt, tb_t);
            return s;
        } else if((f->follow)[s->tk_info.index][globaltk->token]==1) {
            if(invalid_token>0) {
                // printf("asdfg\n");
                invalid_token--;
                return s;
            }
            invalid_prog++;
            printf("Line %d: The token %s for lexeme %s does not match with the expected token\n",
            globaltk->lineNo,tb_t[g->terminals[globaltk->token]].name,globaltk->lexeme);
            return s;
        } else {
            if((f->first)[s->tk_info.index][0]==1) {
                // printf("-%s- \n",tb_nt[g->nonterminals[s->tk_info.index].nt_index].name);
                RULE r = (g->nonterminals)[s->tk_info.index].r;
                while(r!=NULL) {

                    TK_NODE tk = r->start;

                    while(tk!=NULL) {
                        if(tk->type==T && tk->info==0) {
                            tk=NULL;
                            break;
                        }

                        if(tk->type==T) {
                            break;
                        } else {
                            if((f->first)[tk->info][0]==1) {

                                tk=tk->next;
                            } else {

                                break;
                            }
                        }
                    }

                    if(tk==NULL) {
                        return addRule(r, s, fp, pt, f, g, tb_nt, tb_t);
                    }
                    r=r->next;
                }
            } else {
                invalid_prog++;
                printf("Line %d: Extra token %s provided\n",globaltk->lineNo,globaltk->lexeme);
                globaltk = getNextToken(fp);
                while(check_token(globaltk, g)==0) {
                    invalid_token++;
                    globaltk = getNextToken(fp);
                    if(globaltk==NULL) {
                        return s;
                    }
                }
                s = buildParseTree(s, fp, pt, f, g, tb_nt, tb_t);
                return s;
            }
        }
    }

    return s;
}




TREE_NODE parseInputSourceCode(char *testcaseFile, PARSETABLE pt, FirstFollow f, GRAMMAR g, Hashtable tb_nt, Hashtable tb_t) {

    // hashInit(15);

    FILE *fp = fopen(testcaseFile,"r");
    fileEnd = 0;
    free(bufferPre);
    free(bufferCurr);
    bufferPre = (char*)malloc(BUFFER_SIZE*sizeof(char));
    bufferCurr = (char*)malloc(BUFFER_SIZE*sizeof(char));

    memset(bufferPre,0,BUFFER_SIZE*sizeof(char));
    memset(bufferCurr,0,BUFFER_SIZE*sizeof(char));

    getStream(fp);
    currChar = 0;
    lineNo = 1;
    TREE_NODE s = initialize(1, -1, 0);
    globaltk = getNextToken(fp);
    s = buildParseTree(s, fp, pt, f, g, tb_nt, tb_t);
    while(globaltk!=NULL) {
        invalid_prog++;
        printf("Line %d: Extra token %s provided\n",globaltk->lineNo,globaltk->lexeme);
        globaltk=getNextToken(fp);
    }
    fflush(fp);
    fclose(fp);
    return s;
}

void traversal(FILE*fp, GRAMMAR g, TREE_NODE tree, Hashtable tb_nt, Hashtable tb_t){
    if (tree->child != NULL) {
        traversal(fp, g, tree->child, tb_nt, tb_t);
    }
    if(tree->type==T) {
        if(tree->tk_info.tk!=NULL) {
            fprintf(fp,"%-30s\t%-3d\t%-20s\t ",tree->tk_info.tk->lexeme, tree->tk_info.tk->lineNo, tb_t[g->terminals[tree->tk_info.tk->token]].name);
            if(tree->tk_info.tk->dataType==0) {
                fprintf(fp,"%-5d ",tree->tk_info.tk->val->valI);
            } else if(tree->tk_info.tk->dataType==1) {
                fprintf(fp,"%-5f ",tree->tk_info.tk->val->valF);
            } else {
                fprintf(fp,"---- ");
            }
            fprintf(fp,"%s ",tb_nt[g->nonterminals[tree->parent_index].nt_index].name);
            fprintf(fp,"%d ",1-tree->type);
            fprintf(fp,"---- \n");
        }

    } else {
        fprintf(fp,"---- ---- ---- ---- ");
        if(tree->parent_index==-1) {
            fprintf(fp,"ROOT\t");
        } else {
            fprintf(fp,"%-30s\t",tb_nt[g->nonterminals[tree->parent_index].nt_index].name);

        }
        fprintf(fp,"0\t");
        fprintf(fp,"%-20s\t\n",tb_nt[g->nonterminals[tree->tk_info.index].nt_index].name);
    }

    // printf("ntt %d, parent index %d, \n",tree->type, tree->parent_index);
    if(tree->child != NULL){
        TREE_NODE tmp = tree->child->next;
        while(tmp != NULL){
            traversal(fp, g, tmp, tb_nt, tb_t);
            tmp = tmp->next;
        }
    }
}
