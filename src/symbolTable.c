/*
 * symbolTable.c
 *
 *  Created on: 04.10.2016
 *      Author: yael
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

#define TABLE_INITIAL_SIZE 511

static int symbolTableCurrentSize;
static OBJ *symbolTable;

/******************
 * init table
 *
 ******************/
void initSymbolTable(){
	//printf("symbolTable --- init\n");

	//environment initialisieren
	symbolTable = (OBJ*)malloc(sizeof(OBJ)*TABLE_INITIAL_SIZE);
	memset( (void*)symbolTable, 0, (sizeof(OBJ) * TABLE_INITIAL_SIZE));
	symbolTableCurrentSize = TABLE_INITIAL_SIZE;
}


/******************
 * hash
 * from: http://www.cse.yorku.ca/~oz/hash.html --> djb2
 * es gibt wohl auch ein strhash.h was man mit include einbinden kann - vllt mal angucken
 ******************/
unsigned int hash(char *str)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *str++) != 0) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

/******************
 * add to table
 *
 ******************/
OBJ addToSymbolTable(char *symbolName){
	int startIndex = hash(symbolName) % symbolTableCurrentSize;
	int searchIndex = startIndex;

	//printf("symbolTable --- getOrAddFromSymbolTable %d\n", startIndex);

	OBJ storedSymbol;
	while(1){
		storedSymbol = symbolTable[searchIndex];
		if(storedSymbol == NULL){
			//empty slot - add symbol
			symbolTable[searchIndex] = newYbSymbol(symbolName);
			//todo rehash wenn env 3/4 voll --> currentSize anpassen
			//return saved symbol
			return symbolTable[searchIndex];
		}
		if(strcmp(storedSymbol->u.symbol.name, symbolName) == 0) {
			//symbol already exists - return
			return storedSymbol;
		}
		searchIndex = (searchIndex + 1) % symbolTableCurrentSize;
		if (searchIndex == startIndex) {
			//no empty slot found
			//should not happen - rehash when 3/4 full earlier
			ybThrowError(-1, "symbolTable: table full and symbol not found");
		}
	}
}
