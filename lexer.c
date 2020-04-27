/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include "lexerDef.h"

char* bufferCurr;
char* bufferPre;

int currChar;
int fileEnd=0;
int lineNo;


void getStream(FILE *fp){
    char *tmp = bufferPre;
    bufferPre = bufferCurr;
    bufferCurr = tmp;

    memset(bufferCurr,0,BUFFER_SIZE*sizeof(char));

    int bytesRead = fread(bufferCurr,sizeof(char),BUFFER_SIZE,fp);
    bufferCurr[BUFFER_SIZE] = '\0';
    if(bytesRead != BUFFER_SIZE){
        //printf("Either File completed or error, %s\n",bufferCurr);
        fileEnd = 1;
    }else{
        //printf("%d Bytes read successfully, %s\n",bytesRead, bufferCurr );
    }

    //return fp;
}

void removeComments(const char* testCase){
    FILE *fin = fopen(testCase,"r");
    FILE *fout = stdout;
    fileEnd = 0;
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
    fileEnd=0;
    fclose(fin);
    // fclose(fout);

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
    lineNo = 1;

    tokenInfo *tk = getNextToken(fp);
    while (tk!=NULL) {
        if(tk!=NULL){
            printf("%-20s\t%-30s\t%-5d\n",tokenMap[tk->token],tk->lexeme,tk->lineNo);
        }
        tk = getNextToken(fp);

    }
    fflush(fp);
    fclose(fp);

}

char *createLexeme(int j,int flag){
    int curr;

    if (flag ==1){
        curr = currChar-1;
    }else{
        curr = currChar;
    }
    if(j>curr){
        char *lexeme = (char*)malloc((j+1)*sizeof(char));
        memset(lexeme,0,(j+1)*sizeof(char));
        lexeme[j] = '\0';

        int k = j - curr-1;
        int i;
        for(i=0;i<=curr;i++){
            lexeme[k+i] = bufferCurr[i];
        }
        for(i=0;i<k;i++){
            lexeme[k-i-1] = bufferPre[BUFFER_SIZE-i-1];
        }
        return lexeme;
    }else{
        char *lexeme = (char*)malloc((j+1)*sizeof(char));
        lexeme[j] = '\0';
        int i;
        for(i=0;i<j;i++){
            lexeme[i] = bufferCurr[curr-j+i+1];
        }
        return lexeme;
    }
}

tokenInfo* getNextToken(FILE *fp){
    char *lexeme =NULL;
    // char lexeme[MAX_LEXEME_SIZE];
    //memset(lexeme,0,MAX_LEXEME_SIZE*sizeof(char));
    int j = 0;      // forward pointer for lexeme

    int tkGenerated = 0;
    int state = 0;
    int finalStateReached = 0;

    char c;

    while(tkGenerated!=1){
        if(currChar < BUFFER_SIZE && bufferCurr[currChar] == '\0'){
            return NULL;
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
                        j++;
                    }else if (c == '>') {
                        /* code */
                        j++;
                        currChar++;
                        state = 4;
                    }else if (c == '=') {
                        /* code */
                        j++;
                        currChar++;
                        state = 5;
                    }else if (c == '!') {
                        /* code */
                        j++;
                        currChar++;
                        state = 6;
                    }else if (c == '~') {
                        /* code */
                        j++;
                        lexeme = createLexeme(j,0);
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_NOT,lexeme,lineNo);

                    }else if (c == '@') {
                        /* code */
                        j++;
                        currChar++;
                        state = 8;
                    }else if (c == '&') {
                        /* code */
                        j++;
                        currChar++;
                        state = 10;
                    }else if (c == '[' || c == ']' || c == ',' || c == ';' || c == ':' || c == '.' || c == '(' || c == ')' || c == '-' || c == '+' || c == '*' || c == '/') {
                        /* code */
                        switch (c) {
                            case '[':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_SQL,lexeme,lineNo);
                            break;

                            case ']':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_SQR,lexeme,lineNo);
                            break;

                            case ',':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_COMMA,lexeme,lineNo);
                            break;

                            case ';':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_SEM,lexeme,lineNo);
                            break;

                            case ':':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_COLON,lexeme,lineNo);
                            break;

                            case '.':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_DOT,lexeme,lineNo);
                            break;

                            case '(':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_OP,lexeme,lineNo);
                            break;

                            case ')':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_CL,lexeme,lineNo);
                            break;

                            case '+':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_PLUS,lexeme,lineNo);
                            break;

                            case '-':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_MINUS,lexeme,lineNo);
                            break;

                            case '*':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_MUL,lexeme,lineNo);
                            break;

                            case '/':
                                j++;
                                lexeme = createLexeme(j,0);
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_DIV,lexeme,lineNo);
                            break;

                        }
                    }else if (c == '%') {
                        /* code */
                        state = 13;
                        currChar++;
                    }else if (c >=48 && c<=57) {    // for 0-9
                        /* code */
                        j++;
                        state = 14;
                        currChar++;
                    }else if (c == '#') {
                        /* code */
                        state = 17;
                        j++;
                        currChar++;
                    }else if (c == 32 || c==10 || c==9) {     // for delimiter
                        /* code */
                        currChar++;
                        state = 0;
                        if (c==10) {
                            lineNo++;
                        }
                    }else if (c >=98 && c<=100) {       // for b-d
                        /* code */
                        state = 20;
                        j++;
                        currChar++;
                    }else if (c == 'a' || (c >=101 && c<=122)) {        // for a,[e-z]
                        /* code */
                        state = 23;
                        j++;
                        currChar++;
                    }else if (c == '_') {
                        /* code */
                        state = 24;
                        j++;
                        currChar++;
                    }else {
                        /* code */
                        // printf("Error while tokenizing %c at lineNo %d\n",c,lineNo);
                        currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        char *lexeme = (char*)malloc(2*sizeof(char));
                        lexeme[0] = c;
                        lexeme[1] ='\0';
                        return getTKinfo(TK_ERROR3,lexeme,lineNo);
                    }
                break;

                case 1:
                    if (c == '=') {
                        j++;
                        lexeme = createLexeme(j,0);
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        currChar++;
                        return getTKinfo(TK_LE,lexeme,lineNo);;
                    }else if(c == '-'){
                        j++;
                        state = 2;
                        currChar++;
                    }else{
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        lexeme = createLexeme(j,1);
                        return getTKinfo(TK_LT,"<",lineNo);
                    }
                break;

                case 2:
                    if (c=='-') {
                        j++;
                        state = 3;
                        currChar++;
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);
                    }
                break;

                case 3:
                    if (c=='-') {
                        j++;
                        lexeme = createLexeme(j,0);
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        currChar++;
                        return getTKinfo(TK_ASSIGNOP,lexeme,lineNo);;
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);
                    }
                break;

                case 4:
                    if (c == '=') {
                        j++;
                        lexeme = createLexeme(j,0);
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_GE,lexeme,lineNo);;
                    }else{

                        lexeme = createLexeme(j,1);
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_GT,lexeme,lineNo);;
                    }
                break;

                case 5:
                    if (c == '=') {
                        j++;
                        lexeme = createLexeme(j,0);
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_EQ,"==",lineNo);;
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 6:
                    if (c == '=') {
                        j++;
                        lexeme = createLexeme(j,0);
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_NE,lexeme,lineNo);;
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 7:

                break;

                case 8:
                    if (c == '@') {
                        j++;
                        state = 9;
                        currChar++;
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 9:
                    if (c == '@') {
                        j++;
                        lexeme = createLexeme(j,0);
                        state = 0;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_OR,lexeme,lineNo);
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 10:
                    if (c == '&') {
                        j++;
                        state = 11;
                        currChar++;
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 11:
                    if (c == '&') {
                        j++;
                        lexeme = createLexeme(j,0);
                        state = 0;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_AND,lexeme,lineNo);
                    }else{
                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 12:

                break;

                case 13:
                    if(c == '\n'){
                        state=0;
                        currChar++;
                        lineNo++;
                    }else{
                        state = 13;
                        currChar++;
                    }
                break;

                case 14:
                    if(c >=48 && c<=57){
                        j++;
                        state = 14;
                        currChar++;
                    }else if(c == '.'){
                        state = 15;
                        j++;
                        currChar++;
                    }else{
                        lexeme = createLexeme(j,1);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_NUM,lexeme,lineNo);
                    }
                break;

                case 15:
                    if (c >=48 && c<=57) {
                        j++;
                        state = 16;
                        currChar++;
                    } else if(c =='.') {
                    	j--;
                    	currChar--;
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        lexeme = createLexeme(j,1);
                        return getTKinfo(TK_NUM,lexeme,lineNo);
                    } else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 16:
                    if (c >=48 && c<=57) {
                    	j++;
                    	state = 32;
                    	currChar++;
                        //j++;
                        //lexeme = createLexeme(j,0);
                        //state = 0;
                        //currChar++;
                        //finalStateReached = 1;
                        //tkGenerated = 1;
                        //return getTKinfo(TK_RNUM,lexeme,lineNo);
                    }else{

                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;
				
				case 32:
					if(c==101 || c==69) {
						j++;
						state = 33;
						currChar++;
					} else {
				    	tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        lexeme = createLexeme(j,1);
                        return getTKinfo(TK_RNUM,lexeme,lineNo);
					}
					break;
					
				case 33:
					if(c=='+'||c=='-') {
						j++;
						state = 34;
						currChar++;
					} else if (c >=48 && c<=57) {
						j++;
						state = 34;
						currChar++;
					} else {
                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);

                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);
					}
					break;
				case 34:
					if(c>=48 && c<=57) {
						j++;
						state = 34;
						currChar++;
					} else {
				    	tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        lexeme = createLexeme(j,1);
                        return getTKinfo(TK_RNUM,lexeme,lineNo);					
					}
					break;
                case 17:
                    if (c >=97 && c<=122) {
                        state = 18;
                        j++;
                        currChar++;
                    }else{
                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 18:
                    if (c >=97 && c<=122) {
                        state = 18;
                        j++;
                        currChar++;
                    }else if(c == 32 || c==10 || c==9 || c==':'){
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme = createLexeme(j,1);
                        return getTKinfo(TK_RECORDID,lexeme,lineNo);
                    }else{
                        lexeme = createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_RECORDID,lexeme,lineNo);;
                    }
                break;

                case 19:

                break;

                case 20:
                    if ((c >=50 && c<=55)) {
                        state = 21;
                        j++;
                        currChar++;
                    }else if((c >=97 && c<=122)){
                        state = 23;
                        j++;
                        currChar++;
                    }else{
                        lexeme = createLexeme(j,1);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_FIELDID,lexeme,lineNo);;
                    }
                break;

                case 21:
                    if ((c >=50 && c<=55)) {
                        state = 22;
                        j++;
                        currChar++;
                    }else if((c >=98 && c<=100)){
                        state = 21;
                        j++;
                        currChar++;
                    }else{
                        state =0;
                        // lexeme[j++] = c;
                        tkGenerated = 1;
                        finalStateReached = 1;
                        lexeme = createLexeme(j,1);
                        if (j>20) {
                            // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                            return getTKinfo(TK_ERROR2,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_ID,lexeme,lineNo);
                        }
                    }
                break;

                case 22:
                    if ((c >=50 && c<=55)) {
                        state = 22;
                        j++;
                        currChar++;
                    }else{
                        state =0;
                        // lexeme[j++] = c;
                        tkGenerated = 1;
                        finalStateReached = 1;
                        lexeme = createLexeme(j,1);
                        if (j>20) {
                            // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                            return getTKinfo(TK_ERROR2,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_ID,lexeme,lineNo);
                        }
                    }
                break;

                case 23:
                    if ((c >=97 && c<=122)) {
                        state = 23;
                        j++;
                        currChar++;
                    }else{
                        finalStateReached = 1;
                        tkGenerated = 1;
                        /* we have to see if lexeme is a reserved keyword or not*/
                        state = 0;
                        lexeme = createLexeme(j,1);
                        int tk = lookup(lexeme);
                        if (tk>=0) {
                            return getTKinfo(tk,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_FIELDID,lexeme,lineNo);
                        }
                    }
                break;

                case 24:
                    if (c == 'm') {
                        state = 25;
                        j++;
                        currChar++;
                    }else if((c >=97 && c<=108)||(c >=110 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        j++;
                        currChar++;
                    }else{
                        lexeme =createLexeme(j,1);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_FUNID,lexeme,lineNo);;
                    }
                break;

                case 25:
                    if (c == 'a') {
                        state = 26;
                        j++;
                        currChar++;
                    }else if((c >=98 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        j++;
                        currChar++;
                    }else{
                        lexeme =createLexeme(j,1);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_FUNID,lexeme,lineNo);;
                    }
                break;

                case 26:
                    if (c == 'i') {
                        state = 27;
                        j++;
                        currChar++;
                    }else if((c >=97 && c<=104)||(c >=106 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        j++;
                        currChar++;
                    }else{
                        lexeme =createLexeme(j,1);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_FUNID,lexeme,lineNo);;
                    }
                break;

                case 27:
                    if (c == 'n') {
                        state = 28;
                        j++;
                        currChar++;
                    }else if((c >=97 && c<=109)||(c >=111 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        j++;
                        currChar++;
                    }else{
                        lexeme =createLexeme(j,1);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_FUNID,lexeme,lineNo);;
                    }
                break;

                case 28:
                    if ((c >=97 && c<=122) ||(c >=65 && c<=90)) {
                        state = 29;
                        j++;
                        currChar++;
                    }else if(c == 32 || c==10 || c==9){
                        state =0;
                        finalStateReached =1;
                        tkGenerated = 1;
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme = createLexeme(j,1);
                        return getTKinfo(TK_MAIN,lexeme,lineNo);
                    }else {
                        lexeme =createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 29:
                    if ((c >=97 && c<=122) ||(c >=65 && c<=90)) {
                        state = 29;
                        currChar++;
                        j++;
                    }else if((c >=48 && c<=57)){
                        state = 30;
                        currChar++;
                        j++;
                    }else if(c == 32 || c==10 || c==9){
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme = createLexeme(j,1);
                        if (j>30) {
                            // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                            return getTKinfo(TK_ERROR,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_FUNID,lexeme,lineNo);
                        }
                    }else{
                        lexeme =createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

                case 30:
                    if ((c >=48 && c<=57)) {
                        state = 30;
                        currChar++;
                        j++;
                    }else if(c == 32 || c==10 || c==9){
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme = createLexeme(j,1);
                        if (j>30) {
                            return getTKinfo(TK_ERROR2,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_FUNID,lexeme,lineNo);
                        }
                    }else{
                        lexeme =createLexeme(j,1);
                        // printf("Error while tokenizing %s at lineNo %d\n",lexeme,lineNo);
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,lexeme,lineNo);;
                    }
                break;

            }
        }
    }


}

int a_to_i(const char *s)
{
	int sum = 0;
	int sign = 0;
	int length = 0;
	while(s[length] != '\0')
	{
		length++;
	}
	int i = 0;
	if(s[0] == '-')
	{
		sign = -1;
		i++;

	}


	for(i;i<length;i++)
	{
		sum = 10*sum + s[i] - '0';
	}

	if(sign == -1)
		return -sum;
	return sum;
}

float a_to_f(const char *s)
{
	float sum1 = 0.0;
	float sum2 = 0.0;
	int sign = 0;
	int length = 0;
	int dec = 0;
	int dec_index = -1;
	while(s[length] != '\0')
	{
		length++;
	}
	int i =0;
	if(s[0] == '-')
	{
		sign = -1;
		i++;
	}
	for(i;i<length;i++)
	{
		if(s[i] == '.')
		{
			dec = 1;
			dec_index = i;
			continue;
		}

		else
		{
			if(dec == 0)
			{
				sum1 = sum1*10 + s[i] - '0';
			}
			else
			{
				sum2 = sum2/10.0 + s[length-(i-dec_index)] - '0';

			}
		}

	}
	sum2 = sum2/10;
	if(sign == -1)
	{
		return -(sum1 + sum2);
	}
	else
		return (sum1 + sum2);


}
tokenInfo* getTKinfo(int token,char* lexeme,int lineNo){
	tokenInfo* temp = (tokenInfo*)malloc(sizeof(tokenInfo));
	temp->token = token;
  //temp->lexeme =NULL;
  temp->lexeme = lexeme;

	temp->lineNo = lineNo;
	if(token == TK_ID)
		temp->dataType = 2;
	else if(token == TK_NUM)
		temp->dataType = 0;
	else if(token == TK_RNUM)
		temp->dataType = 1;
	else if(token >= TK_ASSIGNOP && token <= TK_NE && (token-(int)token == 0))
		temp->dataType = 3;
	else
		temp->dataType = 4;
	if(temp->dataType == 0)
	{	temp->val = (Value*)malloc(sizeof(int));
		temp->val->valI = a_to_i(lexeme);
	}
	else if (temp->dataType == 1)
	{	temp->val = (Value*)malloc(sizeof(float));
		temp->val->valF = a_to_f(lexeme);
	}
	else
		temp->val = NULL;

	return temp;
}
