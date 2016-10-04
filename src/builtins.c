/*
 * builtins.c
 *
 *  Created on: 03.10.2016
 *      Author: yael
 */

#include "global.h"
#include "evalStack.h"


OBJ builtinPlus(int numArgs){
	int sum =0;

	while(numArgs>0){
		OBJ obj = popFromEvalStack();
		if(obj->u.any.type==T_INT){
			sum += obj->u.integer.value;
			numArgs--;
		}
		else{
			ybThrowError(-1, "builtinPlus can only handle integers");
		}
	}

	return newYbInteger(sum);
}
