/*
 * environment.c
 *
 *  Created on: 03.10.2016
 *      Author: yael
 */

#include "environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAINENV_INITIAL_SIZE 511

static ybEnvironment *mainEnv; //frage mit * oder ohne?
static int mainEnvCurrentSize;

/******************
 * init Environment
 *
 ******************/
void initEnv(){
	//printf("env --- initEnv\n");

	//environment initialisieren
	//mainEnv
	//frage ist das richtig dass ich auch hier den speicher mit malloc reservieren muss?
	keyValuePair *envEntries = (keyValuePair*)malloc(sizeof(keyValuePair)*MAINENV_INITIAL_SIZE);
	memset( (void*)envEntries, 0, (sizeof(keyValuePair) * MAINENV_INITIAL_SIZE));
	mainEnv = newYbEnvironment(MAINENV_INITIAL_SIZE, NULL, envEntries);
	mainEnvCurrentSize = MAINENV_INITIAL_SIZE;
}

/******************
 * add to Environment
 *
 ******************/
void envAdd(OBJ key, OBJ value){

	int startIndex = (int)key % mainEnvCurrentSize;
	int searchIndex = startIndex;

	//printf("env --- envAdd 0x%08x\n", startIndex);

	OBJ storedKey;
	while(1){
		storedKey = mainEnv->entries[searchIndex].key;
		if(storedKey == key){
			//key already exists - replace value
			mainEnv->entries[searchIndex].value = value;
			return;
		}
		if(storedKey == NULL){
			//empty slot - store value
			mainEnv->entries[searchIndex].key = key;
			mainEnv->entries[searchIndex].value = value;
			//todo rehash wenn env 3/4 voll --> currentSize anpassen
			return;
		}
		searchIndex = (searchIndex + 1) % mainEnvCurrentSize;
		if (searchIndex == startIndex) {
			//no empty slot found
			//should not happen - rehash when 3/4 full earlier
			ybThrowError(-1, "env: mainEnv full");
		}
	}
}

/******************
 * get from Environment
 *
 ******************/
OBJ envGet(OBJ key){
	//printf("env --- envGet:\n");
	int startIndex = (int)key % mainEnvCurrentSize;
	int searchIndex = startIndex;

	OBJ storedKey;
	while(1){
		storedKey = mainEnv->entries[searchIndex].key;
		if(storedKey == key){
			//found
			return mainEnv->entries[searchIndex].value;
		}
		if(storedKey == NULL){
			//key does not exist in env
			return globalNil;
		}
		searchIndex = (searchIndex + 1) % mainEnvCurrentSize;
		if (searchIndex == startIndex) {
			//Env full - should not happen. Something went wrong in envAdd()
			return newYbError("env: searched key not found since mainEnv is full. check envAdd -> rehash");
		}
	}
}
