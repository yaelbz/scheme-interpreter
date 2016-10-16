/*
 * global.c
 *
 *  Created on: 02.10.2016
 *      Author: yael
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "global.h"
#include "symbolTable.h"

// #### init #######################################################################################

//------------------------
// init global nil, void, true, false
//------------------------
void initGlobals(){
	globalNil = (OBJ)(malloc( sizeof(struct ybAny)));
	TYPE(globalNil) = T_NIL;

	globalVoid = (OBJ)(malloc( sizeof(struct ybAny)));
	TYPE(globalVoid) = T_VOID;

	globalTrue = (OBJ)(malloc( sizeof(struct ybAny)));
	TYPE(globalTrue) = T_TRUE;

	globalFalse = (OBJ)(malloc( sizeof(struct ybAny)));
	TYPE(globalFalse) = T_FALSE;

	initSymbolTable();
	globalDefine = addToSymbolTable("define");
}

// #### constructor #######################################################################################

//------------------------
// new error
//------------------------
//TODO: variable argument list. Wie wird da der Speicher verwaltet?
OBJ newYbError(const char *format, ...) {
	struct ybError *obj;

	va_list args;
	va_start(args, format);
	//max error message length = 255
	char msg[255];
	vsprintf(msg, format, args);
	va_end(args);

	obj = (struct ybError *)(malloc( sizeof(struct ybError)));
	obj->type = T_ERROR;
	obj->message = (char *)(malloc( strlen(msg)));
	strcpy(obj->message, msg);
	return (OBJ)obj;
}

//------------------------
// new int
//------------------------
OBJ newYbIntNumber(long iVal) {
	struct ybNumber *obj;

	obj = (struct ybNumber *)(malloc( sizeof(struct ybNumber)));
	obj->type = T_NUMBER;
	obj->value.i = iVal;
	obj->isInteger = true;
	return (OBJ)obj;
}

//------------------------
// new float
//------------------------
OBJ newYbFloatNumber(double iVal) {
	struct ybNumber *obj;

	obj = (struct ybNumber *)(malloc( sizeof(struct ybNumber)));
	obj->type = T_NUMBER;
	obj->value.f = iVal;
	obj->isInteger = false;
	return (OBJ)obj;
}

//------------------------
// new string
//------------------------
OBJ newYbString(char *val){
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

//------------------------
// new symbol
//------------------------
OBJ newYbSymbol(char *val){
	struct ybSymbol *obj;
	//siehe anmerkung in newString()
	obj = (struct ybSymbol *)(malloc( sizeof(struct ybSymbol)));
	obj->type = T_SYMBOL;
	obj->name = (char *)(malloc( strlen(val)));
	strcpy(obj->name, val);
	return (OBJ)obj;
}
/*
//------------------------
// new bool
//------------------------
OBJ newYbBool(bool val){
	struct ybBool *obj;
	obj = (struct ybBool *)(malloc( sizeof(struct ybBool)));
	obj->type = T_BOOL;
	obj->value = val;
	return (OBJ)obj;
}
*/

//------------------------
// new cons
//------------------------
OBJ newYbCons(OBJ car, OBJ cdr){
	struct ybCons *obj;
	obj = (struct ybCons *)(malloc(sizeof(struct ybCons)));
	obj->type=T_CONS;
	obj->first = car;
	obj->rest = cdr;
	return (OBJ)obj;
}

//------------------------
// new builtin function
//------------------------
OBJ newYbBuiltinFunction(char *name, ybFctPtr implementation){
	struct ybBuiltinFunction *obj;
	obj = (struct ybBuiltinFunction *)(malloc(sizeof(struct ybBuiltinFunction)));
	obj->type=T_BUILTIN_FUNCTION;
	obj->name = name;
	obj->impl = implementation;
	return (OBJ)obj;
}

//------------------------
// new user defined function
//------------------------
OBJ newYbUserDefinedFunction(char *name, OBJ env, OBJ parameterList, OBJ bodyList, int numDefs){
	struct ybUserFunction *obj;
	obj = (struct ybUserFunction *)(malloc(sizeof(struct ybUserFunction)));
	obj->type=T_USER_FUNCTION;
	obj->name = name;
	obj->env = env;
	obj->numDefs = numDefs;
	obj->parameterList = parameterList;
	obj->numParameter = 0;
	while(parameterList != globalNil){
		obj->numParameter++;
		parameterList=REST(parameterList);
	}
	obj->bodyList = bodyList;
	return (OBJ)obj;
}

//------------------------
// new builtin syntax
//------------------------
OBJ newYbBuiltinSyntax(char *name, ybSyntaxPtr implementation){
	struct ybBuiltinSyntax *obj;
	obj = (struct ybBuiltinSyntax *)(malloc(sizeof(struct ybBuiltinSyntax)));
	obj->type=T_BUILTIN_SYNTAX;
	obj->name = name;
	obj->impl = implementation;
	return (OBJ)obj;
}


//------------------------
// new environment
//------------------------
OBJ newYbEnvironment(int envSize, OBJ parentEnv){
	ybEnvironment *obj;
	obj = (ybEnvironment *)(malloc(sizeof(ybEnvironment)+sizeof(keyValuePair)*envSize));
	obj->type=T_ENVIRONMENT;
	obj->size=envSize;
	obj->entryCount=0;
	obj->parentEnv = parentEnv;
	memset( (void*)&(obj->entries), 0, (sizeof(keyValuePair) * envSize));
	return (OBJ)obj;
}

// #### free object #######################################################################################

void freeObject(OBJ obj){
	//todo implement freeObject
}

// #### error #######################################################################################

//------------------------
// throw error
//------------------------
//TODO: declare enum with errors in global header
void ybThrowError(int ybErrNum, const char *format, ...) {
	va_list args;
	va_start(args, format);

	fprintf(stderr, "#### ERROR 0x%08x:\n", ybErrNum);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n####\n\n");
	fflush(stderr);

    va_end(args);

    //TODO: handle error, return to main execution
    if(ybErrNum < 0) {
    	exit(ybErrNum);
    }
}
