/*
 * evalStack.c
 *
 */

#include "global.h"
#include <stdlib.h>

#define EVAL_STACK_INITIAL_SIZE 10

OBJ *evalStack;
int evalStackPointer;
int evalStackSize;


void initEvalStack(){
	evalStack = (OBJ*)malloc(sizeof(OBJ)*EVAL_STACK_INITIAL_SIZE);
	evalStackSize = EVAL_STACK_INITIAL_SIZE;
	//stackPointer points to next empty slot
	evalStackPointer=0;
}

void pushToEvalStack(OBJ obj){
	if(evalStackPointer>=evalStackSize){
		//stack full
		//enlarge stack
		evalStackSize += EVAL_STACK_INITIAL_SIZE;
		evalStack = (OBJ*)realloc(evalStack, sizeof(OBJ)*evalStackSize);
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
