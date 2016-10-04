/*
 * environment.h
 *
 *  Created on: 03.10.2016
 *      Author: yael
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "global.h"

typedef struct {
	OBJ key; //evtl ein stringObject
	OBJ value; //kann jedes Object sein
} envEntry;


void initEnv();
void envAdd(OBJ, OBJ);
OBJ envGet(OBJ);


#endif /* ENVIRONMENT_H_ */
