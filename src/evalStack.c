/*
 * evalStack.c
 *
 */

#include "global.h"
#include <stdlib.h>

#define EVAL_STACK_INITIAL_SIZE 100

OBJ *evalStack;
int evalStackPointer;


void initEvalStack(){
	evalStack = (OBJ*)malloc(sizeof(OBJ)*EVAL_STACK_INITIAL_SIZE);
	//stackPointer points to next empty slot
	evalStackPointer=0;
}

void pushToEvalStack(OBJ obj){
	if(evalStackPointer>=EVAL_STACK_INITIAL_SIZE){
		//stack full
		//todo enlarge stack
		ybThrowError(-1, "eval: stack overflow");
	}
	evalStack[evalStackPointer]=obj;
	evalStackPointer++;
}

OBJ popFromEvalStack(){
	//if stackPointer = 0 --> stack empty
	if(evalStackPointer>0){
		evalStackPointer--;
		return evalStack[evalStackPointer];
	}
	else{
		return newYbError("eval: no more objects in evalStack");
	}
}
