/*
 * builtins.c
 *
 *  Created on: 03.10.2016
 *      Author: yael
 */


#include <stdio.h>

#include "global.h"
#include "evaluator.h"
#include "evalStack.h"
//Werbung: Hier könnte Ihr include stehen!

//--------------- functions ----------------------//

OBJ builtinPlus(int numArgs){
	long sum =0;

	while(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(obj->u.any.type==T_INT){
			sum += obj->u.integer.value;
			numArgs--;
		}
		else{
			ybThrowError(-1, "builtin (+) can only handle integers");
		}
	}

	return newYbInteger(sum);
}

OBJ builtinMinus(int numArgs){
	long diff = 0;

	if(numArgs < 1){
		ybThrowError(-1, "builtin (-) needs minimum 1 argument");
	}
	while(numArgs>1){
		OBJ obj = popFromEvalStack();
		if(obj->u.any.type==T_INT){
			diff -= obj->u.integer.value;
			numArgs--;
		}
		else{
			ybThrowError(-1, "builtin (-) can only handle integers");
		}
	}
	if(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(obj->u.any.type==T_INT){
			diff += obj->u.integer.value;
			numArgs--;
		}
		else{
			ybThrowError(-1, "builtin (-) can only handle integers");
		}
	}

	return newYbInteger(diff);
}

OBJ builtinMultiplication(int numArgs){
	long prod =1;

	while(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(obj->u.any.type==T_INT){
			prod *= obj->u.integer.value;
			numArgs--;
		}
		else{
			ybThrowError(-1, "builtin (*) can only handle integers");
		}
	}

	return newYbInteger(prod);
}


OBJ builtinDivision(int numArgs){
	double div = 1;

	if(numArgs<1){
		ybThrowError(-1, "builtin (/) needs minimum 1 argument");
	}
	while(numArgs>1){
		OBJ obj = popFromEvalStack();
		if(obj->u.any.type==T_INT){
			div /= (double)obj->u.integer.value;
			numArgs--;
		}
		else{
			ybThrowError(-1, "builtin (/) can only handle integers");
		}
	}
	if(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(obj->u.any.type==T_INT){
			div *= (double)obj->u.integer.value;
			numArgs--;
		}
		else{
			ybThrowError(-1, "builtin (/) can only handle integers");
		}
	}

	return newYbFloatNumber(div);
}

//--------------- syntax ----------------------//
/*
OBJ builtinEqual(OBJ env, OBJ listOfArguments){

}
*/

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
