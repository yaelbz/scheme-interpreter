/*
 * reader.c
 * repl --> reader
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "global.h"
#include "reader.h"
#include "symbolTable.h"


#define CHARSTACK_SIZE   2
static int charStackPtr = -1;
static int charStack[CHARSTACK_SIZE];

//--------------- initialization ----------------------//
// #### init ##########################################################################################

void initReader() {
	flushReaderInputStream();
}

void flushReaderInputStream() {
	charStackPtr = -1;
}

// #### helper functions ##########################################################################################

bool isBinaryDigit(int ch){
	return (ch>='0' && ch<='1');
}

bool isOctalDigit(int ch){
	return (ch>='0' && ch<='7');
}

bool isDecimalDigit(int ch){
	return (ch>='0' && ch<='9');
}

bool isHexadecimalDigit(int ch){
	return ((ch>='0' && ch<='9') || (ch>='a' && ch<='f') || (ch>='A' && ch<='F'));
}

int convertHexDigit(int ch) {
	int chval = -1;
	if(ch>='0' && ch<='9') {
		chval = (ch - '0');
	} else if(ch>='a' && ch<='f') {
		chval = (ch - 'a' + 10);
	} else if(ch>='A' && ch<='F') {
		chval = (ch - 'A' + 10);
	}
	return chval;
}

bool isLetter(int ch){
	return ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'));
}

bool isWhitespace(int ch){
	return (ch==' ' || ch=='\n' || ch=='\t' || ch=='\r');
}

//------------------------
// from: http://www.scheme.com/tspl2d/grammar.html --> identifier
// <letter> | ! | $ | % | & | * | / | : | < | = | > | ? | ~ | _ | ^
// + | - | . | # --> speciel forms
//------------------------
bool isSymbolInitialChar(int ch){
	return (ch=='!' || ch=='$' || ch=='%' || ch=='&' || ch=='*' || ch=='/' ||
			ch==':' || ch=='<' || ch=='=' || ch=='>' || ch=='?' || ch=='~' ||
			ch=='^' || ch=='_' || ch=='.' || ch=='+' || ch=='-' ||
			isLetter(ch));
}

//------------------------
// from: http://www.scheme.com/tspl2d/grammar.html --> identifier
// <initial> | <digit>
//------------------------
bool isSymbolSubsequentChar(int ch){
	return (isSymbolInitialChar(ch) || isDecimalDigit(ch));
}

// #### get/push char ##########################################################################################

void pushCharBack(int ch){
	if(charStackPtr >= CHARSTACK_SIZE){
		ybThrowError(-1, "reader: charStack overflow");
	}
	else{
		charStack[++charStackPtr] = ch;
	}
}

int getNextRelevantChar(FILE* inputStream){
	int ch;

	if(charStackPtr>=0){
		ch = charStack[charStackPtr--];
	}
	else{
		ch = getc(inputStream);
	}
	//Kommentare ignorieren: geht nur, solange nur ein Zeichen back gepushed wird
	//todo problem when in string. e.g. "str;ng" --> should be read as string
	if(ch==';') {
		while(ch != '\n') {
			ch = getc(inputStream);
		}
		// read next relevant char
		ch = getc(inputStream);
	}

//printf("#### relevant: %c\n", isWhitespace(ch)?'_':ch);
	return ch;
}

int getNextRelevantCharWithoutWhitespaces(FILE* inputStream){
	int ch;
	//ignore whitespace
	do{
		ch=getNextRelevantChar(inputStream);
	}while(isWhitespace(ch));
	return ch;
}

// #### read functions ##########################################################################################


//------------------------
// read list
//------------------------
OBJ ybReadList(FILE* inputStream){
	int ch = getNextRelevantCharWithoutWhitespaces(inputStream);
	if(ch==')'){
		return globalNil;
	}

	pushCharBack(ch);

	OBJ first = ybRead(inputStream);
	OBJ rest = ybReadList(inputStream);

	return newYbCons(first, rest);
}


typedef bool (*isDigitPtr)(int);

int ybReadNumberSign(int *ch, FILE* inputStream, isDigitPtr isDigitFct) {
	int sign = 1;

	if(*ch == '-') {
		sign = -1;
		*ch = getNextRelevantChar(inputStream);
		// this is not a number, maybe a symbol
		if(!isDigitFct(*ch)) {
			pushCharBack(*ch);
			pushCharBack('-');
			return 0;
		}
	} else if(*ch == '+') {
		*ch = getNextRelevantChar(inputStream);
		// this is not a number, maybe a symbol
		if(!isDigitFct(*ch)) {
			pushCharBack(*ch);
			pushCharBack('+');
			return 0;
		}
	}

	// this is not a number, maybe a symbol
	if(!isDigitFct(*ch)) {
		pushCharBack(*ch);
		return 0;
	}

	return sign;
}

OBJ ybReadNumberBinary(FILE* inputStream) {
	int ch = getNextRelevantChar(inputStream);
	int sign = ybReadNumberSign(&ch, inputStream, &isBinaryDigit);
	if(!sign) {
		// not a number
		return globalNil;
	}

	long value = ch - '0';
	while(isBinaryDigit(ch = getNextRelevantChar(inputStream))) {
		// assuming compiler will optimize *2 by shift operation
		value = value * 2 + (ch - '0');
	};

	// read one too much
	pushCharBack(ch);
	return newYbIntNumber(value*sign);
}
OBJ ybReadNumberOctal(FILE* inputStream) {
	int ch = getNextRelevantChar(inputStream);
	int sign = ybReadNumberSign(&ch, inputStream, &isOctalDigit);
	if(!sign) {
		// not a number
		return globalNil;
	}

	long value = ch - '0';
	while(isOctalDigit(ch = getNextRelevantChar(inputStream))) {
		// assuming compiler will optimize *8 by shift operation
		value = value * 8 + (ch - '0');
	};

	// read one too much
	pushCharBack(ch);
	return newYbIntNumber(value*sign);
}

OBJ ybReadNumberDecimal(FILE* inputStream) {
	int ch = getNextRelevantChar(inputStream);
	int sign = ybReadNumberSign(&ch, inputStream, &isDecimalDigit);
	if(!sign) {
		// not a number
		return globalNil;
	}

	long value = ch - '0';
	while(isDecimalDigit(ch = getNextRelevantChar(inputStream))) {
		value = value * 10 + (ch - '0');
	};

	// read one too much
	pushCharBack(ch);
	return newYbIntNumber(value*sign);
}

OBJ ybReadNumberHexadecimal(FILE* inputStream) {
	int ch = getNextRelevantChar(inputStream);
	int sign = ybReadNumberSign(&ch, inputStream, &isHexadecimalDigit);
	if(!sign) {
		// not a number
		return globalNil;
	}

	long value = convertHexDigit(ch);
	while(isHexadecimalDigit(ch = getNextRelevantChar(inputStream))) {
		// assuming compiler will optimize *16 by shift operation
		value = value * 16 + convertHexDigit(ch);
	}

	// read one too much
	pushCharBack(ch);
	return newYbIntNumber(value*sign);
}

/******************
 * read string
 * todo unbegrenzte array-länge fürs string:
 * initialisieren mit malloc
 * mit realloc den speicher in der schleife erweitern
 * am ende speicher freigeben mit free
 * todo: strings mit ; drinnen?
 ******************/
//------------------------
// read string
//------------------------
OBJ ybReadString(FILE* inputStream){
	//todo unbegrenzte array-länge fürs string:
	//initialisieren mit malloc
	//mit realloc den speicher in der schleife erweitern
	//am ende speicher freigeben mit free
	OBJ obj;
	//TODO: dynamisch verwalten
	char val[100];
	char *p = val;
	int ch=getNextRelevantChar(inputStream);
	while(ch != '"'){
		*p=ch;
		p++;
		ch=getNextRelevantChar(inputStream);
	}
	*p='\0'; //damit klar ist wann der string zuende ist
	obj = newYbString(val);
	//free() mit dem angelegten Speicher als parameter
	return obj;
}

//------------------------
// read symbol
//------------------------
OBJ ybReadSymbol(FILE* inputStream){
	OBJ obj;
	char val[100];
	//TODO: dynamisch verwalten
	char *p = val;

	//check first char
	int ch = getNextRelevantCharWithoutWhitespaces(inputStream);
	if(!isSymbolInitialChar(ch)){
		return newYbError("syntax error: not a valid symbol, starting with: %c", ch);
	}
	*p=ch;
	p++;

	//check the following chars
	ch=getNextRelevantChar(inputStream);
	while(isSymbolSubsequentChar(ch)){
		*p=ch;
		p++;
		ch=getNextRelevantChar(inputStream);
	}
	//end of string
	*p='\0';
	pushCharBack(ch);

	if((val[0]=='+' || val[0]=='-') && val[1]!='\0'){
		return newYbError("syntax error: not a valid symbol: %s", val);

	}
	if(strcmp(val, "nil")==0){
		return globalNil;
	}

	obj = addToSymbolTable(val);
	return obj;
}


// #### read ##########################################################################################

//------------------------
// main read
// called from repl
//------------------------
OBJ ybRead(FILE* inputStream){
	int ch = getNextRelevantCharWithoutWhitespaces(inputStream);

	//list
	if(ch=='('){
		return ybReadList(inputStream);
	}

	//string
	if(ch=='"'){
		return ybReadString(inputStream);
	}

	// get number system
	if(ch == '#') {
		ch = getNextRelevantChar(inputStream);
		switch(ch) {
		case 'b':
			return ybReadNumberBinary(inputStream);
		case 'o':
			return ybReadNumberOctal(inputStream);
		case 'd':
			return ybReadNumberDecimal(inputStream);
		case 'x':
			return ybReadNumberHexadecimal(inputStream);
		case 't':
			return globalTrue;
		case 'f':
			return globalFalse;
		default:
			// no symbol/other type may start with a #
			return newYbError("syntax error: invalid token: #%c", ch);
		}
	}
	pushCharBack(ch);
	
	// default number system is decimal
	OBJ obj = ybReadNumberDecimal(inputStream);
	// if not a number, this must be a symbol
	if(obj == globalNil) {
		return ybReadSymbol(inputStream);
	}
	return obj;
}
