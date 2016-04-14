/*
 * memory.c
 *
 *  Created on: 13.04.2016
 *      Author: yael
 */
#include "scheme.h"

OBJ
newInteger(scheme_int64 iVal) {
    struct schemeInteger *theInt;

    theInt = (struct schemeInteger *)(malloc( sizeof(struct schemeInteger)));
    theInt->tag = T_INTEGER;
    theInt->intVal = iVal;
    return (OBJ)theInt;

}


