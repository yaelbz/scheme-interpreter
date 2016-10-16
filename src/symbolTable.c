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

static int symbolTableSize;
static int symbolTableFillSize;
static OBJ *symbolTable;

//------------------------
// init table
//------------------------
void initSymbolTable(){
	//printf("symbolTable --- init\n");
	symbolTable = (OBJ*)malloc(sizeof(OBJ)*TABLE_INITIAL_SIZE);
	memset( (void*)symbolTable, 0, (sizeof(OBJ) * TABLE_INITIAL_SIZE));
	symbolTableSize = TABLE_INITIAL_SIZE;
	symbolTableFillSize = 0;
}

//------------------------
// hash
// from: http://www.cse.yorku.ca/~oz/hash.html --> djb2
//------------------------
unsigned int hash(char *str)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *str++) != 0) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

//------------------------
// rehash table
//------------------------
void rehashSymbolTable(){
	//remember old table
	OBJ *oldSymbolTable = symbolTable;
	int oldSymbolTableSize = symbolTableSize;
	//new symbol table
	symbolTableSize = ((oldSymbolTableSize + 1) * 2) -1; //next x^2 -1
	symbolTable = (OBJ*)malloc(sizeof(OBJ)*symbolTableSize);
	memset( (void*)symbolTable, 0, (sizeof(OBJ) * symbolTableSize));

	OBJ oldSymbol;
	for (int i = 0; i < oldSymbolTableSize; ++i) {
		oldSymbol = oldSymbolTable[i];
		if(oldSymbol != NULL){
			//add to new table
			int startIndex = hash(oldSymbol->u.symbol.name) % symbolTableSize;
			int searchIndex = startIndex;
			OBJ storedSymbol;
			while(1){
				storedSymbol = symbolTable[searchIndex];
				if(storedSymbol == NULL){
					//empty slot - add symbol
					symbolTable[searchIndex] = oldSymbol;
					break;
				}

				//slot not empty. use antoher
				searchIndex = (searchIndex + 1) % symbolTableSize;

				if (searchIndex == startIndex) {
					//no empty slot found
					ybThrowError(-1, "symbolTable: fatal error. problem with rehash");
				}
			}

		}
	}

	free(oldSymbolTable);
}

//------------------------
// add to table
//------------------------
OBJ addToSymbolTable(char *symbolName){
	//rehash when env 3/4 full
	if(symbolTableFillSize > (symbolTableSize * 3 / 4)){
		rehashSymbolTable();
	}

	int startIndex = hash(symbolName) % symbolTableSize;
	int searchIndex = startIndex;

	//printf("symbolTable --- getOrAddFromSymbolTable %d\n", startIndex);

	OBJ storedSymbol;
	while(1){
		storedSymbol = symbolTable[searchIndex];
		if(storedSymbol == NULL){
			//empty slot - add symbol
			symbolTable[searchIndex] = newYbSymbol(symbolName);
			symbolTableFillSize++;
			//return saved symbol
			return symbolTable[searchIndex];
		}
		if(strcmp(storedSymbol->u.symbol.name, symbolName) == 0) {
			//symbol already exists - return
			return storedSymbol;
		}
		searchIndex = (searchIndex + 1) % symbolTableSize;
		if (searchIndex == startIndex) {
			//no empty slot found
			//should not happen - rehash when 3/4 full earlier
			ybThrowError(-1, "symbolTable: table full and symbol not found");
		}
	}
}


