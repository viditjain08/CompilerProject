#include "lexerDef.h"

#define BUFFER_SIZE 8
#define MAX_LEXEME_SIZE 50

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

int main() {

    char file[] = "source_code";
    char outFile[] = "output.txt";
    // test(file);

    removeComments(file,outFile);

    return 0;
}
