// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#include "lexerDef.h"
void test(const char *source_Code);
char *tokenMap[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RECORDID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE"
};
#define BUFFER_SIZE 32
#define MAX_LEXEME_SIZE 50

// void getStream(FILE *fp);
// void test(const char *source_Code);
// int getNextToken(FILE *fp);

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
        // printf("Either File completed or error\n");
        fileEnd = 1;
    }else{
        // printf("%d Bytes read successfully\n",bytesRead );
    }

    //return fp;
}


void printTokens(const char *source_Code){
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

    int tk = 0;
    int i = 0;
    while (tk!=-1) {
        tk = getNextToken(fp);
        if(tk >= 0){
            printf("%s\n",tokenMap[tk]);
        }
        i++;
    }

}

tokenType getNextToken(FILE *fp){
    char *lexeme = (char*)malloc(MAX_LEXEME_SIZE*sizeof(char));
    memset(lexeme,0,MAX_LEXEME_SIZE*sizeof(char));
    int j = 0;      // forward pointer for lexeme

    int tkGenerated = 0;
    int state = 0;
    int finalStateReached = 0;

    char c;

    while(tkGenerated!=1){
        if(currChar < BUFFER_SIZE && bufferCurr[currChar] == '\0'){
            return -1;
        }else if(currChar == BUFFER_SIZE){
            getStream(fp);
            currChar=0;
        }else{
            c = bufferCurr[currChar];

            switch (state) {
                case 0:
                    if (c == '<') {
                        /* code */
                        currChar++;
                        state = 1;
                    }else if (c == '>') {
                        /* code */
                        currChar++;
                        state = 4;
                    }else if (c == '=') {
                        /* code */
                        currChar++;
                        state = 5;
                    }else if (c == '!') {
                        /* code */
                        currChar++;
                        state = 6;
                    }else if (c == '~') {
                        /* code */
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return TK_NOT;

                    }else if (c == '@') {
                        /* code */
                        currChar++;
                        state = 8;
                    }else if (c == '&') {
                        /* code */
                        currChar++;
                        state = 10;
                    }else if (c == '[' || c == ']' || c == ',' || c == ';' || c == ':' || c == '.' || c == '(' || c == ')' || c == '-' || c == '+' || c == '*' || c == '/') {
                        /* code */
                        switch (c) {
                            case '[':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_SQL;
                            break;

                            case ']':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_SQR;
                            break;

                            case ',':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_COMMA;
                            break;

                            case ';':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_SEM;
                            break;

                            case ':':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_COLON;
                            break;

                            case '.':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_DOT;
                            break;

                            case '(':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_OP;
                            break;

                            case ')':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_CL;
                            break;

                            case '+':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_PLUS;
                            break;

                            case '-':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_MINUS;
                            break;

                            case '*':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_MUL;
                            break;

                            case '/':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return TK_DIV;
                            break;

                        }
                    }else if (c == '%') {
                        /* code */
                        state = 13;
                        currChar++;
                    }else if (c >=48 && c<=57) {    // for 0-9
                        /* code */
                        state = 14;
                        lexeme[j++] = c;
                        currChar++;
                    }else if (c == '#') {
                        /* code */
                        state = 17;
                        lexeme[j++] = c;
                        currChar++;
                    }else if (c == 32 || c==10 || c==9) {     // for delimiter
                        /* code */
                        currChar++;
                        state = 0;
                    }else if (c >=98 && c<=100) {       // for b-d
                        /* code */
                        state = 20;
                        lexeme[j++] = c;
                        currChar++;
                    }else if (c == 'a' || (c >=101 && c<=122)) {        // for a,[e-z]
                        /* code */
                        state = 23;
                        lexeme[j++] = c;
                        currChar++;
                    }else if (c == '_') {
                        /* code */
                        state = 24;
                        lexeme[j++] = c;
                        currChar++;
                    }else {
                        /* code */
                    }
                break;

                case 1:
                    if (c == '=') {
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        currChar++;
                        return TK_LE;
                    }else if(c == '-'){
                        state = 2;
                        currChar++;
                    }else{
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        return TK_LT;
                    }
                break;

                case 2:
                    if (c=='-') {
                        state = 3;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 3:
                    if (c=='-') {
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        currChar++;
                        return TK_ASSIGNOP;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 4:
                    if (c == '=') {
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return TK_GE;
                    }else{
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return TK_GT;
                    }
                break;

                case 5:
                    if (c == '=') {
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return TK_EQ;
                    }else{
                        printf("Error while tokenizing\n" );
                    }
                break;

                case 6:
                    if (c == '=') {
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return TK_NE;
                    }else{
                        printf("Error while tokenizing\n" );
                    }
                break;

                case 7:

                break;

                case 8:
                    if (c == '@') {
                        state = 9;
                        currChar++;
                    }else{
                        printf("Error while tokeninzing\n");
                    }
                break;

                case 9:
                    if (c == '@') {
                        state = 0;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                    }else{
                        printf("Error while tokeninzing\n");
                    }
                break;

                case 10:
                    if (c == '&') {
                        state = 11;
                        currChar++;
                    }else{
                        printf("Error while tokeninzing\n");
                    }
                break;

                case 11:
                    if (c == '&') {
                        state = 0;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                    }else{
                        printf("Error while tokeninzing\n");
                    }
                break;

                case 12:

                break;

                case 13:
                    if(c == '\n'){
                        state=0;
                        currChar++;
                    }else{
                        state = 13;
                        currChar++;
                    }
                break;

                case 14:
                    if(c >=48 && c<=57){
                        state = 14;
                        lexeme[j++] = c;
                        currChar++;
                    }else if(c == '.'){
                        state = 15;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return TK_NUM;
                    }
                break;

                case 15:
                    if (c >=48 && c<=57) {
                        state = 16;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokeninzing\n" );
                    }
                break;

                case 16:
                    if (c >=48 && c<=57) {
                        state = 0;
                        lexeme[j++] = c;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return TK_RNUM;
                    }else{
                        printf("Error while tokeninzing\n" );
                    }
                break;

                case 17:
                    if (c >=97 && c<=122) {
                        state = 18;
                        lexeme[j++] = c;
                        currChar++;
                    }
                break;

                case 18:
                    if (c >=97 && c<=122) {
                        state = 18;
                        lexeme[j++] = c;
                        currChar++;
                    }else if(c == 32 || c==10 || c==9 || c==':'){
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return TK_RECORDID;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 19:

                break;

                case 20:
                    if ((c >=50 && c<=55)) {
                        state = 21;
                        lexeme[j++] = c;
                        currChar++;
                    }else if((c >=97 && c<=122)){
                        state = 23;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 21:
                    if ((c >=50 && c<=55)) {
                        state = 22;
                        lexeme[j++] = c;
                        currChar++;
                    }else if((c >=98 && c<=100)){
                        state = 21;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        state =0;
                        lexeme[j++] = c;
                        tkGenerated = 1;
                        finalStateReached = 1;
                        return TK_ID;
                    }
                break;

                case 22:
                    if ((c >=50 && c<=55)) {
                        state = 22;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        state =0;
                        lexeme[j++] = c;
                        tkGenerated = 1;
                        finalStateReached = 1;
                        return TK_ID;
                    }
                break;

                case 23:
                    if ((c >=97 && c<=122)) {
                        state = 23;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        finalStateReached = 1;
                        tkGenerated = 1;
                        /* we have to see if lexeme is a reserved keyword or not*/
                        state = 0;
                        return TK_FIELDID;
                    }
                break;

                case 24:
                    if (c == 'm') {
                        state = 25;
                        lexeme[j++] = c;
                        currChar++;
                    }else if((c >=97 && c<=108)||(c >=110 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 25:
                    if (c == 'a') {
                        state = 26;
                        lexeme[j++] = c;
                        currChar++;
                    }else if((c >=98 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 26:
                    if (c == 'i') {
                        state = 27;
                        lexeme[j++] = c;
                        currChar++;
                    }else if((c >=97 && c<=104)||(c >=106 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 27:
                    if (c == 'n') {
                        state = 28;
                        lexeme[j++] = c;
                        currChar++;
                    }else if((c >=97 && c<=109)||(c >=111 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 28:
                    if ((c >=97 && c<=122) ||(c >=65 && c<=90)) {
                        state = 29;
                        lexeme[j++] = c;
                        currChar++;
                    }else if(c == 32 || c==10 || c==9){
                        state =0;
                        finalStateReached =1;
                        tkGenerated = 1;
                        return TK_MAIN;
                    }else {
                        printf("Error while tokenizing\n");
                    }
                break;

                case 29:
                    if ((c >=97 && c<=122) ||(c >=65 && c<=90)) {
                        state = 29;
                        currChar++;
                        lexeme[j++] = c;
                    }else if((c >=48 && c<=57)){
                        state = 30;
                        currChar++;
                        lexeme[j++] = c;
                    }else if(c == 32 || c==10 || c==9){
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return TK_FUNID;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

                case 30:
                    if ((c >=48 && c<=57)) {
                        state = 30;
                        currChar++;
                        lexeme[j++] = c;
                    }else if(c == 32 || c==10 || c==9){
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return TK_FUNID;
                    }else{
                        printf("Error while tokenizing\n");
                    }
                break;

            }
        }
    }


}

int main() {

    char file[] = "source_code";
    char outFile[] = "output.txt";

    //removeComments(file,outFile);

    printTokens(file);

    return 0;
}
