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
		return newYbError("builtin (-) needs minimum 1 argument");
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
		return newYbError("builtin (/) needs minimum 1 argument");
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

bool hasTwoArguments(OBJ listOfArguments){
	if(TYPE(listOfArguments) != T_CONS){
		return false;
	}

	if(TYPE(REST(listOfArguments)) != T_CONS){
		//only one argument
		return false;
	}

	//false: more than two arguments
	return (REST(REST(listOfArguments)) == globalNil);
}

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
		return newYbError("builtin (not): epects only one argument");
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


// #### syntax #######################################################################################


//------------------------
// if
//------------------------
OBJ builtinIf(OBJ env, OBJ listOfArguments){

	if(TYPE(listOfArguments) != T_CONS){
		return newYbError("builtin (if): expects two or three arguments");
	}

	OBJ conditionExpr = FIRST(listOfArguments);
	if(TYPE(REST(listOfArguments)) != T_CONS){
		return newYbError("builtin (if): expects two or three arguments");
	}
	listOfArguments = REST(listOfArguments);

	OBJ trueExpr = FIRST(listOfArguments);
	OBJ falseExpr = globalNil;
	if(TYPE(REST(listOfArguments)) == T_CONS){
		listOfArguments = REST(listOfArguments);
		falseExpr = FIRST(listOfArguments);
		if(REST(listOfArguments) != globalNil){
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



/****
 * Gittinger define-function nimmt als Parameter eine environment und eine Argumentenliste (und nicht die numArgs)
 */
/*
OBJ builtinDefine(OBJ env, OBJ listOfArguments){
	OBJ obj;



	Fälle:
	1. wenn nur ein Argument --> Fehler. Define braucht mind. zwei Argumente
	2. Wenn erstes Argument ein Symbol
		Dann werden die restlichen Argumente an dieses Symbol gebunden?
		laut gittinger muss es dann exakt zwei argumente geben
		Bis nil?
	3. Wenn erstes Argument ein Cons
		Was heißt das dann?
		Dann wirds ne user defined function. Der User will eine fkt definieren
	4. wenn das erste Argument etwas anderes ist --> Fehler! (zb nummer oder string)


	return obj;
}
*/
/****
 * lambda
 * (lambda kw-formals body ...+)
 * macht funktion (ohne namen)
 */
/*
OBJ builtinLambda(int numArgs){
	OBJ obj;


	return obj;
}
*/
