#include "parser.h"
#define HASHSIZE 300

TOKENINFO globaltk;
TOKENINFO prevtk;
int prev_invalid=0;
int max_jump=0;
int invalid_token=0;
int invalid_prog=0;

Hashtable terminals;
Hashtable non_terminals;

void hashTableInit(int size){
	terminals = (Hashtable)malloc(sizeof(struct hashT) * size);
	for(int i=0; i<size; i++) {
		terminals[i].name = NULL;
		terminals[i].value=-1;
	}

	non_terminals = (Hashtable)malloc(sizeof(struct hashT) * size);
	for(int i=0; i<size; i++) {
		non_terminals[i].name = NULL;
		non_terminals[i].value=-1;
		non_terminals[i].index=-1;
	}
}

int hashCode(char *name, int size){
	int len=strlen(name);
	if(strcmp(name,"eps")==0) {
		return 0;
	}

	long long hashVal=0;
	for(int i=0; i<len; i++) {
		hashVal = hashVal + (int)name[i];
	}
	hashVal = hashVal%size;

	// int i = hashVal, h=1;
	// while(n_block.nt_index == -1){
	//   i = (i + (h*h))%size;
	// }

	return hashVal;
}

int hashEntry(Hashtable t, char *name, int value, int index){

	long long int hashval = hashCode(name, HASHSIZE), h=1;

	while(t[hashval].value != -1) {
		if(t[hashval].value == -2 && strcmp(t[hashval].name,name)==0) {
			t[hashval].value = value;
			return hashval;
		}
		hashval = (hashval + (h*h))%HASHSIZE;
		h++;
	}
	t[hashval].name = (char*)malloc(sizeof(char) * (strlen(name)+1));
	max_jump = (max_jump >= h-1) ? max_jump : h-1;
	t[hashval].value = value;
	t[hashval].index = index;
	strcpy(t[hashval].name, name);

	return hashval;
}

// returns the index of the nonterminal/terminal in grammar depending upon
int get_index(Hashtable t, char* token) {

	int hashval = hashCode(token, HASHSIZE), h=1, j=0;

	while(j <= max_jump+1) {
		if(t[hashval].value==-1) {
			return -1;
		}
		if(strcmp(t[hashval].name,token)==0) {
			return t[hashval].value;
		}
		hashval = (hashval + (h*h))%HASHSIZE;
		h++;
		j++;
	}
	return -1;
}

// returns the index of the nonterminal/terminal in grammar depending upon
int get_hash(Hashtable t, char* token) {

	int hashval = hashCode(token, HASHSIZE), h=1, j=0;

	while(j <= max_jump+1) {
		if(t[hashval].value==-1) {
			return -1;
		}
		if(strcmp(t[hashval].name,token)==0) {
			return hashval;
		}
		hashval = (hashval + (h*h))%HASHSIZE;
		h++;
		j++;
	}
	return -1;
}

GRAMMAR populateGrammar(char* grammar_file) {
	FILE* fp = fopen(grammar_file, "r");
	if(fp==NULL) {
		printf("Could not open Grammar file");
		return NULL;
	}
	char c;
	char line[255];

	GRAMMAR g;
	g = (GRAMMAR)malloc(sizeof(grammar));

	g->nonterminals=(RULE)malloc(sizeof(rule)*20);
	g->follow=(FOLLOWDS)malloc(sizeof(followds)*20);
	g->terminals=(int*)malloc(sizeof(int)*20);
	// Make 1st terminal eps
	(g->terminals)[0] = hashEntry(terminals, "eps", 0, 0);
	int hash_value;
	int nont_count=0,t_count=1,nont_max=20,t_max=20,distinct=0,distinct_max=20;

	while(1) {
		fscanf(fp,"%[^\t\n]s\n",line);
		c=fgetc(fp);
		if(c==EOF) {
			break;
		}
		if(line[0]=='\0') {
			continue;
		}
		// space added just before end of line
		line[strlen(line)+1]='\0';
		line[strlen(line)]=' ';

		// terminal boolean------> -1 -> Dont Know, 0-> Non terminal 1-> Terminal
		int i=0,token_ptr=0, is_term=-1;
		char token[255];

		while(line[i]!='<') {
			i++;
		}
		i++;
		while(line[i]!='>') {
			token[token_ptr]=line[i];
			i++;
			token_ptr++;
		}
		i++;
		token[token_ptr]='\0';
		if(nont_count==nont_max) {
			// Array full
			nont_max+=10;
			g->nonterminals=(RULE)realloc(g->nonterminals,sizeof(rule)*nont_max);
		}
		int nt_value = get_index(non_terminals, token);
		if(nt_value<0) {
			hash_value = hashEntry(non_terminals, token, nont_count, distinct);
			if(distinct==distinct_max) {
				distinct_max+=10;
				g->follow=(FOLLOWDS)realloc(g->follow, sizeof(followds)*nont_max);
			}
			if(nt_value==-1) {
				distinct++;
			}
		}
		(g->nonterminals)[nont_count].key = hash_value;
		(g->nonterminals)[nont_count].start = NULL;

		printf("%d %s\n",hash_value,token);

		while(line[i]!='>') {
			i++;
		}
		i++;
		TK_NODE prev=NULL;
		while(line[i]!='\0') {
			while(line[i]=='\t' || line[i]==' ') {
				i++;
			}
			if(line[i]=='\0') {
				break;
			}
			if(line[i]=='<') {
				token_ptr=0;
				i++;
				while(line[i]!='>') {
					token[token_ptr]=line[i];
					token_ptr++;
					i++;
				}
				i++;
				token[token_ptr]='\0';
				int hashtemp = get_hash(non_terminals, token);
				if(hashtemp==-1) {
					hashtemp = hashEntry(non_terminals, token, -2, distinct);
					if(distinct==distinct_max) {
						distinct_max+=10;
						g->follow=(FOLLOWDS)realloc(g->follow, sizeof(followds)*nont_max);
					}
					distinct++;
				}
				int hashind = non_terminals[hashtemp].index;
				TK_NODE temp = (TK_NODE)malloc(sizeof(tk_node));
				temp->next = NULL;
				temp->type = NT;
				temp->info = hashtemp;
				if((g->nonterminals)[nont_count].start==NULL) {
					(g->nonterminals)[nont_count].start = temp;
					prev = temp;
				} else {
					prev->next = temp;
					prev = temp;
				}

				// follow pointer
				FOLLOWDS follow_temp = (g->follow)+hashind;
				if(follow_temp->f==NULL) {
					follow_temp->f = (FOLLOWIND)malloc(sizeof(followind)*10);
					follow_temp->size=0;
				} else if(follow_temp->size%10==0) {
					follow_temp->f = (FOLLOWIND)realloc(follow_temp->f, (sizeof(followind))*(follow_temp->size+10));
				}

				// tk is the pointer to the current token node (to find position of token directly)
				follow_temp->f[follow_temp->size].tk=temp;
				// index stores the index number of the current rule for cases where follow(A) = follow(B)
				follow_temp->f[follow_temp->size].index=nont_count;
				follow_temp->size++;

				printf("%s\n",token);
			} else {
				token_ptr=0;
				token[token_ptr]=line[i];
				i++;
				token_ptr++;
				while(line[i]!='<'&&line[i]!=' '&&line[i]!='\0'&&line[i]!='\t') {
					token[token_ptr]=line[i];
					i++;
					token_ptr++;
				}
				token[token_ptr]='\0';
				int hashtemp = get_hash(terminals, token);
				if(hashtemp==-1) {
					hashtemp = hashEntry(terminals, token, t_count, t_count);
					if(t_count==t_max) {
						t_max+=10;
						g->terminals=(int*)realloc(g->terminals,sizeof(int)*t_max);
					}
					(g->terminals)[t_count] = hashtemp;
					t_count++;
				}
				TK_NODE temp = (TK_NODE)malloc(sizeof(tk_node));
				temp->next = NULL;
				temp->type = T;
				temp->info = hashtemp;
				if((g->nonterminals)[nont_count].start==NULL) {
					printf("%d\n",hashtemp);

					(g->nonterminals)[nont_count].start = temp;
					prev = temp;
				} else {
					prev->next = temp;
					prev = temp;
				}
				printf("%s\n",token);

			}
		}
		nont_count++;
		line[0]='\0';

	}

	// set non terminal and terminal counts in grammar
	g->nonterminal_count=distinct;
	g->terminal_count=t_count;
	g->rules=nont_count;
	fclose(fp);
	return g;
}


// Add jth row of First matrix to ith row of First matrix
void addFirstRows(FirstFollow f, int t_count, int i, int j) {
	for(int x=0; x<t_count; x++) {
		(f->first)[i][x]+=(f->first)[j][x];
	}
}

// Add jth row of Follow matrix to ith row of Follow matrix
void addFollowRows(FirstFollow f, int t_count, int i, int j) {
	for(int x=0; x<t_count; x++) {
		(f->follow)[i][x]+=(f->follow)[j][x];
	}
}

// Add jth row of First matrix to ith row of Follow matrix
void addFollowFirstRows(FirstFollow f, int t_count, int i, int j) {
	for(int x=0; x<t_count; x++) {
		(f->follow)[i][x]+=(f->first)[j][x];
	}
}

void computeFirst(FirstFollow f, GRAMMAR g, int hashval, int* global, int* local) {

	// printf("%s %d\n",((g->nonterminals)[i]).name, i);
	int i = non_terminals[hashval].value;
	int hashindex = non_terminals[hashval].index;

	local[hashindex]=1; // ith non-terminal visited in recursion tree

	RULE temp = (g->nonterminals)+i;
	int key = temp->key;
	while(temp->key==key) {
		TK_NODE temp_node = temp->start;
		while(temp_node!=NULL) {

			if(temp_node->type==T) {
				// printf("%s\n",g->terminals[(temp_node->info)]);
				// A->b
				(f->first)[hashindex][terminals[temp_node->info].index]=1;
				break;
			} else {
				int ind = non_terminals[temp_node->info].index;
				if(global[ind]==1) {
					// First of ind non terminal already computed
					if((f->first)[ind][0]==0) {
						// A->B.... and first(B)!=eps
						addFirstRows(f, g->terminal_count, hashindex, ind);
						break;
					} else {
						// A->B.... and first(B) contains eps
						(f->first)[ind][0]=0;
						// copy all first set of B except eps and go to next token after B
						addFirstRows(f, g->terminal_count, hashindex, ind);
						(f->first)[ind][0]=1;

						temp_node=temp_node->next;
						continue;
					}
				}
				else if(local[ind]==0) {
					// No interdependencies between ith and ind non terminal
					computeFirst(f, g, temp_node->info, global, local);
					if((f->first)[ind][0]==0) {
						// A->B.... and first(B)!=eps
						addFirstRows(f, g->terminal_count, hashindex, ind);
						break;
					} else {
						// A->B.... and first(B) contains eps
						(f->first)[ind][0]=0;
						// copy all first set of B except eps and go to next token after B
						addFirstRows(f, g->terminal_count, hashindex, ind);
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
			(f->first)[hashindex][0]=1;
		}
		temp=temp+1;
		//    printf("i\n");
	}
	global[hashindex]=1;
}
//
void computeFollow(FirstFollow f, GRAMMAR g, int hashval, int* global, int* local) {

	int i = non_terminals[hashval].value;
	int hashindex = non_terminals[hashval].index;
	local[hashindex]=1;


	for(int x=0; x<(g->follow)[hashindex].size; x++) {
		// ith non terminal xth token position in grammar
		// get the token just next to the occurences of the ith non terminal
		TK_NODE temp_node = (((g->follow)[hashindex].f[x]).tk)->next;
		while(temp_node!=NULL) {
			if(temp_node->type==T) {

				// X->Ab => follow(A)=first(b)=b
				(f->follow)[hashindex][terminals[temp_node->info].index]=1;
				break;

			} else {
				int ind = non_terminals[temp_node->info].index;

				if((f->first)[ind][0]==0) {
					// X->AB if first(B) does not contain eps, follow(A)+=first(B)
					addFollowFirstRows(f, g->terminal_count, hashindex, ind);
					break;
				} else {
					// X->ABC if first(B) contains eps, follow(A)+=(first(B)+first(C))
					(f->first)[ind][0]=0;
					addFollowFirstRows(f, g->terminal_count, hashindex, ind);
					(f->first)[ind][0]=1;

					temp_node=temp_node->next;
					continue;
				}

			}
			temp_node = temp_node->next;
		}
		if(temp_node==NULL) {
			// X->ABCD and first of B,C,D all contain eps => follow(A)=follow(X)
			int parentind = non_terminals[g->nonterminals[((g->follow)[hashindex].f[x]).index].key].index;
			int parenthash = g->nonterminals[((g->follow)[hashindex].f[x]).index].key;
			if(global[parentind]==1) {
				// follow(X) known
				addFollowRows(f,g->terminal_count, hashindex, parentind);
			} else if(parentind==0) {
				// compute follow(X) and then add
				computeFollow(f, g, parenthash, global, local);
				addFollowRows(f,g->terminal_count, hashindex, parentind);
			}
		}
	}
	// If non-terminal not available on right hand side of any rule (program)
	if((g->follow)[hashindex].size==0) {
		(f->follow)[hashindex][0]=1;
	}
	global[hashindex]=1;
}


FirstFollow ComputeFirstAndFollowSets(GRAMMAR g) {

	// Create a 2d matrix for first and follow sets where 0 indicates absent and values>0 represent present
	FirstFollow f = (FirstFollow)malloc(sizeof(firstfollow));
	f->first = (int**)malloc(sizeof(int*)*g->nonterminal_count);
	f->follow = (int**)malloc(sizeof(int*)*g->nonterminal_count);
	int global[g->nonterminal_count]; // Checks if first already computed for ith non terminal (Dynamic Programming)
	memset(global,0,g->nonterminal_count*sizeof(int));
	for(int i=0; i<g->nonterminal_count; i++) {
		(f->first)[i] = (int*)malloc(sizeof(int)*g->terminal_count);
		(f->follow)[i] = (int*)malloc(sizeof(int)*g->terminal_count);
		memset((f->first)[i], 0, g->terminal_count*sizeof(int));
		memset((f->follow)[i], 0, g->terminal_count*sizeof(int));
	}
	int key=-1;
	for(int i=0; i<g->rules; i++) {
		if(key!=(g->nonterminals)[i].key) {
			key=(g->nonterminals)[i].key;
			if(global[non_terminals[key].index]==1) {
				continue;
			}

			int local[g->nonterminal_count]; // Not used correctly yet, will be used for dependencies among two non terminals
			memset(local,0,g->nonterminal_count*sizeof(int));
			computeFirst(f, g, key, global, local);
			// for(int i=0;i<50;i++) {
			//  printf("%d ",global[i]);
			// }
		}

	}
	int globalfollow[g->nonterminal_count];
	memset(globalfollow,0,g->nonterminal_count*sizeof(int));

	key=-1;
	for(int i=0; i<g->rules; i++) {
		if(key!=(g->nonterminals)[i].key) {
			key=(g->nonterminals)[i].key;

			if(globalfollow[non_terminals[key].index]==1) {
				continue;
			}
			int localfollow[g->nonterminal_count];
			memset(localfollow,0,g->nonterminal_count*sizeof(int));
			computeFollow(f, g, key, globalfollow, localfollow);

		}
	}
// printing first and follow
	key=-1;
	for(int x=0; x<g->rules; x++) {
		if(key!=(g->nonterminals)[x].key) {
			key=(g->nonterminals)[x].key;
			printf("\n\n%s",non_terminals[key].name);
			printf("\nFIRST");
			for(int i=0; i<g->terminal_count; i++) {
				if((f->first)[non_terminals[key].index][i]>0) {
					(f->first)[non_terminals[key].index][i]=1;
					printf("-%s",terminals[(g->terminals)[i]].name);
				}
			}
			printf("\nFOLLOW");
			for(int i=0; i<g->terminal_count; i++) {
				if((f->follow)[non_terminals[key].index][i]>0) {
					(f->follow)[non_terminals[key].index][i]=1;
					printf("-%s",terminals[(g->terminals)[i]].name);
				}

			}
		}


	}
	printf("\n");
	return f;
}

PARSETABLE createParseTable(FirstFollow F, GRAMMAR G, PARSETABLE PT) {
	PT = (PARSETABLE)malloc(sizeof(int*)*(G->nonterminal_count));
	// iterate through every rule
	for(int i=0; i<G->nonterminal_count; i++) {
		PT[i] = (int*)malloc(sizeof(int)*(G->terminal_count));
		for(int j=0; j<G->terminal_count; j++) {
			PT[i][j]=-1;
		}
	}
	for(int i=0; i<(G->rules); i++) {
		int key=(G->nonterminals)[i].key;
		int hashInd = non_terminals[key].index;

		TK_NODE tk_temp = (G->nonterminals)[i].start;
		RULE cur_rule = (G->nonterminals)+i;
		while(tk_temp!=NULL) {
			if(tk_temp->type==T) {
				if(tk_temp->info==0) {
					for(int j=0; j<G->terminal_count; j++) {
						if((F->follow)[hashInd][j]==1) {
							// if(PT[i][j]!=NULL) {
							//     printf("qwert");
							// }
							PT[hashInd][j]=i;
						}
					}
					break;
				} else {
					PT[hashInd][terminals[tk_temp->info].index]=i;
					break;
				}
			} else {
				// A->BCD.... rule will be added to first(B), first(C).... till first() does not contain eps
				int ind = non_terminals[tk_temp->info].index;
				for(int j=1; j<G->terminal_count; j++) {
					if((F->first)[ind][j]==1) {
						// if(PT[i][j]!=NULL) {
						//     printf("rtghj");
						// }
						PT[hashInd][j]=i;
					}
				}
				if(F->first[ind][0]==0) {
					break;
				}
				tk_temp=tk_temp->next;
			}
		}
		if(tk_temp==NULL) {
			// if A->BCD.... & first(BCD...) contains eps, add this rule for all follow(A) including $
			for(int j=0; j<G->terminal_count; j++) {
				if((F->follow)[hashInd][j]==1) {
					// if(PT[i][j]!=NULL) {
					//     printf("zdf5rds");
					// }
					PT[hashInd][j]=i;
				}
			}
		}

	}
	return PT;
}

TREE_NODE initialize(int t, int p_ind, int hashval) {
	TREE_NODE s = (TREE_NODE)malloc(sizeof(tree_node));
	s->child=NULL;
	s->next=NULL;
	s->parent_index=p_ind;
	s->type = t;
	s->tk_info.index = hashval;
	s->rule_index=-1;
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

TREE_NODE addRule(int r_index, TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g) {

	// printf("%s \n",tb_nt[g->nonterminals[s->tk_info.index].nt_index].name);
    RULE r = (g->nonterminals)+r_index;
	TK_NODE temp = r->start;
	// TK_NODE x = temp;
	// while(x!=NULL) {
	//   printf("%d %d\n",x->type,x->info);
	//   x=x->next;
	// }
	// printf("\n\n\n");
	TREE_NODE tn = initialize(temp->type, s->tk_info.index, temp->info);
    s->rule_index = r_index;
	s->child = tn;
	tn = buildParseTree(tn, fp, pt, f, g);
	if(globaltk==NULL) {
		return s;
	}
	TREE_NODE prev=tn;
	temp=temp->next;
	while(temp!=NULL) {
		tn = initialize(temp->type, s->tk_info.index, temp->info);
		// printf("%d %d\n",invalid_token, (tn->tk_info).index);
		tn = buildParseTree(tn, fp, pt, f, g);
		prev->next = tn;
		temp=temp->next;
		prev=prev->next;
		if(globaltk==NULL) {
			return s;
		}
	}

	return s;
}

TREE_NODE buildParseTree(TREE_NODE s, FILE* fp, PARSETABLE pt, FirstFollow f, GRAMMAR g) {
	if(globaltk==NULL) {
		return s;
	}
	if(s->type==T) {
		if(globaltk->token==terminals[(s->tk_info).index].value) {
            prev_invalid=0;
            prevtk=NULL;
			invalid_token=0;
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
            prev_invalid=0;
            prevtk=NULL;
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
			       globaltk->lineNo,terminals[g->terminals[globaltk->token]].name,globaltk->lexeme,terminals[(s->tk_info).index].name);

			s->tk_info.tk=NULL;

			return s;
		}
		globaltk = getNextToken(fp);
		return s;
	} else {

		if(pt[non_terminals[s->tk_info.index].index][globaltk->token]!=-1) {
			invalid_token=0;
            prev_invalid=0;
            prevtk=NULL;
			s = addRule(pt[non_terminals[s->tk_info.index].index][globaltk->token], s, fp, pt, f, g);
			return s;
		} else if((f->follow)[non_terminals[s->tk_info.index].index][globaltk->token]==1) {
            // printf("-%s- \n",non_terminals[s->tk_info.index].name);

			if(invalid_token>0) {
				// printf("asdfg\n");
				invalid_token--;
				return s;
			}
			invalid_prog++;

            if(prev_invalid==1) {
    			printf("Line %d: The token %s for lexeme %s  does not match with the expected token %s\n",
    			       prevtk->lineNo,terminals[g->terminals[prevtk->token]].name,prevtk->lexeme,non_terminals[s->tk_info.index].name);

                prev_invalid=0;
                prevtk=NULL;
            } else {

                printf("Line %d: The token %s for lexeme %s does not match with the expected token\n",
                       globaltk->lineNo,terminals[g->terminals[globaltk->token]].name,globaltk->lexeme);
            }

			return s;
		} else {
            prev_invalid=0;
            prevtk=NULL;
			if((f->first)[non_terminals[s->tk_info.index].index][0]==1) {
				RULE r = (g->nonterminals)+non_terminals[s->tk_info.index].index;
                int key = r->key;
                int r_ind = non_terminals[s->tk_info.index].index;
				while(key==r->key) {

					TK_NODE tk = r->start;

					while(tk!=NULL) {
						if(tk->type==T && tk->info==0) {
							tk=NULL;
							break;
						}

						if(tk->type==T) {
							break;
						} else {
							if((f->first)[non_terminals[tk->info].index][0]==1) {

								tk=tk->next;
							} else {

								break;
							}
						}
					}

					if(tk==NULL) {
						return addRule(r_ind, s, fp, pt, f, g);
					}
					r_ind++;
                    r=(g->nonterminals)+r_ind;

				}
			} else {
				invalid_prog++;
				// printf("Line %d: Extra token %s provided\n",globaltk->lineNo,globaltk->lexeme);
                prevtk = globaltk;
                prev_invalid = 1;
				globaltk = getNextToken(fp);
				while(check_token(globaltk, g)==0) {
					invalid_token++;
					globaltk = getNextToken(fp);
					if(globaltk==NULL) {
						return s;
					}
				}
				s = buildParseTree(s, fp, pt, f, g);
				return s;
			}
		}
	}

	return s;
}




TREE_NODE parseInputSourceCode(char *testcaseFile, PARSETABLE pt, FirstFollow f, GRAMMAR g) {

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
	TREE_NODE s = initialize(1, -1, get_hash(non_terminals, "program"));
	globaltk = getNextToken(fp);
	s = buildParseTree(s, fp, pt, f, g);
	while(globaltk!=NULL) {
		invalid_prog++;
		printf("Line %d: Extra token %s provided\n",globaltk->lineNo,globaltk->lexeme);
		globaltk=getNextToken(fp);
	}
	fflush(fp);
	fclose(fp);
	return s;
}

void traversal(FILE*fp, GRAMMAR g, TREE_NODE tree){
	if (tree->child != NULL) {
		traversal(fp, g, tree->child);
	}
	if(tree->type==T) {
		if(tree->tk_info.tk!=NULL) {
			fprintf(fp,"%-30s\t%-3d\t%-20s\t ",tree->tk_info.tk->lexeme, tree->tk_info.tk->lineNo, terminals[g->terminals[tree->tk_info.tk->token]].name);
			if(tree->tk_info.tk->dataType==0) {
				fprintf(fp,"%-5d ",tree->tk_info.tk->val->valI);
			} else if(tree->tk_info.tk->dataType==1) {
				fprintf(fp,"%-5f ",tree->tk_info.tk->val->valF);
			} else {
				fprintf(fp,"---- ");
			}
			fprintf(fp,"%s ",non_terminals[tree->parent_index].name);
			fprintf(fp,"%d ",1-tree->type);
			fprintf(fp,"---- \n");
		}

	} else {
		fprintf(fp,"---- ---- ---- ---- ");
		if(tree->parent_index==-1) {
			fprintf(fp,"ROOT\t");
		} else {
			fprintf(fp,"%-30s\t",non_terminals[tree->parent_index].name);

		}
		fprintf(fp,"0\t");
		fprintf(fp,"%-20s\t\n",non_terminals[tree->tk_info.index].name);
	}

	// printf("ntt %d, parent index %d, \n",tree->type, tree->parent_index);
	if(tree->child != NULL) {
		TREE_NODE tmp = tree->child->next;
		while(tmp != NULL) {
			traversal(fp, g, tmp);
			tmp = tmp->next;
		}
	}
}
