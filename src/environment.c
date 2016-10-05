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


#define ENV_INITIAL_SIZE 511


static envEntry *env;
static int envCurrentSize;

/******************
 * init Environment
 *
 ******************/
void initEnv(){
	//printf("env --- initEnv\n");

	//environment initialisieren
	env = (envEntry*)malloc(sizeof(envEntry)*ENV_INITIAL_SIZE);
	memset( (void*)env, 0, (sizeof(envEntry) * ENV_INITIAL_SIZE));
	envCurrentSize = ENV_INITIAL_SIZE;
}

/******************
 * add to Environment
 *
 ******************/
void envAdd(OBJ key, OBJ value){

	int startIndex = (int)key % envCurrentSize;
	int searchIndex = startIndex;

	//printf("env --- envAdd 0x%08x\n", startIndex);

	OBJ storedKey;
	while(1){
		storedKey = env[searchIndex].key;
		if(storedKey == key){
			//key already exists - replace value
			env[searchIndex].value = value;
			return;
		}
		if(storedKey == NULL){
			//empty slot - store value
			env[searchIndex].key = key;
			env[searchIndex].value = value;
			//todo rehash wenn env 3/4 voll --> currentSize anpassen
			return;
		}
		searchIndex = (searchIndex + 1) % envCurrentSize;
		if (searchIndex == startIndex) {
			//no empty slot found
			//should not happen - rehash when 3/4 full earlier
			ybThrowError(-1, "env: env full");
		}
	}
}

/******************
 * get from Environment
 *
 ******************/
OBJ envGet(OBJ key){
	//printf("env --- envGet:\n");
	int startIndex = (int)key % envCurrentSize;
	int searchIndex = startIndex;

	OBJ storedKey;
	while(1){
		storedKey = env[searchIndex].key;
		if(storedKey == key){
			//found
			return env[searchIndex].value;
		}
		if(storedKey == NULL){
			//key does not exist in env
			return globalNil;
		}
		searchIndex = (searchIndex + 1) % envCurrentSize;
		if (searchIndex == startIndex) {
			//Env full - should not happen. Something went wrong in envAdd()
			return newYbError("env: searched key not found since env is full. check envAdd -> rehash");
		}
	}
}
