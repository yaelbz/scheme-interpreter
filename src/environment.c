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

// #### init #######################################################################################


//------------------------
// init
//------------------------
void initEnv(){
	//printf("env --- initEnv\n");

	globalEnv = newYbEnvironment(GLOBALENV_INITIAL_SIZE, NULL);
}

// #### add #######################################################################################


//------------------------
// add to hashed environment
//------------------------
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
				//todo rehash when env 3/4 full
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

//------------------------
// add to local environment
//------------------------
void localEnvAdd(OBJ env, OBJ key, OBJ value){
	for (int i = 0; i < env->u.environment.size; ++i) {
		if(env->u.environment.entries[i].key == key){
			//replace value
			env->u.environment.entries[i].value = value;
			return;
		}
		else if(env->u.environment.entries[i].key == NULL){
			//empty slot
			env->u.environment.entries[i].key = key;
			env->u.environment.entries[i].value = value;
			return;
		}
	}
	ybThrowError(-1, "localEnv full");
}

//------------------------
// add to environment
//
//------------------------
void envAdd(OBJ env, OBJ key, OBJ value){
	if(env == NULL) {
		env = globalEnv;
	}

	if(env == globalEnv) {
		globalEnvAdd(env, key, value);
	} else {
		localEnvAdd(env, key, value);
	}

}

// #### get #######################################################################################


//------------------------
// get from hashed environment
//------------------------
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

//------------------------
// get from local environment
//------------------------
OBJ localEnvGet(OBJ env, OBJ key){
	for (int i = 0; i < env->u.environment.size; ++i) {
		if(env->u.environment.entries[i].key == key){
			//return value
			return env->u.environment.entries[i].value;
		}
	}
	//key does not exist in env
	return envGet(env->u.environment.parentEnv, key);
}

//------------------------
// get from environment
//------------------------
OBJ envGet(OBJ env, OBJ key){
	if(env == NULL) {
		env = globalEnv;
	}
	if(env == globalEnv) {
		return globalEnvGet(env, key);
	} else {
		return localEnvGet(env, key);
	}
}
