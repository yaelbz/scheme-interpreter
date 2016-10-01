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

OBJ ybRead(FILE*);


static int pushedChar = -1;

bool isDigit(int ch){
	return (ch>='0' && ch<='9');
}

bool isLetter(int ch){
	return ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'));
}

bool isWhitespace(int ch){
	return (ch==' ' || ch=='\n' || ch=='\t' || ch=='\r');
}

bool isSymboInitialChar(int ch){
	//aus: http://www.scheme.com/tspl2d/grammar.html --> identifier
	//<letter> | ! | $ | % | & | * | / | : | < | = | > | ? | ~ | _ | ^
	return (ch=='!' || ch=='$' || ch=='%' || ch=='&' || ch=='*' || ch=='/' ||
			ch==':' || ch=='<' || ch=='=' || ch=='>' || ch=='?' || ch=='~' ||
			ch=='^' || ch=='_' || isLetter(ch));
}

bool isSubsequentChar(int ch){
	//aus: http://www.scheme.com/tspl2d/grammar.html --> identifier
	//<initial> | <digit> | . | + | -
	return (isSymboInitialChar(ch) || isDigit(ch) || ch=='.' || ch=='+' || ch=='-');
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
		printf("#### Error: pushedChar not empty\n");
		exit(-1);
	}
	else{
		pushedChar = ch;
	}
}

//--------------- object constructor - evtl auslagern in datei ----------------------//
//TODO Objekte anlegen (malloc)

/******************
 * von gittinger übernommen (er hats in memory.c)
 * habs etwas an meinen code angepasst
 * ?? ist es sinnvoll ein memory.c zu haben? oder lieber irgendwie anders?
 ******************/
OBJ newInteger(long iVal) {
	struct ybInt *obj;

	obj = (struct ybInt *)(malloc( sizeof(struct ybInt)));
	obj->type = T_INT;
	obj->value = iVal;
	return (OBJ)obj;
}

OBJ newString(char *val){
	struct ybString *obj;
	/*/*
	 * NewString sollte aber trotzdem strcpy verwenden
	 * Um den wert zu kopieren (der übergeben wird)
	 * Wenn in [] nichts drin steht, ist es dasselbe wie char*,
	 * dann ist kein speicher reserviert und muss erst mit malloc angelegt werden
	 */
	obj = (struct ybString *)(malloc( sizeof(struct ybString)));
	obj->type = T_STRING;
	obj->string = (char *)(malloc( strlen(val)));
	strcpy(obj->string, val);
	return (OBJ)obj;
}

OBJ newSymbol(char *val){
	struct ybSymbol *obj;
	//siehe anmerkung in newString()
	obj = (struct ybSymbol *)(malloc( sizeof(struct ybSymbol)));
	obj->type = T_SYMBOL;
	obj->name = (char *)(malloc( strlen(val)));
	strcpy(obj->name, val);
	return (OBJ)obj;
}

OBJ newCons(OBJ car, OBJ cdr){
	struct ybCons *obj;
	obj = (struct ybCons *)(malloc(sizeof(struct ybCons)));
	obj->type=T_CONS;
	obj->first = car;
	obj->rest = cdr;
	return (OBJ)obj;
}

//--------------- read functions ----------------------//

/******************
 * read list
 *
 ******************/
OBJ ybReadList(FILE* inputStream){
	int ch = getNextRelevantCharWithoutWhitespaces(inputStream);
	if(ch==')'){
		return NULL; //abbruch. besser nicht NULL nehmen, sondern eigenes Zeichen das das Ende der Liste symbolisiert. Damit der Eval das Ende findet
	}

	pushCharBack(ch);

	OBJ first = ybRead(inputStream);
	OBJ rest = ybReadList(inputStream);

	return newCons(first, rest);
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
		return NULL; //abbruch TODO weiß nicht wie genau das laufen soll (auch bei der anderen fkt)
	}
	if(isWhitespace(ch)){
		//kein unreadChar
		return NULL;
	}*/

	//gittinger:
		val = firstDigit - '0';
		int ch;
		while ( isDigit( ch = getNextRelevantChar(inputStream))) {
			val = val * 10 + (ch - '0');
		}
		pushCharBack(ch);

		obj = newInteger(val);

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
	obj = newString(val);
	//free() mit dem angelegten Speicher als parameter
	return obj;
}

/******************
 * read symbol
 ******************/
OBJ ybReadSymbol(FILE* inputStream, int firstCh){
	OBJ obj;
	char val[100];
	char *p = val;

	//check first char
	if(!isSymboInitialChar(firstCh)){
		printf("#### syntax error: not valid symbol");
		//todo was soll hier nach dem Error passieren? Soll das Programm abbrechen oder irgnedwie weiterlaufen?
	}
	*p=firstCh;
	p++;

	//check if not a single period
	//evtl erst prüfen wenn das ganze symbol eingelesen wurde
	int ch=getNextRelevantChar(inputStream);

//printf("#### ybReadSymbol: %c\n", isWhitespace(firstCh)?'_':firstCh); fflush(stdout);

	while(isSubsequentChar(ch)){
		*p=ch;
		p++;
		ch=getNextRelevantChar(inputStream);
	}
	*p='\0'; //damit klar ist wann der string zuende ist
	pushCharBack(ch);

	obj = newSymbol(val);
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

	//Zahl
	if(isDigit(ch)){
		return ybReadInt(inputStream, ch);
	}

	//String
	if(ch=='"'){
		return ybReadString(inputStream);
	}

	//"else" -> symbol
	return ybReadSymbol(inputStream, ch);

}
