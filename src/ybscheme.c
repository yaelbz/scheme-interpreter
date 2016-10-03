/*
 * ybscheme.c
 *
 *  Created on: 01.09.2016
 *      Author: yael
 */

#include <stdio.h>
#include "global.h"
#include "reader.h"
#include "evaluator.h"
#include "printer.h"



int main(){

	//selftest();

	printf("Hallöle!\n");

	//repl
	while(1){
		printf("yscm> ");
		//read
		OBJ readObj = ybRead(stdin);
		//eval
		OBJ evalObj = ybEval(readObj);
		//print
		ybPrint(evalObj);
	}

	//todo clean up free memory - durch sig9


	return 0;
}
