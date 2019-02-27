#include "parser.h"

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
    g->gr = (RULE*)malloc(sizeof(RULE)*grammar_len);
    for(int i=0;i<grammar_len;i++) {
        (g->gr)[i]=NULL;
    }
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
        for(int i=0;line[i]!='\0';i++) {
            if(line[i]=='\t') {
                continue;
            }
            if(left==1) {
                if(line[i]=='<') {
                    token_ptr=0;
                } else if(line[i]=='>') {
                    token[token_ptr]='\0';
                    left=0;
                } else {
                    token[token_ptr]=line[i];
                    token_ptr++;
                }
            }
        }
        printf("%s\n",token);
        printf("%s\n",line);

        line[0]='\0';
    }


    fclose(fp);
    return g;
}

void main() {
    GRAMMAR g = populateGrammar("grammar.txt");

}
