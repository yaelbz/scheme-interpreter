/*
 * evaluator.c
 *
 *  Created on: 02.10.2016
 *      Author: yael
 */

#include "global.h"

OBJ ybEvalSymbol(OBJ obj){
	return obj;
}

OBJ ybEvalCons(OBJ obj){
	return obj;
}


OBJ ybEval(OBJ obj){
	switch(obj->u.any.type){
	case T_ERROR:
	case T_NIL:
	case T_STRING:
	case T_INT:
		return obj;
	case T_SYMBOL:
		return ybEvalSymbol(obj);
	case T_CONS:
		return ybEvalCons(obj);
	default:
		//ybThrowError(-1, "eval: unknown type: 0x%x", obj->u.any.type);
		return newYbError("eval: unknown type");
	}
}
