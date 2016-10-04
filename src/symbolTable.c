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
	printf("symbolTable --- init\n");

	//environment initialisieren
	symbolTable = (OBJ*)malloc(sizeof(OBJ)*TABLE_INITIAL_SIZE);
	memset( (void*)symbolTable, 0, (sizeof(OBJ) * TABLE_INITIAL_SIZE));
	symbolTableCurrentSize = TABLE_INITIAL_SIZE;
}


/******************
 * hash
 * from: http://www.cse.yorku.ca/~oz/hash.html --> djb2
 * todo: es gibt wohl auch ein strhash.h was man mit include einbinden kann - vllt mal angucken
 ******************/
unsigned int hash(char *str)
{
    unsigned int hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/******************
 * add to table
 * todo when not needed
 ******************/
void addToSymbolTable(OBJ symbolToAdd){
	if(symbolToAdd->u.any.type != T_SYMBOL){
		//todo Fehler
		return;
	}
	int startIndex = hash(symbolToAdd->u.symbol.name) % symbolTableCurrentSize;
	int searchIndex = startIndex;

	printf("symbolTable --- addToSymbolTable 0x%08x\n", startIndex);

	OBJ storedSymbol;
	while(1){
		storedSymbol = symbolTable[searchIndex];
		if(strcmp(storedSymbol->u.symbol.name, symbolToAdd->u.symbol.name) == 0) {
			//symbol already exists - what now? nothing?
			return;
		}
		if(storedSymbol == NULL){
			//empty slot - add symbol
			symbolTable[searchIndex] = symbolToAdd;
			//todo rehash wenn env 3/4 voll --> currentSize anpassen
			return;
		}
		searchIndex = (searchIndex + 1) % symbolTableCurrentSize;
		if (searchIndex == startIndex) {
			//no empty slot found
			//should not happen - rehash when 3/4 full earlier
			ybThrowError(-1, "symbolTable: table full");
		}
	}
}

/******************
 * get from table
 * todo when not needed
 ******************/
OBJ getFromSymbolTable(char *symbolName){
	printf("symbolTable --- getFromSymbolTable:\n");
	int startIndex = hash(symbolName) % symbolTableCurrentSize;
	int searchIndex = startIndex;

	OBJ storedSymbol;
	while(1){
		storedSymbol = symbolTable[searchIndex];
		if(strcmp(storedSymbol->u.symbol.name, symbolName) == 0){
			//found
			return storedSymbol;
		}
		if(storedSymbol == NULL){
			//key does not exist in env
			return newYbNil();
		}
		searchIndex = (searchIndex + 1) % symbolTableCurrentSize;
		if (searchIndex == startIndex) {
			//Env full - should not happen. Something went wrong in add function
			return newYbError("env: searched key not found since symbolTable is full. check addToSymbolTable -> rehash");
		}
	}
}

/******************
 * add or get from table
 *
 ******************/
OBJ getOrAddFromSymbolTable(char *symbolName){
	int startIndex = hash(symbolName) % symbolTableCurrentSize;
	int searchIndex = startIndex;

	printf("symbolTable --- getOrAddFromSymbolTable %d\n", startIndex);

	OBJ storedSymbol;
	while(1){
		printf(".");
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
			//todo passt das so hier?
			ybThrowError(-1, "symbolTable: table full");
		}
	}
}
