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
	//functions
	envAdd(addToSymbolTable("+"), newYbBuiltinFunction("+", &builtinPlus));
	envAdd(addToSymbolTable("-"), newYbBuiltinFunction("-", &builtinMinus));
	envAdd(addToSymbolTable("*"), newYbBuiltinFunction("*", &builtinMultiplication));
	envAdd(addToSymbolTable("/"), newYbBuiltinFunction("/", &builtinDivision));
	//syntax
	envAdd(addToSymbolTable("define"), newYbBuiltinSyntax("define", &builtinDefine));
	//quote
	//if
	//lambda
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
	//je nach dem was das erste element ist, muss der rest der liste anders behandelt werden
	//todo evtl anders schreiben - dass die schleife ganz um alles geht, und da drinnen dann die abfrage erfolgt welcher type das ist
	switch(evalFirst->u.any.type){
	case T_BUILTIN_SYNTAX:
		while(rest->u.any.type!=T_NIL){
			//in dem Fall dürfen die Argumente nicht evaluiert werden
			pushToEvalStack(rest->u.cons.first);
			countArgs++;
			rest = rest->u.cons.rest;
		}
		//syntax aufrufen
		//ybEnvironment env = newYbEnvironment();
		//return (*evalFirst->u.syntax.impl)(env, rest);
		return NULL;
		//Gittingers Version:
					//return evaluatedFunctionSlot->u.builtinSyntax.theCode(env, argList);
					//wobei argList einfach rest wäre (bei mir)
		break;
	case T_BUILTIN_FUNCTION:
		while(rest->u.any.type!=T_NIL){
			pushToEvalStack(ybEval(rest->u.cons.first));
			countArgs++;
			rest = rest->u.cons.rest;
		}
		//die funktion in der funktionslot ausführen, diese holt sich dann die argumente vom stack.
		return (*evalFirst->u.fctBuiltin.impl)(countArgs);


		break;
	case T_USER_FUNCTION:
	default:
		//todo was passiert hier genau? welche Fälle könnte es noch geben?
		//frage wenn das erste symbol in der Liste eine variable ist und kein "ausführbares" symbol dann gibt das n fehler, oder?
		return globalNil;
		break;
	}
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
