/*
 * global.h
 * Für globale Definitionen die von überall zugänglich sind
 *
 *  Created on: 01.09.2016
 *      Author: yael
 *
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

//--- types of objects ---//

typedef enum {
	T_NIL,
	T_INT,
	T_STRING,
	T_SYMBOL,
	T_CONS
}objType;

//--- object definitions ---//

typedef struct ybObject *OBJ;

struct ybAny {
	objType type;
};

struct ybInt {
	objType type;
	int value;
};

struct ybSymbol {
	objType type;
	char *name;
};

//kommt irgendwann wieder weg, bzw wird anders
struct ybString {
	objType type;
	char *string;
};

struct ybCons {
	objType type;
	OBJ first; //car
	OBJ rest; //cdr
};


//--- general object ---//

struct ybObject {
	union {
		struct ybAny any;
		struct ybInt integer;
		struct ybString string;
		struct ybSymbol symbol;
		struct ybCons cons;
	} u;
};



void ybError(int, const char *, ...);


#endif /* GLOBAL_H_ */
