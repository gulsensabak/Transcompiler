#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long long int counter = 1; // for naming the integer variables in llvm file 

void printAlloca( char val[257], FILE* fp){ // a function allocates memory for the vraiables in llvm file
	fprintf(fp, "\t%%%s = alloca i32\n", val);
}


// Returns 'true' if the character is a DELIMITER.
bool isDelimiter(char ch){
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
		ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
		ch == '[' || ch == ']' || ch == '{' || ch == '}' || 
		ch == '|' || ch == '&'|| ch == '%')
		return (true);
	return (false);
}

// Returns 'true' if the character is an OPERATOR.
bool isOperator(char ch){
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '|' || ch == '&' ||
		ch == '=' || ch == ',' || ch == '%' || ch == '/')
		return (true);
	return (false);
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(char* str){
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
		str[0] == '3' || str[0] == '4' || str[0] == '5' ||
		str[0] == '6' || str[0] == '7' || str[0] == '8' ||
		str[0] == '9' || isDelimiter(str[0]) == true)
		return (false);
	return (true);
}

// Returns 'true' if the string is an INTEGER.
bool isInteger(char* str){	
	int i, len = strlen(str);
	
	if (len == 0)
		return (false);	
	
	int sum= 0;
	for (i = 0; i < len; i++) {
	sum++;
	
		if (str[i] != '0' && str[i] != '1' && str[i] != '2'
			&& str[i] != '3' && str[i] != '4' && str[i] != '5'
			&& str[i] != '6' && str[i] != '7' && str[i] != '8'
			&& str[i] != '9' || (str[i] == '-' && i > 0)){

			return (false);}
	}
	return (true);
}

// Extracts the SUBSTRING.
char* subString(char* str, int left, int right){
	int i;
	char* subStr = (char*)malloc(
				sizeof(char) * (right - left + 2));

	for (i = left; i <= right; i++)
		subStr[i - left] = str[i];
	subStr[right - left + 1] = '\0';
	return (subStr);
}

// Returns 'true' if the string is a FUNCTION.
bool isFunction(char c[]){
	if(strcmp(c, "xor") ==0 || strcmp(c, "ls") ==0 || strcmp(c, "rs")==0 || strcmp(c, "lr") ==0 || strcmp(c, "rr")==0 || strcmp(c, "not") == 0)
	return true;
return false;

}

// Returns 'true' if the character is a LEFT PARENTHESES.
bool isLeftParentheses(char ch){
	if(ch == '(')
		return true;
	return false;
}

// Returns 'true' if the character is a RIGHT PARENTHESES.
bool isRightParentheses(char ch){
	if(ch == ')')
		return true;
	return false;
}

// TokenType enumeration
typedef enum {
	// General TokenTypes
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_LEFT_PARENTHESES,
    TOKEN_TYPE_RIGHT_PARENTHESES,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_FUNCTION,
    
    // Spesific TokenTypes
    TOKEN_TYPE_ADD, 
    TOKEN_TYPE_SUBT,   
    TOKEN_TYPE_MULT, 
    TOKEN_TYPE_BAND,
    TOKEN_TYPE_BOR,
    TOKEN_TYPE_XOR, 
    TOKEN_TYPE_LS, 
    TOKEN_TYPE_RS,
    TOKEN_TYPE_NOT,
    TOKEN_TYPE_RR,
    TOKEN_TYPE_LR,
    TOKEN_TYPE_EQUAL,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_MODULO,
    TOKEN_TYPE_DIV,
    TOKEN_TYPE_REG, // refers to intiger variables in llvm file
    TOKEN_TYPE_REGIDENTIFIER, // refers to variables in llvm file
    TOKEN_TYPE_ERROR,
    DEFAULT,
} TokenType;

// Struct a TOKEN data type
typedef struct {
    TokenType type;
    char val[257];
    char identifierValue[257];
} Token;

// takes Token as a parameter and it generalize the type of Token
char isGeneral(Token tok){
	if(tok.type == TOKEN_TYPE_ADD || tok.type == TOKEN_TYPE_SUBT || tok.type == TOKEN_TYPE_MULT || tok.type == TOKEN_TYPE_BAND || tok.type == TOKEN_TYPE_BOR || tok.type == TOKEN_TYPE_MODULO || tok.type == TOKEN_TYPE_DIV) return  TOKEN_TYPE_OPERATOR;
	else if(tok.type == TOKEN_TYPE_XOR || tok.type == TOKEN_TYPE_LS || tok.type == TOKEN_TYPE_RS || tok.type == TOKEN_TYPE_LR || tok.type == TOKEN_TYPE_RR || tok.type == TOKEN_TYPE_NOT) return TOKEN_TYPE_FUNCTION;
}

// Struct a NODE data type
struct Node{
	Token data;
	struct Node* nextNode;
};
typedef struct Node Node;

//QUEUE STARTS HERE
// decleration of functions for queue
void enqueue(Node* *firstNode, Token data);
void dequeue(Node* *firstNode);
void printQueue(Node* currentNode);
bool hasOneElt(Node* currentNode);

// for printing queue	
void printQueue(Node* currentNode){
	Node* tempNode = currentNode;
	if(tempNode == NULL || tempNode->nextNode == NULL){
		printf("Queue is empty\n");
		return; 
	}
	while (tempNode != NULL){
		printf("%s -->", tempNode->data.val);
		tempNode = tempNode->nextNode;
	}
	
	printf("NULL\n");
	return;	
}

// it creates new node and adds it to the queue
void enqueue(Node* *firstNode, Token data){
	Node* newNode = malloc(sizeof(Node));
	
	newNode->data = data;
	newNode->nextNode = NULL;
	
	Node* currentNode = *firstNode;
	
	while(currentNode->nextNode != NULL){
		currentNode = currentNode->nextNode;
	}
	
	currentNode->nextNode = newNode;
}

// it deletes the element from the queue
void dequeue(Node* *startNode){
	
	Node* start = *startNode;
	
	if(start == NULL || start->nextNode == NULL){
		return;
	}
	
	Node* firstNode = start->nextNode;
	Node* secondNode = firstNode->nextNode;
	
	start->nextNode = secondNode;
	
	free(firstNode);
	return;
}

// Returns 'true' if there is one element in the queue
bool hasOneElt(Node* currentNode){
	Node* tempNode = currentNode;
	if(tempNode->nextNode->nextNode == NULL ){
		return true; 
	}

	return false;	
}
//QUEUE ENDS HERE

//STACK STARTS HERE
// decleration of functions for stack
void push(Node* *firstNode, Token data);
Token peek(Node* *firstNode);
void pop(Node* *firstNode);
bool isEmpty(Node* firstNode);
void printStack(Node* currentNode);


// it creates a node, adds a Token inside of it and pushes into the Stack 
void push(Node* *firstNode, Token data){
	Node* start = *firstNode;
	Node* newNode = malloc(sizeof(Node));
	
	newNode->data = data;
	newNode->nextNode = NULL;
	
	if(start == NULL || start->nextNode == NULL){
		start->nextNode = newNode;
		return;
	}
	Node* firstNodeOfStack = start->nextNode;
	
	newNode->nextNode = firstNodeOfStack;
	start->nextNode = newNode;
	return;
}

// it returns the last Token in the Stack
Token peek(Node* *startNode){
	Node* start = *startNode;
	if(start->nextNode != NULL)
		return start->nextNode->data;
}

// it deletes the last elelement in the stack
void pop(Node* *startNode){
	Node* start = *startNode;
	if(start == NULL || start->nextNode == NULL){
		return;
	}
	Node* firstNode = start->nextNode;
	
	Token tokenToReturn = firstNode->data;
	
	Node* secondNode = firstNode->nextNode;
	
	start->nextNode = secondNode;
	
	free(firstNode);
	return;
}

// it controls whether stack is empty or not
bool isEmpty(Node* firstNode){
	Node* start = firstNode;
	
	if(start == NULL || start->nextNode == NULL){
		return true;
	}
	else{
		return false;
	}
}

// it prints the stack
void printStack(Node* currentNode){
	Node* tempNode = currentNode;
	if(tempNode == NULL || tempNode->nextNode == NULL){
		printf("Stack is empty\n");
		return; 
	}
	while (tempNode != NULL){
		printf("%s --->", tempNode->data.val);
		tempNode = tempNode->nextNode;
	}
	
	printf("NULL\n");
	return;
}
//STACK ENDS HERE

//HASH TABLE STARTS HERE
// decleration of functions for dictionary 
char* get(Node* startNode ,char* identifier);
void add(Node* *startNode, Token newToken);
void printDict(Node* startNode);
void updateValue(Node* startNode, char newValue[257], char identifier[257] );
bool doesExist(Node* startNode ,char* identifier);

// it creates a node, adds a Token inside of it and adds into the Dictionary
void add(Node* *startNode, Token newToken){
	Node* start = *startNode;
	Node* newNode = malloc(sizeof(Node));
	
	newNode->data = newToken;
	newNode->nextNode = NULL;
	newNode->data.identifierValue[0] = '0';
	newNode->data.identifierValue[1] = '\0';
	
	if(start == NULL || start->nextNode == NULL){
		start->nextNode = newNode;
		return;
	}
	Node* firstNodeOfDict = start->nextNode;
	
	newNode->nextNode = firstNodeOfDict;
	start->nextNode = newNode;
	return;
}

// it returns the value of given identifier
char* get(Node* startNode ,char identifier[257]){
	Node* tempNode = startNode;
	
	char* str = malloc(sizeof(char)*257);	
	while (tempNode != NULL){
		if(strcmp(tempNode->data.val, identifier) == 0){
			strcpy(str,tempNode->data.identifierValue);
			return str;		
		}	
		tempNode = tempNode->nextNode;
	}
	return str;
	
}

// it prints the elements of Dictionary
void printDict(Node* startNode){
	Node* tempNode = startNode;
	if(tempNode == NULL || tempNode->nextNode == NULL){
		printf("Dictionary is empty\n");
		return; 
	}
	while (tempNode != NULL){
		printf("(%s, %s) ~~>", tempNode->data.val, tempNode->data.identifierValue);
		tempNode = tempNode->nextNode;
	}
	
	printf("NULL\n");	
}

// it updates the value of given identifer
void updateValue(Node* startNode, char newValue[257], char identifier[257]){
	Node* tempNode = startNode;
	while (tempNode != NULL){
		if(strcmp(tempNode->data.val, identifier) == 0){
			strcpy(tempNode->data.identifierValue, newValue);
			return;
		}	
		tempNode = tempNode->nextNode;
	}
	
	return;
}

// it controls whether the identifer exist in Dictionary or not
bool doesExist(Node* startNode ,char* identifier){
	Node* tempNode = startNode;
		
	while (tempNode != NULL){
		if(strcmp(tempNode->data.val, identifier) == 0){
			return true;		
		}	
		tempNode = tempNode->nextNode;
	}
	
	return false;
}
//DICTIONARY ENDS HERE

//determine the exact operator
char whichOperator(char ch){
	if(ch == '+') return TOKEN_TYPE_ADD;
	else if(ch == '-') return TOKEN_TYPE_SUBT;
	else if(ch == '*') return TOKEN_TYPE_MULT;
	else if(ch == '&') return TOKEN_TYPE_BAND;
	else if(ch == '|') return TOKEN_TYPE_BOR;
	else if(ch == '=') return TOKEN_TYPE_EQUAL;
	else if(ch == ',') return TOKEN_TYPE_COMMA;
	else if(ch == '/') return TOKEN_TYPE_DIV;
	else if(ch == '%') return TOKEN_TYPE_MODULO;
}

// returns the precedence of the given Token
int precedence(Token tok){
	if(tok.type == TOKEN_TYPE_BOR) return 1;
	else if(tok.type== TOKEN_TYPE_BAND) return 2;
	else if(tok.type == TOKEN_TYPE_SUBT || tok.type == TOKEN_TYPE_ADD) return 3;
	else if(tok.type== TOKEN_TYPE_MULT || tok.type == TOKEN_TYPE_MODULO || tok.type == TOKEN_TYPE_DIV) return 4;
	else if(tok.type == TOKEN_TYPE_XOR || tok.type == TOKEN_TYPE_LR || tok.type == TOKEN_TYPE_RR || tok.type == TOKEN_TYPE_LS || tok.type == TOKEN_TYPE_RS || tok.type == TOKEN_TYPE_NOT ) return 5;
	else if(tok.type == TOKEN_TYPE_EQUAL) return 0;
	
	else return -1;
}

// returns the precedence of given TokenType
int precedence2(TokenType tt){
	if(tt == TOKEN_TYPE_BOR) return 1;
	else if(tt== TOKEN_TYPE_BAND) return 2;
	else if(tt == TOKEN_TYPE_SUBT || tt == TOKEN_TYPE_ADD) return 3;
	else if(tt== TOKEN_TYPE_MULT || tt == TOKEN_TYPE_MODULO || tt == TOKEN_TYPE_DIV) return 4;
	else if(tt == TOKEN_TYPE_XOR || tt == TOKEN_TYPE_LS || tt == TOKEN_TYPE_RS || tt == TOKEN_TYPE_RR || tt == TOKEN_TYPE_LR || tt == TOKEN_TYPE_NOT) return 5;
	else if(tt == TOKEN_TYPE_EQUAL) return 0;	
	else return -1;
}


int numOfTokens; //number of Tokens in the line
int numOfIdentifiers; //number of identifers in the line
int numOfEq; //number of equal signs in the line
int numOfLeftPar; //number of open parenthesis in the line 
int numOfRightPar; //number of close parenthesis in the line
int numOfComma = 0; // number of commas in a line.
int	numOfFuncs = 0;// number of functions except "not" function in a line.

// Parsing the input STRING.
Token * parse(char* str){
	Token * tokenArray; // decleration of tokenArray 
	tokenArray = (Token*) calloc(257, sizeof(Token)); // initialization of tokenArray with calloc, to make it dynamic
	int left = 0, right = 0;
	int len = strlen(str) - 1;
	
	int i = 0;
	
	numOfTokens= 0; // initialization of number of Tokens. When we call parse function, value of numOfTokens will be zero.
	numOfIdentifiers = 0; // initialization of number of Identifiers. When we call parse function, value of numOfIdentifiers will be zero.
	numOfEq = 0; // initialization of number of Equal signs. When we call parse function, value of numOfEq will be zero.
	numOfLeftPar = 0;	// initialization of number of open parentheses. When we call parse function, value of numOfLeftPar will be zero.
	numOfRightPar = 0; // initialization of number of close parentheses. When we call parse function, value of numOfRightPar will be zero.
	numOfComma = 0; // number of commas in a line.
	numOfFuncs = 0;// number of functions except "not" function in a line.

	while (right <= len && left <= right) {
		Token token;
		
			
		if(str[right] == '=')
			numOfEq++;
		if (isDelimiter(str[right]) == false)
			right++;

		if (isDelimiter(str[right]) == true && left == right) {
			// determine whether the coming token is operator or not
			if (isOperator(str[right]) == true){
				token.type = whichOperator(str[right]);
				token.val[0]= str[right];
				token.val[1]= '\0';
				tokenArray[i] = token;
				i++;
				numOfTokens++;
				if(token.type == TOKEN_TYPE_COMMA)
					numOfComma++;
				}
				
			// determine whether the coming token is open parentheses or not	
			else if(isLeftParentheses(str[right]) == true){
				token.type = TOKEN_TYPE_LEFT_PARENTHESES;
				token.val[0]= str[right];
				token.val[1]= '\0';
				tokenArray[i] = token;
				i++;
				numOfTokens++;
				numOfLeftPar++;
				}

			// determine whether the coming token is close parentheses or not
			else if(isRightParentheses(str[right]) == true){
				token.type = TOKEN_TYPE_RIGHT_PARENTHESES;
				token.val[0]= str[right];
				token.val[1]= '\0';
				tokenArray[i] = token;
				i++;
				numOfTokens++;
				numOfRightPar++;
				}

			right++;
			left = right;
		} else if (isDelimiter(str[right]) == true && left != right || (right == len && left != right)) {
		
			char* subStr = subString(str, left, right - 1);

			// determine wether the coming token is function or not
			if(isFunction(subStr) == true){
				// determine wether the coming token's type is xor or not
				if(strcmp(subStr, "xor") == 0){					
					token.type = TOKEN_TYPE_XOR;
					strcpy(token.val,subStr);
					tokenArray[i] = token;
					i++;
					numOfTokens++;
					numOfFuncs++;
					}
				// determine wether the coming token's type is ls or not
				else if(strcmp(subStr, "ls") == 0){				
					token.type = TOKEN_TYPE_LS;
					strcpy(token.val,subStr);
					tokenArray[i] = token;
					i++;
					numOfTokens++;
					numOfFuncs++;
				}
				// determine wether the coming token's type is rs or not
				else if(strcmp(subStr, "rs") == 0){
					token.type = TOKEN_TYPE_RS;
					strcpy(token.val,subStr);
					tokenArray[i] = token;
					i++;
					numOfTokens++;
					numOfFuncs++;
				}
				// determine wether the coming token's type is lr or not
				else if(strcmp(subStr, "lr") == 0){
					token.type = TOKEN_TYPE_LR;
					strcpy(token.val,subStr);
					tokenArray[i] = token;
					i++;
					numOfTokens++;
					numOfFuncs++;
				}
				// determine wether the coming token's type is rr or not
				else if(strcmp(subStr, "rr") == 0){
					token.type = TOKEN_TYPE_RR;
					strcpy(token.val,subStr);
					tokenArray[i] = token;
					i++;
					numOfTokens++;
					numOfFuncs++;
				}
				// determine wether the coming token's type is not or not
				else if(strcmp(subStr, "not") == 0){
					token.type = TOKEN_TYPE_NOT;
					strcpy(token.val,subStr);
					tokenArray[i] = token;
					i++;
					numOfTokens++;
				}
			}
			
			// determine wether the coming token is integer or not
			else if (isInteger(subStr) == true){
				if(subStr != " "){
				token.type = TOKEN_TYPE_NUMBER;
				strcpy(token.val,subStr);
				tokenArray[i] = token;
				i++;
				numOfTokens++;
				}
			}
			
			// determine wether the coming token is identifier or not
			else if (validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false){
				token.type = TOKEN_TYPE_IDENTIFIER;
				strcpy(token.val,subStr);
				tokenArray[i] = token;
				i++;
				numOfTokens++;	
				numOfIdentifiers++;
				}
				
			else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false){
                     		strcpy(token.val,subStr);
				tokenArray[i] = token;
				i++;
				numOfTokens++; 
            }

			left = right;
		}
	}
	return tokenArray;
}

long long int calculate(char str1[], char str2[], TokenType typeOfOperator, FILE* fp); 

long long int calculate(char str1[], char str2[], TokenType typeOfOperator, FILE* fp){ // writes the operataions and functions to the llvm file in the correct format
// str2 is the first number/variable in the given operation, str1 is the second number/variable in the given operation
// it returns the name of llvm variable without "%" sign which stores the value of the operation

	switch(typeOfOperator){
		case TOKEN_TYPE_ADD:
			fprintf(fp, "\t%%%lld = add i32 %s,%s\n", counter, str2, str1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_SUBT:
			fprintf(fp, "\t%%%lld = sub i32 %s,%s\n", counter, str2, str1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_DIV:
			fprintf(fp, "\t%%%lld = sdiv i32 %s,%s\n", counter, str2, str1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_MODULO:
			fprintf(fp, "\t%%%lld = srem i32 %s,%s\n", counter, str2, str1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_MULT:
			fprintf(fp, "\t%%%lld = mul i32 %s,%s\n", counter, str2, str1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_BOR:
			fprintf(fp, "\t%%%lld = or i32 %s,%s\n", counter, str1, str2);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_BAND:
			fprintf(fp, "\t%%%lld = and i32 %s,%s\n", counter, str1, str2);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_XOR:
			fprintf(fp, "\t%%%lld = xor i32 %s,%s\n", counter, str1, str2);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_LS:
			fprintf(fp, "\t%%%lld = shl i32 %s,%s\n", counter, str2, str1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_RS:
			fprintf(fp, "\t%%%lld = ashr i32 %s,%s\n", counter, str2, str1);
			counter++;
			return counter - 1;
			break;	
		case TOKEN_TYPE_NOT:
			fprintf(fp, "\t%%%lld = xor i32 %s,%d\n", counter, str1, -1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_RR:
			fprintf(fp, "\t%%%lld = sub i32 %d,%s\n", counter, 32, str1);
			counter++;
			fprintf(fp, "\t%%%lld = lshr i32 %s,%s\n", counter, str2, str1);
			counter++;
			fprintf(fp, "\t%%%lld = shl i32 %s,%%%lld\n", counter, str2, counter - 2);
			counter++;
			fprintf(fp, "\t%%%lld = or i32 %%%lld,%%%lld\n", counter, counter - 2, counter - 1);
			counter++;
			return counter - 1;
			break;
		case TOKEN_TYPE_LR:
			fprintf(fp, "\t%%%lld = sub i32 %d,%s\n", counter, 32, str1);
			counter++;
			fprintf(fp, "\t%%%lld = shl i32 %s,%s\n", counter, str2, str1);
			counter++;
			fprintf(fp, "\t%%%lld = lshr i32 %s,%%%lld\n", counter, str2, counter - 2);
			counter++;
			fprintf(fp, "\t%%%lld = or i32 %%%lld,%%%lld\n", counter, counter - 2, counter - 1);
			counter++;
			return counter - 1;
			break;	
		default:
			printf("Error on the line!\n");
			return 1;				
	}
}


// MAIN FUNCTION
int main(int argc, char *argv[])
{
	
	FILE * fp; // initialize llvm file
	
	char argv1[257];
	strcpy(argv1, argv[1]);
	char *outputFile = strtok(argv1, ".");
	strcat(outputFile, ".ll");
	
   	fp = fopen (outputFile, "w");
   	fclose(fp);
   	
   	fp = fopen (outputFile, "a");
   	
   	fprintf(fp, "; ModuleID = 'advcalc2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n\n"); // write the required lines for print statements to llvm file
   	fprintf(fp, "define i32 @main() {\n");
	
	fclose(fp);
	
	
	bool isErrorGeneral = false; // if there is at least one error in the input file, this will be true and at the end of the execution llvm file will be deleted.
	
	char line[257] = "";  // String which will be used to store input lines
	
	Node* firstNodeOfDict = (Node*) malloc(sizeof(Node));  // initial Node of Dictionary
	Node* firstNode = (Node*) malloc(sizeof(Node));        // initial Node of Queue
	Node* firstNodeOfStack = (Node*) malloc(sizeof(Node)); // initial Node of Stack
	
	FILE * read; // initialize input file.

	read = fopen(argv[1], "r"); // define input file
	
	int lineCounter = 0; // for counting the line of errors.
	
	while(fgets(line, sizeof(line), read)){
			
		lineCounter++;
		
		bool isError = false; // indicates if the line has an error or not.
				
		Token *tokenArray; // holds Tokens of the lines here
		tokenArray = parse(line);		
		
//ERROR HANDLINGS STARTS HERE
		bool warn= true;
		if(numOfTokens == 0){ // if there is no tokens, continues.
		    continue;
		}
		if(numOfEq > 1){ // if there is more than one equal sign, returns an error.
			printf("Error on line %d!\n", lineCounter);
			isError = true;
			isErrorGeneral = true;
			continue;
		}
		if(numOfRightPar != numOfLeftPar){ // if number of open parentheses is not equal to number of close parentheses, returns an error.
			printf("Error on line %d!\n", lineCounter);
			isError = true;
			isErrorGeneral = true;
			continue;
		}
		if(numOfComma != numOfFuncs){ // if number of commas is not equal to the number of functions except "not" function, gives an error.
			printf("Error on line %d!\n", lineCounter); 
			isError = true;
			isErrorGeneral = true;
			continue;
		}
		if(numOfTokens == 1 && tokenArray[0].type != TOKEN_TYPE_NUMBER && tokenArray[0].type != TOKEN_TYPE_IDENTIFIER){ // if there is one Token and if it is not a number or a identifier, returns an error.
			printf("Error on line %d!\n", lineCounter);
			isError = true;
			isErrorGeneral = true;
			continue;	
		}		
		if(isGeneral(tokenArray[numOfTokens-1]) == TOKEN_TYPE_OPERATOR || isGeneral(tokenArray[numOfTokens-1]) == TOKEN_TYPE_FUNCTION){ // if there is a operator or a function at the end of the line, gives an error.
			printf("Error on line %d!\n", lineCounter);
			isError = true;
			isErrorGeneral = true;
			continue;
		}
		
		
		int m;

		for(int k = 0; k < numOfTokens -1; k++){
			int l = k+1;
			m = k - 1;
			if(k>0) {
				m = k-1;
				if(	tokenArray[m].type != TOKEN_TYPE_NUMBER && tokenArray[m].type != TOKEN_TYPE_IDENTIFIER && tokenArray[m].type != TOKEN_TYPE_RIGHT_PARENTHESES){ // gives an error for unary minus sign.
					if(tokenArray[k].type == TOKEN_TYPE_SUBT && (tokenArray[l].type == TOKEN_TYPE_NUMBER || tokenArray[l].type == TOKEN_TYPE_IDENTIFIER)){
						printf("Error on line %d!\n", lineCounter);
						isError = true;
						isErrorGeneral = true;
						warn = false;
						break;
					}
				}	
	
			}
			if(k == 0 && isGeneral(tokenArray[k]) == TOKEN_TYPE_OPERATOR){ // if first token is an operator, gives an error.
				printf("Error on line %d!\n", lineCounter);
				isError = true;
				isErrorGeneral = true;
				warn = false;
				break;
			}
			if((k == 0 && tokenArray[k].type == TOKEN_TYPE_LEFT_PARENTHESES) || (tokenArray[k].type == TOKEN_TYPE_LEFT_PARENTHESES && isGeneral(tokenArray[m]) != TOKEN_TYPE_FUNCTION)){ // if "," is used between paretheses without using a function gives an error.
					int c = k + 1;
					bool warn2 = true;
					while(c < numOfTokens && tokenArray[c].type != TOKEN_TYPE_RIGHT_PARENTHESES && tokenArray[c].type != TOKEN_TYPE_LEFT_PARENTHESES){
						if(tokenArray[c].type == TOKEN_TYPE_COMMA){

							warn2 = false;
							break;
						}
						c++;
					}
					if(!warn2){
						printf("Error on line %d!\n", lineCounter);
						isError = true;
						isErrorGeneral = true;
						warn = false;
						break;
					}
					
				}
			if(isGeneral(tokenArray[k]) == TOKEN_TYPE_FUNCTION){ // returns an error if there is not a left parentheses after a function
				if(tokenArray[l].type != TOKEN_TYPE_LEFT_PARENTHESES){
					printf("Error on line %d!\n", lineCounter);
					isError = true;
					isErrorGeneral = true;
					warn = false;
					break;
				}
			}
			if(tokenArray[k].type == TOKEN_TYPE_EQUAL){  // if first Token is equal sign or there are more than one Token on the LHS of the equal sign, gives an error.
				if(k == 0){
					printf("Error on line %d!\n", lineCounter);
					isError = true;
					isErrorGeneral = true;
					warn = false;
					break;
				}
				if(k == 1 && tokenArray[k-1].type == TOKEN_TYPE_IDENTIFIER) {

					warn = true;
				}
				else{
					printf("Error on line %d!\n", lineCounter);
					isError = true;
					isErrorGeneral = true;
					warn = false;
					break;
				}	
			}
			if(tokenArray[k].type == TOKEN_TYPE_LEFT_PARENTHESES && (tokenArray[l].type == TOKEN_TYPE_RIGHT_PARENTHESES || isGeneral(tokenArray[l]) == TOKEN_TYPE_OPERATOR)){ // if there are empty brackets or brackets starting with an operator, gives an error.
				printf("Error on line %d!\n", lineCounter);
				isError = true;
				isErrorGeneral = true;
				warn = false;
				break;
			}
			if(tokenArray[k].type == TOKEN_TYPE_NUMBER && tokenArray[l].type == TOKEN_TYPE_NUMBER){ // if there are two sequential numbers without anything between them, gives an error.
				printf("Error on line %d!\n", lineCounter);	
				isError = true;
				isErrorGeneral = true;
				warn= false;
				break;			
			}
			if(tokenArray[k].type == TOKEN_TYPE_IDENTIFIER && tokenArray[l].type == TOKEN_TYPE_IDENTIFIER){ // if there are two sequential identifiers without anything between them, gives an error.
				printf("Error on line %d!\n", lineCounter);	
				isError = true;
				isErrorGeneral = true;
				warn= false;
				break;			
			}
			if(isGeneral(tokenArray[k]) == TOKEN_TYPE_OPERATOR && isGeneral(tokenArray[l]) == TOKEN_TYPE_OPERATOR || isGeneral(tokenArray[k]) == TOKEN_TYPE_FUNCTION && isGeneral(tokenArray[l]) == TOKEN_TYPE_FUNCTION ) { // if there are two sequential operators or functions, gives an error.
				printf("Error on line %d!\n", lineCounter);	
				isError = true;
				isErrorGeneral = true;
				warn= false;
				break;			
			}
			if(tokenArray[k].type == TOKEN_TYPE_COMMA && isGeneral(tokenArray[l]) == TOKEN_TYPE_OPERATOR){ // if there is a comma followed by an operator, gives an error.
				printf("Error on line %d!\n", lineCounter);
				isError = true;
				isErrorGeneral = true;
				warn = false;	
				break;
			}
				
		}
		
		// ERROR HANDLINGS ENDS HERE
		if(warn) {
			fp = fopen (outputFile, "a");
			//INFIX TO POSTFIX PART

			//This for loop and following while loop transforms the line from infix to postfix notation using the Shunting-yard Algorithm.
			for(int j = 0; j < numOfTokens ; j++){
				if(tokenArray[j].type == TOKEN_TYPE_NUMBER || tokenArray[j].type == TOKEN_TYPE_IDENTIFIER){ // if the Token is a number or a identifer, adds it to the queue.
					enqueue(&firstNode, tokenArray[j]);
					continue;
				}
				
				
				if(tokenArray[j].type == TOKEN_TYPE_ADD ||tokenArray[j].type == TOKEN_TYPE_MULT ||tokenArray[j].type == TOKEN_TYPE_SUBT ||tokenArray[j].type == TOKEN_TYPE_BAND ||
				tokenArray[j].type == TOKEN_TYPE_BOR || tokenArray[j].type == TOKEN_TYPE_XOR || tokenArray[j].type == TOKEN_TYPE_LS || tokenArray[j].type == TOKEN_TYPE_RS|| 
				tokenArray[j].type == TOKEN_TYPE_LR|| tokenArray[j].type == TOKEN_TYPE_RR|| tokenArray[j].type == TOKEN_TYPE_NOT || tokenArray[j].type == TOKEN_TYPE_EQUAL ||
				tokenArray[j].type == TOKEN_TYPE_DIV|| tokenArray[j].type == TOKEN_TYPE_MODULO){
				// if the Token is an operator, followings is done
						
					if(isEmpty(firstNodeOfStack)){	// if the Stack is empty, it adds the Token to the Stack	
							
						push(&firstNodeOfStack, tokenArray[j]);
					}
					else{	
						while(peek(&firstNodeOfStack).type != TOKEN_TYPE_LEFT_PARENTHESES && precedence(peek(&firstNodeOfStack)) >=  precedence(tokenArray[j])){ // while the top element of the Stack is not "(" and the top element's precedence is greater than or equal to the new Token, pop Tokens from the Stack and add themk to Queue
							
							enqueue(&firstNode, peek(&firstNodeOfStack));
							pop(&firstNodeOfStack);
							
							if(isEmpty(firstNodeOfStack))
								break;
						}
						push(&firstNodeOfStack, tokenArray[j]);
				
					}
					continue;
				}
				if(tokenArray[j].type == TOKEN_TYPE_LEFT_PARENTHESES){ // if the Token is a "(" add it to the Stack
					push(&firstNodeOfStack, tokenArray[j]);
					continue;
				}
				if(tokenArray[j].type == TOKEN_TYPE_RIGHT_PARENTHESES){
					while(peek(&firstNodeOfStack).type != TOKEN_TYPE_LEFT_PARENTHESES){ // if the Token is a ")", while the top element of the Stack is not a "(", pop Tokens from the Stack, and add them to the Queue
						enqueue(&firstNode, peek(&firstNodeOfStack));
						pop(&firstNodeOfStack);
						
					}
					pop(&firstNodeOfStack);
					continue;
				}
				if(tokenArray[j].type == TOKEN_TYPE_COMMA){
					while(peek(&firstNodeOfStack).type != TOKEN_TYPE_LEFT_PARENTHESES){ //if the Token is a ",", while the top element of the Stack is not a "(", pop Tokens from the Stack, and add them to the Queue
						enqueue(&firstNode, peek(&firstNodeOfStack));
						pop(&firstNodeOfStack);
					}
					continue;
				}
				
			}	
			
			// it enqueues the remaining Tokens in the Stack		
			while(!isEmpty(firstNodeOfStack)){
				enqueue(&firstNode, peek(&firstNodeOfStack));
				pop(&firstNodeOfStack);	
			}

			//CALCULATION PART
			
			//This while loop calculates the expressions and pushes the final result in to the Stack
			int numOfDefinedVars = 0; // number of newly defined variables in a line
			while(firstNode->nextNode != NULL){
				if(firstNode->nextNode->data.type == TOKEN_TYPE_IDENTIFIER){	 // if the Token is an identifer, it adds it to the Dictionary and to the Stack. If it already exists, adds it to the Stack only.			
					if(hasOneElt(firstNode)){  
						if(!doesExist(firstNodeOfDict, firstNode->nextNode->data.val)){ // if there is one variable on the line and it is not defined, isError is true
							firstNode->nextNode->data.type = TOKEN_TYPE_ERROR;
							isError = true;
							isErrorGeneral = true;
							push(&firstNodeOfStack, firstNode->nextNode->data);
							dequeue(&firstNode);
							break;
						}
						
						
						push(&firstNodeOfStack, firstNode->nextNode->data);
						dequeue(&firstNode);
						break;
					}
					else{
						if(!doesExist(firstNodeOfDict, firstNode->nextNode->data.val)){
							if(numOfEq != 1){ // if there is not a equal sign and there is a undefined variable, isError is true
								firstNode->nextNode->data.type = TOKEN_TYPE_ERROR;
								isError = true;
								isErrorGeneral = true;
								push(&firstNodeOfStack, firstNode->nextNode->data);
								dequeue(&firstNode);
								break;
							}
							if(numOfDefinedVars > 0 && numOfEq == 1){ // if there are two newly defined variables and one equal sign, gives an error
								firstNode->nextNode->data.type = TOKEN_TYPE_ERROR;
								isError = true;
								isErrorGeneral = true;
								push(&firstNodeOfStack, firstNode->nextNode->data);
								dequeue(&firstNode);
								printf("Error on line %d!\n", lineCounter);
								break;
							}
							firstNode->nextNode->data.type == TOKEN_TYPE_REGIDENTIFIER;
							add(&firstNodeOfDict, firstNode->nextNode->data);
							printAlloca(firstNode->nextNode->data.val, fp);
							numOfDefinedVars++;	
						}
						
						
						push(&firstNodeOfStack, firstNode->nextNode->data);
						dequeue(&firstNode);
					}
					
					continue;
				}
				if(firstNode->nextNode->data.type == TOKEN_TYPE_NUMBER){ // if the Token is a number, it deletes from the Queue and adds to the Stack.
					push(&firstNodeOfStack, firstNode->nextNode->data);
					dequeue(&firstNode);
					continue;
				}
				if(firstNode->nextNode->data.type == TOKEN_TYPE_ADD || firstNode->nextNode->data.type == TOKEN_TYPE_MULT || firstNode->nextNode->data.type == TOKEN_TYPE_SUBT ||
				firstNode->nextNode->data.type == TOKEN_TYPE_BAND || firstNode->nextNode->data.type == TOKEN_TYPE_BOR || firstNode->nextNode->data.type == TOKEN_TYPE_XOR || 
				firstNode->nextNode->data.type == TOKEN_TYPE_LS || firstNode->nextNode->data.type == TOKEN_TYPE_RS|| firstNode->nextNode->data.type == TOKEN_TYPE_LR|| 
				firstNode->nextNode->data.type == TOKEN_TYPE_RR || firstNode->nextNode->data.type == TOKEN_TYPE_DIV || firstNode->nextNode->data.type == TOKEN_TYPE_MODULO){ 
				// if the Token is one of the above operators, it is calculated here.
								
					Token firstOperand = peek(&firstNodeOfStack);
					pop(&firstNodeOfStack);
					Token secondOperand = peek(&firstNodeOfStack);
					pop(&firstNodeOfStack);
					if(firstOperand.type != TOKEN_TYPE_NUMBER && firstOperand.type != TOKEN_TYPE_IDENTIFIER && firstOperand.type != TOKEN_TYPE_REG && secondOperand.type != TOKEN_TYPE_NUMBER && secondOperand.type != TOKEN_TYPE_IDENTIFIER && secondOperand.type != TOKEN_TYPE_REG){
						printf("Error on the line!\n");
						break;
					}
					
					char str1[257];
					char str2[257];
					//if elses below controls whether the Tokens are identifers or numbers or reg or regidentifier and writes to the llvm file accordingly . 
					if(firstOperand.type == TOKEN_TYPE_NUMBER || firstOperand.type == TOKEN_TYPE_REG){ 
						strcpy(str1, firstOperand.val);
					}
					else{
						fprintf(fp, "\t%%%lld = load i32, i32* %%%s\n", counter, firstOperand.val); 
						sprintf(str1, "%%%lld", counter);
						counter++;
					}
					if(secondOperand.type == TOKEN_TYPE_NUMBER || secondOperand.type == TOKEN_TYPE_REG){
						strcpy(str2, secondOperand.val);
					}	
					else{
						fprintf(fp, "\t%%%lld = load i32, i32* %%%s\n", counter, secondOperand.val);
						sprintf(str2, "%%%lld", counter);
						counter++;
					}
					
					long long int result = calculate(str1, str2, firstNode->nextNode->data.type, fp);
					char resultForToken[257];
					sprintf(resultForToken, "%%%lld", result);
									
					Token newToken;
					newToken.type = TOKEN_TYPE_REG;
					strcpy(newToken.val, resultForToken);
									
					push(&firstNodeOfStack, newToken);
					dequeue(&firstNode);
				}	
				
				else if(firstNode->nextNode->data.type == TOKEN_TYPE_NOT){ // calculation for operator "not". 
					Token operand = peek(&firstNodeOfStack);
					pop(&firstNodeOfStack);

					if(operand.type != TOKEN_TYPE_NUMBER && operand.type != TOKEN_TYPE_IDENTIFIER && operand.type != TOKEN_TYPE_REG){
						printf("Error on the line!\n");
						break;
					}
					char str[257];
					if(operand.type == TOKEN_TYPE_NUMBER || operand.type == TOKEN_TYPE_REG){
						strcpy(str, operand.val);
					}
					else{						
						fprintf(fp, "\t%%%lld = load i32, i32* %%%s\n", counter, operand.val);
						sprintf(str, "%%%lld", counter);
						counter++;
					}
					
					long long int result = calculate(str, str, firstNode->nextNode->data.type, fp);
								
					char resultForToken[257];
					sprintf(resultForToken, "%%%lld", result);
					Token newToken;
					
					newToken.type = TOKEN_TYPE_REG;
					strcpy(newToken.val, resultForToken);
					push(&firstNodeOfStack, newToken);
					dequeue(&firstNode);
				}
				else if(firstNode->nextNode->data.type == TOKEN_TYPE_EQUAL){ // when it sees an equal sign, it updatess the value of the variable.
					Token operand = peek(&firstNodeOfStack);
					pop(&firstNodeOfStack);
					Token identifier = peek(&firstNodeOfStack);
					pop(&firstNodeOfStack);
					if(identifier.type != TOKEN_TYPE_IDENTIFIER && identifier.type != TOKEN_TYPE_REGIDENTIFIER ){
						printf("Error on the line!\n");
						break;
					}
						
					if(operand.type == TOKEN_TYPE_NUMBER || operand.type == TOKEN_TYPE_REG){
						updateValue(firstNodeOfDict, operand.val, identifier.val);
						fprintf(fp, "\tstore i32 %s, i32* %%%s\n", operand.val, identifier.val);
					}	
					else{
						updateValue(firstNodeOfDict, get(firstNodeOfDict, operand.val), identifier.val);
						fprintf(fp, "\t%%%lld = load i32, i32* %%%s\n", counter, operand.val);
						fprintf(fp, "\tstore i32 %%%lld, i32* %%%s\n", counter, identifier.val);
						counter++;
						
					}
								
					dequeue(&firstNode);
					continue;
				}
				else if(firstNode->nextNode->data.type == TOKEN_TYPE_COMMA){
					dequeue(&firstNode);
					continue;
				}	
			}
			
			// Here final result is popped from the Stack and it is written to the llvm file
			if(numOfEq != 1){
				char finalResult[257];
				
				if(isError || peek(&firstNodeOfStack).type == TOKEN_TYPE_ERROR){
					printf("Error on line %d!\n", lineCounter);
					pop(&firstNodeOfStack);
				}
				else{
					if(peek(&firstNodeOfStack).type == TOKEN_TYPE_REG){
						strcpy(finalResult, peek(&firstNodeOfStack).val);
						pop(&firstNodeOfStack);
					}
					else if(peek(&firstNodeOfStack).type == TOKEN_TYPE_NUMBER){
						sprintf(finalResult, "%s", peek(&firstNodeOfStack).val);
						pop(&firstNodeOfStack);
					}
					else{
						fprintf(fp, "\t%%%lld = load i32, i32* %%%s\n", counter, peek(&firstNodeOfStack).val);
						sprintf(finalResult, "%%%lld", counter);
						pop(&firstNodeOfStack);
						counter++;
					}
					
					fprintf(fp, "\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s )\n", finalResult);
					counter++;
				}
			}	
					
			firstNodeOfStack->nextNode = NULL;
			firstNode->nextNode = NULL;
			fclose(fp);
		}
     	//freeing allocated memory
		free(tokenArray);	
  		
  
  	}
  	if(isErrorGeneral){ // if there is an error, remove the file
  		remove(outputFile);
  	}
  	else{
	  	fp = fopen(outputFile, "a");
	  	fprintf(fp, "\tret i32 0\n\n}");
	  	fclose(fp);
	} 	
  	//freeing allocated memory
	free(firstNodeOfDict);
	free(firstNode);
	free(firstNodeOfStack);
	
}
