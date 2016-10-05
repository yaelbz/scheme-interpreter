/*
 * builtins.c
 *
 *  Created on: 03.10.2016
 *      Author: yael
 */

//todo hier könnten doch sowohl die builtin-Funktions als auch die builtin-Syntax rein

#include <stdio.h>

#include "global.h"
#include "evalStack.h"
//Werbung: Hier könnte Ihr include stehen!


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
