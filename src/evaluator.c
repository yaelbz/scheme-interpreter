/*
 * evaluator.c
 *
 *  Created on: 02.10.2016
 *      Author: yael
 */

#include <stdio.h>
#include "global.h"
#include "evalStack.h"
#include "builtins.h"
#include "environment.h"
#include "evaluator.h"
#include "printer.h"
#include "symbolTable.h"

//--------------- init ----------------------//

/******************
 * init builtins
 *
 ******************/
void initBuiltins(){
	envAdd(addToSymbolTable("+"), newYbFctBuiltin("+", &builtinPlus));
	envAdd(addToSymbolTable("-"), newYbFctBuiltin("-", &builtinMinus));
	envAdd(addToSymbolTable("*"), newYbFctBuiltin("*", &builtinMultiplication));
	envAdd(addToSymbolTable("/"), newYbFctBuiltin("/", &builtinDivision));
}

void initEval(){
	initEvalStack();
	initSymbolTable();
	initEnv();
	initBuiltins(); //Environment and symbolTable must be initialized first!
}

//--------------- eval functions ----------------------//

OBJ ybEvalSymbol(OBJ obj){
	//printf("eval --- ybEvalSymbol:\n");
	OBJ evalObj = envGet(obj);
	//wenn objekt gefunden
	if(evalObj) return evalObj;
	//else error
	return newYbError("eval: symbol could not be evaluated");
}

OBJ ybEvalCons(OBJ obj){
	//printf("eval --- ybEvalCons:\n");
	//first element als funktionslot abspeichern
	OBJ evalFirst = ybEval(obj->u.cons.first);

	//rest in ner schleife durchgehen
	OBJ rest = obj->u.cons.rest;
	int countArgs = 0;
	while(rest->u.any.type!=T_NIL){
		pushToEvalStack(ybEval(rest->u.cons.first));
		countArgs++;
		rest = rest->u.cons.rest;
	}

	//die funktion in der funktionslot ausführen, diese holt sich dann die argumente vom stack.
	return (*evalFirst->u.fctBuiltin.impl)(countArgs);
}


//--------------- eval ----------------------//


OBJ ybEval(OBJ obj){
	//printf("eval --- ybEval:\n");
	//switch(obj->u.any.type){
	switch(TYPE(obj)){
	case T_ERROR:
	case T_NIL:
	case T_STRING:
	case T_INT:
		return obj;
	case T_SYMBOL:
		return ybEvalSymbol(obj);
	case T_CONS:
		return ybEvalCons(obj);
	default:
		//ybThrowError(-1, "eval: unknown type: 0x%x", obj->u.any.type);
		return newYbError("eval: unknown type");
	}
}
