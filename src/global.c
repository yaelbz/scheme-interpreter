/*
 * global.c
 *
 *  Created on: 02.10.2016
 *      Author: yael
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "global.h"


//--------------- object constructor - evtl auslagern in datei ----------------------//
/******************
 * von gittinger übernommen (er hats in memory.c)
 * habs etwas an meinen code angepasst
 * ?? ist es sinnvoll ein memory.c zu haben? oder lieber irgendwie anders?
 ******************/

OBJ newYbNil() {
	struct ybAny *obj;

	obj = (struct ybAny *)(malloc( sizeof(struct ybAny)));
	obj->type = T_NIL;
	return (OBJ)obj;
}

//TODO: variable argument list. Wie wird da der Speicher verwaltet?
OBJ newYbError(char *msg) {
	struct ybError *obj;

	obj = (struct ybError *)(malloc( sizeof(struct ybError)));
	obj->type = T_ERROR;
	obj->message = (char *)(malloc( strlen(msg)));
	strcpy(obj->message, msg);
	return (OBJ)obj;
}

OBJ newYbInteger(long iVal) {
	struct ybInt *obj;

	obj = (struct ybInt *)(malloc( sizeof(struct ybInt)));
	obj->type = T_INT;
	obj->value = iVal;
	return (OBJ)obj;
}

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

OBJ newYbSymbol(char *val){
	struct ybSymbol *obj;
	//siehe anmerkung in newString()
	obj = (struct ybSymbol *)(malloc( sizeof(struct ybSymbol)));
	obj->type = T_SYMBOL;
	obj->name = (char *)(malloc( strlen(val)));
	strcpy(obj->name, val);
	return (OBJ)obj;
}

OBJ newYbCons(OBJ car, OBJ cdr){
	struct ybCons *obj;
	obj = (struct ybCons *)(malloc(sizeof(struct ybCons)));
	obj->type=T_CONS;
	obj->first = car;
	obj->rest = cdr;
	return (OBJ)obj;
}

OBJ newYbFctBuiltin(char *name, ybFctPtr implementation){
	struct ybFctBuiltin *obj;
	obj = (struct ybFctBuiltin *)(malloc(sizeof(struct ybFctBuiltin)));
	obj->type=T_FCT_BUILTIN;
	obj->name = name;
	obj->impl = implementation;
	return (OBJ)obj;
}


//--------------- Error ----------------------//

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
