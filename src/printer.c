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
         |     +-nil
         +-cons
            +-int(5)
            +-cons
               +-int(6)
               +-nil
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
			printf("## ERROR ## %s", obj->u.error.message);
			break;
		case T_NIL:
			//printf("'()\n");
			printf("nil");
			break;
		case T_VOID:
			//do nothing
			break;
		case T_TRUE:
			printf("#t");
			break;
		case T_FALSE:
			printf("#f");
			break;
		case T_NUMBER:
			if(obj->u.number.isInteger) {
				printf("%ld", obj->u.number.value.i);
			} else {
				printf("%f", obj->u.number.value.f);
			}
			break;
		case T_STRING:
			printf("\"%s\"", obj->u.string.string);
			break;
		case T_SYMBOL:
			printf("'%s", obj->u.symbol.name);
			break;
		case T_BUILTIN_FUNCTION:
			printf("#<procedure:%s>", obj->u.builtinFct.name);
			break;
		case T_USER_FUNCTION:
			printf("#<procedure:%s>", obj->u.userFct.name);
			break;
		case T_CONS:
			printf("(");
			ybPrintRacketStyle(FIRST(obj));
			while(TYPE(REST(obj)) == T_CONS) {
				printf(" ");
				obj = REST(obj);
				ybPrintRacketStyle(FIRST(obj));
			}
			if(REST(obj)!=globalNil) {
				printf(" . ");
				ybPrintRacketStyle(REST(obj));
			}
			printf(")");
			break;
		default:
			printf("this object cant be printed. type: %d", TYPE(obj));
			break;
			}
	}
	else {

	}

}


void ybPrint(OBJ obj) {
	//ybPrintIndent(0, "", obj);
	ybPrintRacketStyle(obj);
	printf("\n");
	fflush(stdout);
}

