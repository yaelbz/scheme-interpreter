/*
 * printer.c
 *
 *  Created on: 29.09.2016
 *      Author: yael
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

/*
 * so solls aussehen:
 *


cons
   +-int(1)
   +-cons
      +-int(2)
      +-cons
         +-cons
         |  +-int(3)
         |  +-cons
         |     +-int(4)
         |     +-NULL  !!remove me!!
         +-cons
            +-int(5)
            +-cons
               +-int(6)
               +-NULL  !!remove me!!
 */







/*
// under construction
void ybPrintIndent(char* prefix, const char* prefix2, OBJ obj) {

	if(obj){
		//TODO der zugang zu den Objekt-tags könnte über ein Makro kürzer werden.
		switch (obj->u.any.type) {
		case T_NIL:
			printf("%s%snil\n", prefix, prefix2);
			break;
		case T_INT:
			printf("%s%sint(%d)\n", prefix, prefix2, obj->u.integer.value);
			break;
		case T_STRING:
			printf("%s%sstring(%s)\n", prefix, prefix2, obj->u.string.string);
			break;
		case T_SYMBOL:
			printf("%s%ssymbol(%s)\n", prefix, prefix2, obj->u.symbol.name);
			break;
		case T_CONS: {
			printf("%s%scons\n", prefix, prefix2);
			// Vorausschauen für Baumstruktur
			char* newPrefix1 = (char*)malloc(sizeof(prefix)+sizeof(prefix2)+6);
			char* newPrefixR = (char*)malloc(sizeof(prefix)+sizeof(prefix2)+6);
			//strcat(newPrefix, prefix2);
			if(obj->u.cons.first == NULL || obj->u.cons.first->u.any.type==T_CONS){
				strcat(newPrefix1, prefix);
				strcat(newPrefix1, "|  ");
				ybPrintIndent(newPrefix1,    "+-", obj->u.cons.first);
				strcat(newPrefixR, prefix);
				strcat(newPrefixR, "   ");
				ybPrintIndent(newPrefixR, "+-", obj->u.cons.rest);
			} else {
				strcat(newPrefix1, prefix);
				strcat(newPrefix1, "   ");
				ybPrintIndent(newPrefix1, "+-", obj->u.cons.first);
				ybPrintIndent(newPrefix1, "+-", obj->u.cons.rest);
			}
			free(newPrefix1);
			free(newPrefixR);
			//ybPrintIndent(indentCount+3, "+-", obj->u.cons.first);
			//ybPrintIndent(indentCount+3, "+-", obj->u.cons.rest);
			//  (1 2 (3 4) 5 6)
			break;
		}
		default:
			printf("%s%s<unknown type>\n", prefix, prefix2);
			break;
		}
	}
	else{
		printf("%s%sNULL  !!remove me!!\n", prefix, prefix2);
	}

	fflush(stdout);

	// Dynamisch angelegten Speicher wieder freigeben
	//free(indentString);

}

void ybPrint(OBJ obj) {
	ybPrintIndent("", "", obj);
}

/*/



void ybPrintIndent(int indentCount, char* prefix, OBJ obj) {

	// String anlegen, der indentCount spaces enthält
	char* indentString = malloc(indentCount*sizeof(char)+1);
	memset(indentString, ' ', indentCount);
	indentString[indentCount] = '\0';

	if(obj){
		//TODO der zugang zu den Objekt-tags könnte über ein Makro kürzer werden.
		switch (obj->u.any.type) {
		case T_NIL:
			printf("%s%snil\n", indentString, prefix);
			break;
		case T_INT:
			printf("%s%sint(%d)\n", indentString, prefix, obj->u.integer.value);
			break;
		case T_STRING:
			printf("%s%sstring(%s)\n", indentString, prefix, obj->u.string.string);
			break;
		case T_SYMBOL:
			printf("%s%ssymbol(%s)\n", indentString, prefix, obj->u.symbol.name);
			break;
		case T_CONS:
			printf("%s%scons\n", indentString, prefix);
			ybPrintIndent(indentCount+3, "+-", obj->u.cons.first);
			ybPrintIndent(indentCount+3, "+-", obj->u.cons.rest);
			break;
		default:
			printf("%s%s<unknown type>\n", indentString, prefix);
			break;
		}
	}
	else{
		printf("%s%sNULL  !!remove me!!\n", indentString, prefix);
	}

	fflush(stdout);

	// Dynamisch angelegten Speicher wieder freigeben
	free(indentString);
}


void ybPrint(OBJ obj) {
	ybPrintIndent(0, "", obj);
}
// */



