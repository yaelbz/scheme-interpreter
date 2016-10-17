/*
 * printer.c
 * repl --> printer
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
		switch (obj->u.any.type) {
		case T_ERROR:
			printf("%s%serror(%s)\n", indentString, prefix, obj->u.error.message);
			ybThrowError(1, obj->u.error.message);
			//fprintf(stderr, "%s\n", obj->u.error.message);
			break;
		case T_NIL:
			printf("%s%snil\n", indentString, prefix);
			break;
		case T_VOID:
			//do nothing
			break;
		case T_TRUE:
			printf("%s%strue\n", indentString, prefix);
			break;
		case T_FALSE:
			printf("%s%sfalse\n", indentString, prefix);
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
			printf("%s%sbuiltin(%s,0x%08lx)\n", indentString, prefix, obj->u.builtinFct.name, (long)obj->u.builtinFct.impl);
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

void ybPrintRacketStyle(OBJ obj){
	if(obj){
		switch (obj->u.any.type) {
		case T_ERROR:
			printf("## ERROR ## %s\n", obj->u.error.message);
			break;
		case T_NIL:
			printf("'()\n");
			break;
		case T_VOID:
			//do nothing
			break;
		case T_TRUE:
			printf("#t\n");
			break;
		case T_FALSE:
			printf("#f\n");
			break;
		case T_NUMBER:
			if(obj->u.number.isInteger) {
				printf("%ld\n", obj->u.number.value.i);
			} else {
				printf("%f\n", obj->u.number.value.f);
			}
			break;
		case T_STRING:
			printf("\"%s\"\n", obj->u.string.string);
			break;
		case T_SYMBOL:
			printf("TODO: return binding or error msg if not exists. %s\n", obj->u.symbol.name);
			break;
		case T_BUILTIN_FUNCTION:
			printf("#<procedure:%s>\n", obj->u.builtinFct.name);
			break;
		case T_USER_FUNCTION:
			printf("#<procedure>\n");
			break;
		case T_CONS:
			printf("cons: ");
			ybPrintRacketStyle(FIRST(obj));
			ybPrintRacketStyle(REST(obj));
			break;
		default:
			printf("this object cant be printed. type: %d\n", TYPE(obj));
			break;
			}
	}
	else {

	}

}


void ybPrint(OBJ obj) {
	//ybPrintIndent(0, "", obj);
	ybPrintRacketStyle(obj);
}

