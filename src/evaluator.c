/*
 * evaluator.c
 * repl --> evaluator
 *
 */

#include <stdio.h>
#include "global.h"
#include "evalStack.h"
#include "builtins.h"
#include "environment.h"
#include "evaluator.h"
#include "printer.h"
#include "symbolTable.h"

// #### init #######################################################################################

//------------------------
// init builtins
//------------------------
void initBuiltins(){
	//functions
	envAdd(NULL, addToSymbolTable("+"), newYbBuiltinFunction("+", &builtinPlus));
	envAdd(NULL, addToSymbolTable("-"), newYbBuiltinFunction("-", &builtinMinus));
	envAdd(NULL, addToSymbolTable("*"), newYbBuiltinFunction("*", &builtinMultiplication));
	envAdd(NULL, addToSymbolTable("/"), newYbBuiltinFunction("/", &builtinDivision));
	envAdd(NULL, addToSymbolTable("eq?"),newYbBuiltinFunction("eq?",	&builtinEqQ));
	envAdd(NULL, addToSymbolTable("="),	newYbBuiltinFunction("=",	&builtinEqualOperator));
	envAdd(NULL, addToSymbolTable("eqv?"),newYbBuiltinFunction("eqv?",&builtinEqvQ));
	envAdd(NULL, addToSymbolTable("not"),newYbBuiltinFunction("not",&builtinNot));

	//syntax
	//envAdd(NULL, addToSymbolTable("define"), newYbBuiltinSyntax("define", &builtinDefine));
	envAdd(NULL, addToSymbolTable("if"),	newYbBuiltinSyntax("if",	&builtinIf));


	//quote
	//lambda
}

//------------------------
// init evaluator
//------------------------
void initEvaluator(){
	initEvalStack();
	initEnv();
	initBuiltins(); //Environment and symbolTable must be initialized first!
}

// #### eval functions #######################################################################################

//------------------------
// symbol
//------------------------
OBJ ybEvalSymbol(OBJ env, OBJ obj){
	//printf("eval --- ybEvalSymbol:\n");
	OBJ evalObj = envGet(env, obj);
	//object found
	if(evalObj) return evalObj;
	//else error
	return newYbError("eval: symbol could not be evaluated");
}

//------------------------
// list
//------------------------
OBJ ybEvalCons(OBJ env, OBJ obj){
	//printf("eval --- ybEvalCons:\n");

	OBJ evalFirst = ybEval(env, FIRST(obj));

	OBJ rest = REST(obj);
	int countArgs = 0;
	switch(TYPE(evalFirst)){
	case T_BUILTIN_SYNTAX:
		return (*evalFirst->u.builtinSyntax.impl)(env, rest);
	case T_BUILTIN_FUNCTION:
		while(rest->u.any.type!=T_NIL){
			pushToEvalStack(ybEval(env, FIRST(rest)));
			countArgs++;
			rest = REST(rest);
		}
		return (*evalFirst->u.builtinFct.impl)(countArgs);
	case T_USER_FUNCTION:
		//create new env
		//create new o
	default:
		//was passiert hier genau? welche Fälle könnte es noch geben?
		//frage wenn das erste symbol in der Liste eine variable ist und kein "ausführbares" symbol dann gibt das n fehler, oder?
		return globalNil;
	}
	// never reached
	return newYbError("eval: should not be here");
}


// #### eval #######################################################################################

//------------------------
// main eval
// called from repl
//------------------------
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
