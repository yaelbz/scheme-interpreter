/*
 * evalStack.c
 *
 *  Created on: 03.10.2016
 *      Author: yael
 */

#include "global.h"
#include <stdlib.h>

#define EVAL_STACK_INITIAL_SIZE 100

OBJ *evalStack;
int evalStackPointer;


void initEvalStack(){
	//evalStack initialisieren
	evalStack = (OBJ*)malloc(sizeof(OBJ)*EVAL_STACK_INITIAL_SIZE);
	//der stackPointer zeigt immer auf das nächste freie Feld
	evalStackPointer=0;
}

void pushToEvalStack(OBJ obj){
	if(evalStackPointer>=EVAL_STACK_INITIAL_SIZE){
		//stack voll
		//todo stack vergrößern
		ybThrowError(-1, "eval: stack overflow");
	}
	evalStack[evalStackPointer]=obj;
	evalStackPointer++;
}

OBJ popFromEvalStack(){
	//wenn der stackPointer=0 ist, dann ist der Stack leer
	if(evalStackPointer>0){
		evalStackPointer--;
		return evalStack[evalStackPointer];
	}
	else{
		return newYbError("eval: no more objects in evalStack");
	}
}
