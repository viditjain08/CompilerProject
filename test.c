#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "lexerDef.h"

#define BUFFER_SIZE 8
#define MAX_LEXEME_SIZE 50

void getStream(FILE *fp);
void test(const char *source_Code);
char* getNextToken(FILE *fp);

// declaring Buffers
char* bufferCurr;
char* bufferPre;

int currChar;
int fileEnd;

void getStream(FILE *fp){
    char *tmp = bufferPre;
    bufferPre = bufferCurr;
    bufferCurr = tmp;

    memset(bufferCurr,0,BUFFER_SIZE*sizeof(char));

    int bytesRead = fread(bufferCurr,sizeof(char),BUFFER_SIZE,fp);
    bufferCurr[BUFFER_SIZE] = '\0';
    if(bytesRead != BUFFER_SIZE){
        printf("Either File completed or error\n");
        fileEnd = 1;
    }else{
        printf("%d Bytes read successfully\n",bytesRead );
    }

    //return fp;
}

void removeComments(const char* testCase, const char* outfile){
    FILE *fin = fopen(testCase,"r");
    FILE *fout = fopen(outfile,"w+");

    free(bufferPre);
    free(bufferCurr);
    bufferPre = (char*)malloc(BUFFER_SIZE*sizeof(char));
    bufferCurr = (char*)malloc(BUFFER_SIZE*sizeof(char));

    memset(bufferPre,0,BUFFER_SIZE*sizeof(char));
    memset(bufferCurr,0,BUFFER_SIZE*sizeof(char));

    getStream(fin);
    currChar = 0;
    int preChar = 0;
    while(1){
        if (fileEnd == 0) {
            while (bufferCurr[currChar] != '%' && !(bufferCurr[currChar] == '\0' && currChar < BUFFER_SIZE)) {
                if (currChar==BUFFER_SIZE) {
                    fwrite(bufferCurr+preChar,sizeof(char),BUFFER_SIZE-preChar,fout);
                    getStream(fin);
                    currChar = -1;
                    preChar = 0;
                }
                currChar++;
            }
            fwrite(bufferCurr+preChar,sizeof(char),currChar-preChar,fout);
            while (bufferCurr[currChar] != '\n' && !(bufferCurr[currChar] == '\0' && currChar < BUFFER_SIZE)) {
                if (currChar==BUFFER_SIZE) {
                    fwrite(bufferCurr+currChar,sizeof(char),BUFFER_SIZE-currChar,fout);
                    getStream(fin);
                    currChar = -1;
                }
                currChar++;
            }
            preChar = currChar;
        }else{
            break;
        }
    }

    fclose(fin);
    fclose(fout);

}

void test(const char *source_Code){
    FILE *fp = fopen(source_Code,"r");
    fileEnd = 0;
    free(bufferPre);
    free(bufferCurr);
    bufferPre = (char*)malloc(BUFFER_SIZE*sizeof(char));
    bufferCurr = (char*)malloc(BUFFER_SIZE*sizeof(char));

    memset(bufferPre,0,BUFFER_SIZE*sizeof(char));
    memset(bufferCurr,0,BUFFER_SIZE*sizeof(char));

    getStream(fp);
    currChar = 0;

    printf("Checkpoint 1\n" );

    char *lex;
    int i = 0;
    while ((fileEnd != 1 && i<20)) {
        printf("%s\n", getNextToken(fp));
        i++;
    }

}

char* getNextToken(FILE *fp){

    char *lexeme = (char*)malloc(MAX_LEXEME_SIZE*sizeof(char));
    memset(lexeme,0,MAX_LEXEME_SIZE*sizeof(char));
    int j = 0;
    FILE *f;
    //f = *fp;
    while(bufferCurr[currChar] != ' '){

        if(currChar < BUFFER_SIZE && bufferCurr[currChar]!='\0'){
            lexeme[j++] = bufferCurr[currChar++];
        }else if(currChar == BUFFER_SIZE){
            getStream(fp);
            currChar = 0;
        }else if (bufferCurr[currChar]=='\0'){
            printf("File Finished\n");
            fileEnd = 1;
            break;
        }
    }
    lexeme[j] = '\0';
    currChar++;

    return lexeme;
}

int main() {

    char file[] = "source_code";
    char outFile[] = "output.txt";
    // test(file);

    removeComments(file,outFile);

    return 0;
}
