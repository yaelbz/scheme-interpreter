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

#include <stdbool.h>

//--- types of objects ---//

typedef enum {
	T_ERROR=-1,
	T_NIL=0,
	T_TRUE,
	T_FALSE,
	T_INT,
	T_NUMBER,
	T_STRING,
	T_SYMBOL,
	T_CONS,
	T_FCT_BUILTIN,
	T_SYNTAX,
	T_FCT_USER,
	T_ENVIRONMENT,
	T_BOOL,
}objType;

//--- object definitions ---//

typedef struct ybObject *OBJ;
typedef OBJ (*ybFctPtr)(int); //Function pointer for builtin function, syntax (and user defined functions)


struct ybAny {
	objType type;
};

struct ybError {
	objType type;
	char *message;
};

struct ybInt {
	objType type;
	long value;
};

struct ybNumber {
	objType type;
	//todo merken was es ist- i oder f
	union {
		long   i;
		double f;
	} value;
	bool isInteger;
};

struct ybSymbol {
	objType type;
	char *name;
};

struct ybString {
	objType type;
	char *string;
};

struct ybBool {
	objType type;
	bool value;
};

struct ybCons {
	objType type;
	OBJ first; //car
	OBJ rest; //cdr
};

struct ybFctBuiltin {
	objType type;
	char *name;
	ybFctPtr impl; //implementation
};

struct ybSyntax {
	objType type;
	char *name;
	ybFctPtr impl; //implementation
};

struct ybEnvironment {
	objType type;
	//todo was soll das enthalten? array?
};


//--- general object ---//

struct ybObject {
	union {
		struct ybAny    	 any;
		struct ybError  	 error;
		struct ybInt    	 integer;
		struct ybNumber    	 number;
		struct ybString 	 string;
		struct ybSymbol 	 symbol;
		struct ybBool		 boolean;
		struct ybCons   	 cons;
		struct ybFctBuiltin  fctBuiltin;
		struct ybSyntax		 syntax;
		struct ybEnvironment environment;
		//struct ybFctUser 	fctUser;
	} u;
};

//macro
#define TYPE(x) ((x)->u.any.type)

OBJ globalNil;
OBJ globalTrue;
OBJ globalFalse;

// prototypen

void initGlobals();

OBJ newYbError(char *);
OBJ newYbInteger(long);
OBJ newYbIntNumber(long);
OBJ newYbFloatNumber(double);
OBJ newYbString(char *);
OBJ newYbSymbol(char *);
OBJ newYbBool(bool);
OBJ newYbCons(OBJ, OBJ);
OBJ newYbFctBuiltin(char *, ybFctPtr);
OBJ newYbSyntax(char *, ybFctPtr);
OBJ newYbEnvironment(); //todo parameter???



void ybThrowError(int, const char *, ...);





#endif /* GLOBAL_H_ */
