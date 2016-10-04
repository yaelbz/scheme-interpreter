/*
 * reader.c
 * Reader-Implementierung
 *
 *
 *  Created on: 01.09.2016
 *      Author: yael
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "global.h"
#include "reader.h"
#include "symbolTable.h"


static int pushedChar = -1;

//--------------- helper functions ----------------------//

bool isDigit(int ch){
	return (ch>='0' && ch<='9');
}

bool isLetter(int ch){
	return ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'));
}

bool isWhitespace(int ch){
	return (ch==' ' || ch=='\n' || ch=='\t' || ch=='\r');
}

bool isSymbolInitialChar(int ch){
	//aus: http://www.scheme.com/tspl2d/grammar.html --> identifier
	//<letter> | ! | $ | % | & | * | / | : | < | = | > | ? | ~ | _ | ^
	// + | - | . --> prüfung auf Sonderfälle in ybReadSymbol
	return (ch=='!' || ch=='$' || ch=='%' || ch=='&' || ch=='*' || ch=='/' ||
			ch==':' || ch=='<' || ch=='=' || ch=='>' || ch=='?' || ch=='~' ||
			ch=='^' || ch=='_' || ch=='.' || ch=='+' || ch=='-' || isLetter(ch));
}

bool isSymbolSubsequentChar(int ch){
	//aus: http://www.scheme.com/tspl2d/grammar.html --> identifier
	//<initial> | <digit>
	return (isSymbolInitialChar(ch) || isDigit(ch));
}

//--------------- get/push char ----------------------//

int getNextRelevantChar(FILE* inputStream){
	int ch;

	if(pushedChar>=0){
		ch=pushedChar;
		pushedChar=-1;
	}
	else{
		ch=getc(inputStream);
	}
	//Kommentare ignorieren: geht nur, solange nur ein Zeichen back gepushed wird
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


void pushCharBack(int ch){
	if(pushedChar>=0){
		ybThrowError(-1, "pushedChar not empty");
	}
	else{
		pushedChar = ch;
	}
}

//--------------- read functions ----------------------//


/******************
 * read list
 *
 ******************/
OBJ ybReadList(FILE* inputStream){
	int ch = getNextRelevantCharWithoutWhitespaces(inputStream);
	if(ch==')'){
		return newYbNil();
	}

	pushCharBack(ch);

	OBJ first = ybRead(inputStream);
	OBJ rest = ybReadList(inputStream);

	return newYbCons(first, rest);
}


/******************
 * read int
 ******************/
OBJ ybReadInt(FILE* inputStream, int firstDigit){
	OBJ obj;
	long val; //gittinger

	/*//brauche ich im moment nicht. evtl wenn ichs doch rekursiv machen will
	int ch = getNextRelevantChar(inputStream);

	if(!isDigit(ch)){
		//evtl unreadChar() ??? Das zeichen wird ja später nochmal gebraucht
		//todo hier muss auch abgebrochen werden wenn ein leerzeichen gekommen ist (von getNextRelevantChar), es könnten ja zwei zahlen hintereinander kommen, zb 1 2, wenn da das leerzeichen ignoriert wird wirds ja ne 12
		return newYbNil();
	}
	if(isWhitespace(ch)){
		//kein unreadChar
		return newYbNil();
	}*/

	//gittinger:
		val = firstDigit - '0';
		int ch;
		while ( isDigit( ch = getNextRelevantChar(inputStream))) {
			val = val * 10 + (ch - '0');
		}
		pushCharBack(ch);

		obj = newYbInteger(val);

	return obj;
}

/******************
 * read string
 * todo unbegrenzte array-länge fürs string:
 * initialisieren mit malloc
 * mit realloc den speicher in der schleife erweitern
 * am ende speicher freigeben mit free
 ******************/
OBJ ybReadString(FILE* inputStream){
	OBJ obj;
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

/******************
 * read symbol
 ******************/
OBJ ybReadSymbol(FILE* inputStream){
	OBJ obj;
	char val[100];
	char *p = val;

	//check first char
	int ch = getNextRelevantChar(inputStream);
	if(!isSymbolInitialChar(ch)){
		return newYbError("syntax error: not a valid symbol");
		//ybThrowError(-1, "syntax error: not valid symbol");
	}
	*p=ch;
	p++;

//printf("#### ybReadSymbol: %c\n", isWhitespace(firstCh)?'_':firstCh); fflush(stdout);

	//check the following chars
	ch=getNextRelevantChar(inputStream);
	while(isSymbolSubsequentChar(ch)){
		*p=ch;
		p++;
		ch=getNextRelevantChar(inputStream);
	}
	*p='\0'; //damit klar ist wann der string zuende ist
	pushCharBack(ch);

	//sonderfälle prüfen: + - ...
	if((val[0]=='+' || val[0]=='-') && val[1]!='\0'){
		//fehlermeldung, da beginnt nämlich ein symbol mit + oder -
		return newYbError("syntax error: not a valid symbol");
		//ybThrowError(-1, "syntax error: not a symbol");

		//todo auch auf ... prüfen
	}

	obj = getOrAddFromSymbolTable(val);
	//free() mit dem angelegten Speicher als parameter
	return obj;
}


//--------------- Read ----------------------//

/******************
 * main read function
 * called from ybscheme
 ******************/
OBJ ybRead(FILE* inputStream){
	int ch = getNextRelevantCharWithoutWhitespaces(inputStream);

//printf("#### ybRead: %c\n", isWhitespace(ch)?'_':ch); fflush(stdout);

	//Liste
	if(ch=='('){
		return ybReadList(inputStream);
	}

	//String
	if(ch=='"'){
		return ybReadString(inputStream);
	}

	//todo implementieren:
	/*
	if(ch=='+' || ch=='-' || ch=='.'){
		int secondCh = getNextRelevantChar(inputStream);
		if(isDigit(secondCh)){
			pushCharBack(ch, secondCh); //beide Zeichen zurücklegen!!!
			return ybReadNumber(inputStream); //in ybReadInt umbenennen in ybReadNumber und dort die zurückgelegte Zeichen auslesen
		}
		else{
			pushCharBack(ch, secondCh); //beide Zeichen zurücklegen!!!
			return ybReadSymbol(inputStream); //dort die Zeichen wieder auslesen und weiterprüfen
		}
	}
	*/

	//Zahl
	if(isDigit(ch)){
		return ybReadInt(inputStream, ch);
	}

	//"else" -> symbol
	pushCharBack(ch);
	return ybReadSymbol(inputStream);

}
