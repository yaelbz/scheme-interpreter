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

	printf("Hallöle!\n");

	initGlobals();
	initReader();
	initEvaluator();

	//repl
	while(1){
		printf("yscm> ");
		fflush(stdout);
		//read
		OBJ readObj = ybRead(stdin);
		//eval. NULL = global environment
		OBJ evalObj = ybEval(NULL, readObj);
		//print
		ybPrint(evalObj);
	}

	//todo clean up free memory - durch sig9


	return 0;
}
