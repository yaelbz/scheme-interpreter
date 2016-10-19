/*
 * ybscheme.c
 *
 */

#include <stdio.h>
#include "global.h"
#include "reader.h"
#include "evaluator.h"
#include "printer.h"


int main(){

	printf("Welcome to ybscheme!\n");

	initGlobals();
	initReader();
	initEvaluator();

	//repl
	while(1){
		printf("> ");
		fflush(stdout);
		//read
		OBJ readObj = ybRead(stdin);
		//eval. NULL = global environment
		OBJ evalObj = ybEval(NULL, readObj);
		//print
		ybPrint(evalObj);
		if(TYPE(evalObj) == T_ERROR) {
			flushReaderInputStream();
		}
	}

	return 0;
}
