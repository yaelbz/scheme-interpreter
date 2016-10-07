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








void ybPrintIndent(int indentCount, char* prefix, OBJ obj) {

	// String anlegen, der indentCount spaces enthält
	char* indentString = malloc(indentCount*sizeof(char)+1);
	memset(indentString, ' ', indentCount);
	indentString[indentCount] = '\0';

	if(obj){
		//TODO der zugang zu den Objekt-tags könnte über ein Makro kürzer werden.
		switch (obj->u.any.type) {
		case T_ERROR:
			printf("%s%serror(%s)\n", indentString, prefix, obj->u.error.message);
			ybThrowError(1, obj->u.error.message);
			//fprintf(stderr, "%s\n", obj->u.error.message);
			break;
		case T_NIL:
			printf("%s%snil\n", indentString, prefix);
			break;
		case T_INT:
			printf("%s%sint(%ld)\n", indentString, prefix, obj->u.integer.value);
			break;
		case T_NUMBER:
			if(obj->u.number.isInteger) {
				printf("%s%sint(%ld)\n", indentString, prefix, obj->u.number.value.i);
			} else {
				printf("%s%sint(%f)\n", indentString, prefix, obj->u.number.value.f);
			}
			break;
		case T_STRING:
			printf("%s%sstring(%s)\n", indentString, prefix, obj->u.string.string);
			break;
		case T_SYMBOL:
			printf("%s%ssymbol(%s)\n", indentString, prefix, obj->u.symbol.name);
			break;
		case T_BUILTIN_FUNCTION:
			printf("%s%sbuiltin(%s,0x%08x)\n", indentString, prefix, obj->u.fctBuiltin.name, (int)obj->u.fctBuiltin.impl);
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

