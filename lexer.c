#include "lexerDef.h"

char* bufferCurr;
char* bufferPre;

int currChar;
int fileEnd;
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

    tokenInfo *tk;
    while (tk!=NULL) {
        tk = getNextToken(fp);
        if(tk!=NULL){
            printf("%-20s\t%-30s\t%-5d\n",tokenMap[tk->token],tk->lexeme,tk->lineNo);
        }
    }

}

tokenInfo* getNextToken(FILE *fp){
    char *lexeme = (char*)malloc(MAX_LEXEME_SIZE*sizeof(char));
    memset(lexeme,0,MAX_LEXEME_SIZE*sizeof(char));
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
                        return getTKinfo(TK_NOT,"~",lineNo);

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
                                return getTKinfo(TK_SQL,"[",lineNo);
                            break;

                            case ']':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_SQR,"]",lineNo);
                            break;

                            case ',':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_COMMA,",",lineNo);
                            break;

                            case ';':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_SEM,";",lineNo);
                            break;

                            case ':':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_COLON,":",lineNo);
                            break;

                            case '.':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_DOT,".",lineNo);
                            break;

                            case '(':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_OP,"(",lineNo);
                            break;

                            case ')':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_CL,")",lineNo);
                            break;

                            case '+':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_PLUS,"+",lineNo);
                            break;

                            case '-':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_MINUS,"-",lineNo);
                            break;

                            case '*':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_MUL,"*",lineNo);
                            break;

                            case '/':
                                tkGenerated = 1;
                                finalStateReached = 1;
                                state = 0;
                                currChar++;
                                return getTKinfo(TK_DIV,"/",lineNo);
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
                        if (c==10) {
                            lineNo++;
                        }
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
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);
                    }
                break;

                case 1:
                    if (c == '=') {
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        currChar++;
                        return getTKinfo(TK_LE,"<=",lineNo);;
                    }else if(c == '-'){
                        state = 2;
                        currChar++;
                    }else{
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        return getTKinfo(TK_LT,"<",lineNo);
                    }
                break;

                case 2:
                    if (c=='-') {
                        state = 3;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);
                    }
                break;

                case 3:
                    if (c=='-') {
                        tkGenerated = 1;
                        finalStateReached = 1;
                        state = 0;
                        currChar++;
                        return getTKinfo(TK_ASSIGNOP,"<---",lineNo);;
                    }else{
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);
                    }
                break;

                case 4:
                    if (c == '=') {
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_GE,">=",lineNo);;
                    }else{
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_GT,">",lineNo);;
                    }
                break;

                case 5:
                    if (c == '=') {
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_EQ,"==",lineNo);;
                    }else{
                        printf("Error while tokenizing\n" ); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
                    }
                break;

                case 6:
                    if (c == '=') {
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        state = 0;
                        return getTKinfo(TK_NE,"!=",lineNo);;
                    }else{
                        printf("Error while tokenizing\n" ); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
                    }
                break;

                case 7:

                break;

                case 8:
                    if (c == '@') {
                        state = 9;
                        currChar++;
                    }else{
                        printf("Error while tokeninzing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
                    }
                break;

                case 9:
                    if (c == '@') {
                        state = 0;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_OR,"@@@",lineNo);
                    }else{
                        printf("Error while tokeninzing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
                    }
                break;

                case 10:
                    if (c == '&') {
                        state = 11;
                        currChar++;
                    }else{
                        printf("Error while tokeninzing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
                    }
                break;

                case 11:
                    if (c == '&') {
                        state = 0;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_AND,"&&&",lineNo);
                    }else{
                        printf("Error while tokeninzing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        lexeme[j] = '\0';
                        return getTKinfo(TK_NUM,lexeme,lineNo);
                    }
                break;

                case 15:
                    if (c >=48 && c<=57) {
                        state = 16;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokeninzing\n" ); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
                    }
                break;

                case 16:
                    if (c >=48 && c<=57) {
                        state = 0;
                        lexeme[j++] = c;
                        currChar++;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        lexeme[j] = '\0';
                        return getTKinfo(TK_RNUM,lexeme,lineNo);
                    }else{
                        printf("Error while tokeninzing\n" ); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme[j] = '\0';
                        return getTKinfo(TK_RECORDID,lexeme,lineNo);
                    }else{
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        // lexeme[j++] = c;
                        tkGenerated = 1;
                        finalStateReached = 1;
                        lexeme[j] = '\0';
                        if (j>20) {
                            return getTKinfo(TK_ERROR,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_ID,lexeme,lineNo);
                        }
                    }
                break;

                case 22:
                    if ((c >=50 && c<=55)) {
                        state = 22;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        state =0;
                        // lexeme[j++] = c;
                        tkGenerated = 1;
                        finalStateReached = 1;
                        lexeme[j] = '\0';
                        if (j>20) {
                            return getTKinfo(TK_ERROR,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_ID,lexeme,lineNo);
                        }                    }
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
                        lexeme[j] = '\0';
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
                        lexeme[j++] = c;
                        currChar++;
                    }else if((c >=97 && c<=108)||(c >=110 && c<=122) ||(c >=65 && c<=90)){
                        state = 29;
                        lexeme[j++] = c;
                        currChar++;
                    }else{
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme[j] = '\0';
                        return getTKinfo(TK_MAIN,lexeme,lineNo);
                    }else {
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme[j] = '\0';
                        if (j>30) {
                            return getTKinfo(TK_ERROR,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_FUNID,lexeme,lineNo);
                        }
                    }else{
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
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
                        // if (c==10) {
                        //     lineNo++;
                        // }
                        lexeme[j] = '\0';
                        if (j>30) {
                            return getTKinfo(TK_ERROR,lexeme,lineNo);
                        }else{
                            return getTKinfo(TK_FUNID,lexeme,lineNo);
                        }
                    }else{
                        printf("Error while tokenizing\n"); currChar++;
                        state = 0;
                        finalStateReached = 1;
                        tkGenerated = 1;
                        return getTKinfo(TK_ERROR,"error",lineNo);;
                    }
                break;

            }
        }
    }


}

tokenInfo* getTKinfo(int token,char* lexeme,int lineNo){
	tokenInfo* temp = (tokenInfo*)malloc(sizeof(tokenInfo));
	temp->token = token;
	int len = 0;
	while(lexeme[len] != '\0')
	{
		len++;
	}
	len++;
	// temp->lexeme = (char*)malloc(sizeof(char)*len);
	// int i = 0;
	// while(i < len)
	// {
	// 	temp->lexeme[i] = lexeme[i];
	// 	i++;
	// }
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
		temp->val->valI = atoi(lexeme);
	}
	else if (temp->dataType == 1)
	{	temp->val = (Value*)malloc(sizeof(float));
		temp->val->valF = atof(lexeme);
	}
	else
		temp->val = NULL;

	return temp;
}
