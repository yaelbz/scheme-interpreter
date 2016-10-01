/*
 * ybscheme.c
 *
 *  Created on: 01.09.2016
 *      Author: yael
 */

#include <stdio.h>
#include "global.h"
#include "reader.h"
#include "printer.h"
#include "selftest.h"

int main(){

	//selftest();

	printf("Hallöle!\n");

	//repl
	while(1){
		printf("yscm> ");
		//read
		OBJ obj = ybRead(stdin);
		//eval
		//print
		ybPrint(obj);
	}


	return 0;
}
