/*
 * builtins.c
 *
 */


#include <stdio.h>
#include <float.h>
#include <string.h>

#include "global.h"
#include "evaluator.h"
#include "evalStack.h"
#include "environment.h"
#include "symbolTable.h"

// #### functions #######################################################################################

//------------------------
// +
//------------------------
OBJ builtinPlus(int numArgs){
	long sum =0;

	while(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(TYPE(obj)==T_NUMBER && obj->u.number.isInteger) {
			sum += obj->u.number.value.i;
			numArgs--;
		} else {
			return newYbError("builtin (+) can only handle integers");
		}
	}

	return newYbIntNumber(sum);
}

//------------------------
// -
//------------------------
OBJ builtinMinus(int numArgs){
	long diff = 0;

	if(numArgs < 1){
		return newYbError("builtin (-) expects at least 1 argument");
	}
	while(numArgs>1){
		OBJ obj = popFromEvalStack();
		if(TYPE(obj)==T_NUMBER && obj->u.number.isInteger) {
			diff -= obj->u.number.value.i;
			numArgs--;
		} else {
			return newYbError("builtin (-) can only handle integers");
		}
	}
	if(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(TYPE(obj)==T_NUMBER && obj->u.number.isInteger) {
			diff += obj->u.number.value.i;
			numArgs--;
		} else {
			return newYbError("builtin (-) can only handle integers");
		}
	}

	return newYbIntNumber(diff);
}

//------------------------
// *
//------------------------
OBJ builtinMultiplication(int numArgs){
	long prod =1;

	while(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(TYPE(obj)==T_NUMBER && obj->u.number.isInteger) {
			prod *= obj->u.number.value.i;
			numArgs--;
		} else {
			return newYbError("builtin (*) can only handle integers");
		}
	}

	return newYbIntNumber(prod);
}

//------------------------
// /
//------------------------
OBJ builtinDivision(int numArgs){
	double div = 1;

	if(numArgs<1){
		return newYbError("builtin (/) expects at least 1 argument");
	}
	while(numArgs>1){
		OBJ obj = popFromEvalStack();
		if(TYPE(obj)==T_NUMBER && obj->u.number.isInteger) {
			div /= (double)obj->u.number.value.i;
			numArgs--;
		} else {
			return newYbError("builtin (/) can only handle integers");
		}
	}
	if(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(TYPE(obj)==T_NUMBER && obj->u.number.isInteger) {
			div *= (double)obj->u.number.value.i;
			numArgs--;
		} else {
			return newYbError("builtin (/) can only handle integers");
		}
	}

	return newYbFloatNumber(div);
}

//equal - helper functions


//------------------------
// todo source?
//------------------------
bool areFloatsEqual(double a, double b){
	double diff = a-b;
	return (diff<DBL_EPSILON) && (-diff<DBL_EPSILON);
}

//------------------------
// (eq? x x)
// compare if same object
//------------------------
OBJ builtinEqQ(int numArgs){
	if(numArgs != 2){
		return newYbError("builtin (eq?): expects exactly two arguments");
	}

	OBJ secondObj = popFromEvalStack();
	OBJ firstObj = popFromEvalStack();


	if(firstObj == secondObj){
		return globalTrue;
	}

	return globalFalse;
}

//------------------------
// (= x x)
// compare if two numbers are equivalent.
// only for numbers
//------------------------
OBJ builtinEqualOperator(int numArgs){
	if(numArgs != 2){
			return newYbError("builtin (=): expects exactly two arguments");
		}

	OBJ secondObj = popFromEvalStack();
	OBJ firstObj = popFromEvalStack();

	//both arguments must be numbers
	if(TYPE(firstObj)!=T_NUMBER || TYPE(secondObj)!=T_NUMBER){
		return newYbError("builtin (=): Arguments must be numbers");
	}

	//if both arguments are integers
	if(firstObj->u.number.isInteger && secondObj->u.number.isInteger){
		//if both arguments (numbers) have same value
		if(firstObj->u.number.value.i == secondObj->u.number.value.i){
			return globalTrue;
		}
	}

	//if both arguments are float (not integer)
	if(!firstObj->u.number.isInteger && !secondObj->u.number.isInteger){
		if(areFloatsEqual(firstObj->u.number.value.f, secondObj->u.number.value.f)){
			return globalTrue;
		}
	}

	//frage was wenn ein objekt int ist und ein objekt float ist?

	return globalFalse;
}

//------------------------
// (eqv? x x)
// compare if two non-numeric values are equivalent.
//------------------------
OBJ builtinEqvQ(int numArgs){
	if(numArgs != 2){
		return newYbError("builtin (eqv?): expects exactly two arguments");
	}

	OBJ secondObj = popFromEvalStack();
	OBJ firstObj = popFromEvalStack();

	//if both arguments are numbers
	if((TYPE(firstObj) == T_NUMBER) && (TYPE(secondObj) == T_NUMBER)){
		//push objects back to stack to use already implemented function
		pushToEvalStack(firstObj);
		pushToEvalStack(secondObj);
		return builtinEqualOperator(numArgs);
	}

	//if both arguments are strings
	if(TYPE(firstObj) == T_STRING && TYPE(secondObj) == T_STRING){
		if(strcmp(firstObj->u.string.string, secondObj->u.string.string) == 0){
			return globalTrue;
		}
	}

	return globalFalse;

	//frage was ist mit anderen Objekttypen?
}

//------------------------
// (equal? x x)
// to test if two lists, vectors, etc. are equivalent.
//------------------------
/*OBJ builtinEqualQ(int numArgs){

}*/


//------------------------
// not
// expects only one argument
//
// > (if (= 3 3) 1 2)
// 1
// > (if (not (= 3 3)) 1 2)
// 2
//------------------------
OBJ builtinNot(int numArgs){
	if(numArgs!=1){
		return newYbError("builtin (not): expects only one argument");
	}

	OBJ obj = popFromEvalStack();

	if(TYPE(obj) == T_ERROR){
		return obj;
	}

	if(obj == globalFalse){
		return globalTrue;
	}

	return globalFalse;
}

//------------------------
// cons
//------------------------
OBJ builtinCons(int numArgs){
	if(numArgs != 2){
		return newYbError("builtin (cons): expects two arguments");
	}

	OBJ rest = popFromEvalStack();
	OBJ first = popFromEvalStack();

	return newYbCons(first, rest);

}

//------------------------
// car
//------------------------
OBJ builtinCar(int numArgs){
	if(numArgs != 1){
		return newYbError("biultin (car): expects one argument");
	}
	OBJ obj = popFromEvalStack();
	if(TYPE(obj) == T_CONS){
		return FIRST(obj);
	}
	return newYbError("builtin (car); object not a cons");
}

//------------------------
// cdr
//------------------------
OBJ builtinCdr(int numArgs){
	if(numArgs != 1){
		return newYbError("biultin (cdr): expects one argument");
	}
	OBJ obj = popFromEvalStack();
	if(TYPE(obj) == T_CONS){
		return REST(obj);
	}
	return newYbError("builtin (cdr); object not a cons");

}



// #### syntax #######################################################################################


//------------------------
// if
//------------------------
OBJ builtinIf(OBJ env, OBJ objList){

	if(TYPE(objList) != T_CONS){
		return newYbError("builtin (if): expects two or three arguments");
	}

	OBJ conditionExpr = FIRST(objList);
	if(TYPE(REST(objList)) != T_CONS){
		return newYbError("builtin (if): expects two or three arguments");
	}
	objList = REST(objList);

	OBJ trueExpr = FIRST(objList);
	OBJ falseExpr = globalNil;
	if(TYPE(REST(objList)) == T_CONS){
		objList = REST(objList);
		falseExpr = FIRST(objList);
		if(REST(objList) != globalNil){
			return newYbError("builtin (if): expects two or three arguments");
		}
	}

	//evaluate condition
	OBJ evaluatedCondition = ybEval(env, conditionExpr);
	if(evaluatedCondition == globalTrue){
		return ybEval(env, trueExpr);
	}
	if(evaluatedCondition == globalFalse){
		return ybEval(env, falseExpr);
	}

	return newYbError("builtin (if): condition is not a boolean");
}




//------------------------
// count defines
// --> helper function for lambda
//------------------------
static int countDefines(OBJ bodyList) {
	int count = 0;

	while (bodyList != globalNil) {
		OBJ first = FIRST(bodyList);
		if(TYPE(first) == T_CONS){
			if (FIRST(first) == globalDefine) {
				count++;
			}
		}

		bodyList = REST(bodyList);
	}
	return count;
}


//------------------------
// lambda
//------------------------
OBJ builtinLambda(OBJ env, OBJ objList){

	if(TYPE(objList) != T_CONS){
		return newYbError("builtin (lambda): expects at least two arguments");
	}

	OBJ parameterList = FIRST(objList);
	OBJ bodyList = REST(objList);

	if(TYPE(parameterList) != T_CONS){
		return newYbError("builtin (lambda): must have a list of parameter");
	}
	if(TYPE(bodyList) != T_CONS){
		return newYbError("builtin (lambda): must have a body");
	}

	OBJ udfObj =  newYbUserDefinedFunction("unnamed", env, parameterList, bodyList, countDefines(bodyList));

	return udfObj;
}


//------------------------
// define
//------------------------
OBJ builtinDefine(OBJ env, OBJ listOfArguments){

	if(TYPE(listOfArguments) != T_CONS){
		return newYbError("builtin (define): expects at least two arguments");
	}

	OBJ firstObj = FIRST(listOfArguments);
	OBJ secondObj = REST(listOfArguments);

	switch (TYPE(firstObj)) {
		case T_SYMBOL:
			//when first Obj is a symbol, there must be only one more object
			if(REST(secondObj) == globalNil){
				//new binding in env
				envAdd(env, addToSymbolTable(firstObj->u.symbol.name), ybEval(env, FIRST(secondObj)));
				return globalVoid;
			}
			return newYbError("builtin (define): expects two arguments");
		case T_CONS:
		{
			OBJ lambdaArgs = newYbCons(REST(firstObj), secondObj);
			OBJ udfValue = builtinLambda(env, lambdaArgs);
			OBJ udfKey = FIRST(firstObj);
			if(TYPE(udfKey) == T_SYMBOL){
				if(TYPE(udfValue) == T_USER_FUNCTION){
					envAdd(env, udfKey, udfValue);
					return globalVoid;
				}
				//forward error
				return udfValue;
			}
			return newYbError("builtin (define): identifier not a symbol");
		}
		default:
			//error
			return newYbError("builtin (define); error");
	}
}

//------------------------
// quote
//------------------------
OBJ builtinQuote(OBJ env, OBJ listOfArguments) {

	if(TYPE(listOfArguments) != T_CONS){
		return newYbError("builtin (quote): expects exactly one argument");
	}

	OBJ obj = FIRST(listOfArguments);
	OBJ restArgs = REST(listOfArguments);
	if(restArgs != globalNil){
		return newYbError("builtin (quote): expects exactly one argument");
	}

	//free listOfArguments
	FIRST(listOfArguments) = globalNil;
	freeObject(listOfArguments);

	return obj;
}

