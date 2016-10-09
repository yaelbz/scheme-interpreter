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
	envAdd(NULL, addToSymbolTable("+"), newYbBuiltinFunction("+", &builtinPlus));
	envAdd(NULL, addToSymbolTable("-"), newYbBuiltinFunction("-", &builtinMinus));
	envAdd(NULL, addToSymbolTable("*"), newYbBuiltinFunction("*", &builtinMultiplication));
	envAdd(NULL, addToSymbolTable("/"), newYbBuiltinFunction("/", &builtinDivision));
	//syntax
	//envAdd(NULL, addToSymbolTable("define"), newYbBuiltinSyntax("define", &builtinDefine));
	envAdd(NULL, addToSymbolTable("if"),     newYbBuiltinSyntax("if",     &builtinIf));
	//quote
	//lambda
}

void initEvaluator(){
	initEvalStack();
	initEnv();
	initBuiltins(); //Environment and symbolTable must be initialized first!
}

//--------------- eval functions ----------------------//

OBJ ybEvalSymbol(OBJ env, OBJ obj){
	//printf("eval --- ybEvalSymbol:\n");
	OBJ evalObj = envGet(env, obj);
	//wenn objekt gefunden
	if(evalObj) return evalObj;
	//else error
	return newYbError("eval: symbol could not be evaluated");
}

OBJ ybEvalCons(OBJ env, OBJ obj){
	//printf("eval --- ybEvalCons:\n");
	//first element als funktionslot abspeichern
	OBJ evalFirst = ybEval(env, obj->u.cons.first);

	//rest in ner schleife durchgehen
	OBJ rest = obj->u.cons.rest;
	int countArgs = 0;
	switch(evalFirst->u.any.type){
	case T_BUILTIN_SYNTAX:
		//syntax aufrufen
		//ybEnvironment env = newYbEnvironment();
		return (*evalFirst->u.syntax.impl)(env, rest);
	case T_BUILTIN_FUNCTION:
		while(rest->u.any.type!=T_NIL){
			pushToEvalStack(ybEval(env, rest->u.cons.first));
			countArgs++;
			rest = rest->u.cons.rest;
		}
		//die funktion in der funktionslot ausführen, diese holt sich dann die argumente vom stack.
		return (*evalFirst->u.fctBuiltin.impl)(countArgs);
	case T_USER_FUNCTION:
	default:
		//was passiert hier genau? welche Fälle könnte es noch geben?
		//frage wenn das erste symbol in der Liste eine variable ist und kein "ausführbares" symbol dann gibt das n fehler, oder?
		return globalNil;
		break;
	}
}


//--------------- eval ----------------------//


OBJ ybEval(OBJ env, OBJ obj){
	switch(TYPE(obj)){
	case T_SYMBOL:
		return ybEvalSymbol(env, obj);
	case T_CONS:
		return ybEvalCons(env, obj);
	default:
		return obj;
	}
}
