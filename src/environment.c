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


#define GLOBALENV_INITIAL_SIZE 511

OBJ globalEnv;
int globalEnvSize;

/******************
 * init Environment
 *
 ******************/
void initEnv(){
	//printf("env --- initEnv\n");

	//global environment initialisieren
	globalEnv = newYbEnvironment(GLOBALENV_INITIAL_SIZE, NULL);
}

/******************
 * add to main Environment
 *
 ******************/
void globalEnvAdd(OBJ env, OBJ key, OBJ value){

	int startIndex = (long)key % env->u.environment.size;
	int searchIndex = startIndex;

	//printf("env --- envAdd 0x%08x\n", startIndex);

	OBJ storedKey;
	while(1){
		storedKey = env->u.environment.entries[searchIndex].key;
		if(storedKey == key){
			//key already exists - replace value
			env->u.environment.entries[searchIndex].value = value;
			return;
		}
		if(storedKey == NULL){
			//empty slot - store value
			env->u.environment.entries[searchIndex].key = key;
			env->u.environment.entries[searchIndex].value = value;
			env->u.environment.entryCount++;
			if(env->u.environment.entryCount >= env->u.environment.size*.75){
				//todo rehash wenn env 3/4 voll
			}
			return;
		}
		searchIndex = (searchIndex + 1) % env->u.environment.size;
		if (searchIndex == startIndex) {
			//no empty slot found
			//should not happen - rehash when 3/4 full earlier
			ybThrowError(-1, "env: mainEnv full");
		}
	}
}

/******************
 * add to Environment
 * mit optionalem parameter env. wenn nichts mitgegeben wird, dann wird in die main env geschrieben
 * frage ist das gut so oder ist das unsicher?
 ******************/
void envAdd(OBJ env, OBJ key, OBJ value){
	if(env == NULL) {
		env = globalEnv;
	}

	//TODO: Type prüfen, es könnte ja ein ganz anderes objekt sein...

	if(env == globalEnv) {
		globalEnvAdd(env, key, value);
	} else {
		//local environment
	}

	//else
	//wenn es noch freie slots in der env gibt -> wenn länge des entries-array kleiner size
	//env->entries[?].key = key;
	//env->entries[?].value = value;
}

/******************
 * get from main Environment
 *
 ******************/
OBJ globalEnvGet(OBJ env, OBJ key){
	//printf("env --- envGet:\n");
	int startIndex = (long)key % env->u.environment.size;
	int searchIndex = startIndex;

	OBJ storedKey;
	while(1){
		storedKey = globalEnv->u.environment.entries[searchIndex].key;
		if(storedKey == key){
			//found
			return globalEnv->u.environment.entries[searchIndex].value;
		}
		if(storedKey == NULL){
			//key does not exist in env
			return globalNil;
		}
		searchIndex = (searchIndex + 1) % env->u.environment.size;
		if (searchIndex == startIndex) {
			//Env full - should not happen. Something went wrong in envAdd()
			return newYbError("env: searched key not found since mainEnv is full. check envAdd -> rehash");
		}
	}
	return newYbError("env: searched key not found since mainEnv is full. check envAdd -> rehash");
}

/******************
 * get from Environment
 *
 ******************/
OBJ envGet(OBJ env, OBJ key){
	if(env == NULL) {
		env = globalEnv;
	}
	if(env == globalEnv) {
		return globalEnvGet(env, key);
	} else {
		//todo: local environment
		return globalEnvGet(env, key);
	}
}
