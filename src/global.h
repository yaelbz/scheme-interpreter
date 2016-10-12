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
	T_NUMBER,
	T_STRING,
	T_SYMBOL,
	T_CONS,
	T_BUILTIN_FUNCTION,
	T_BUILTIN_SYNTAX,
	T_USER_FUNCTION,
	T_ENVIRONMENT,
}objType;

//--- object definitions ---//

typedef struct ybObject *OBJ;
typedef OBJ (*ybFctPtr)(int); //Function pointer for builtin function
typedef OBJ (*ybSyntaxPtr)(OBJ, OBJ); //Function pointer for builtin syntax

struct ybAny {
	objType type;
};

struct ybError {
	objType type;
	char *message;
};

struct ybNumber {
	objType type;
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
/*
struct ybBool {
	objType type;
	bool value;
};
*/
struct ybCons {
	objType type;
	OBJ first; //car
	OBJ rest; //cdr
};

struct ybBuiltinFunction {
	objType type;
	char *name;
	ybFctPtr impl; //implementation
};

struct ybBuiltinSyntax {
	objType type;
	char *name;
	ybSyntaxPtr impl; //implementation
};

/*
struct ybUserFunction {
	objType type;
	ybEnvironment localEnv;
	char *name;
	ybSyntaxPtr impl; //implementation
};
*/
typedef struct {
	OBJ key;
	OBJ value;
}keyValuePair;

typedef struct {
	objType type;
	int size;
	int entryCount;
	OBJ parentEnv;
	keyValuePair entries[]; //Ein Array mit Key-Value-Paaren -> das ist im Prinzip die Env
} ybEnvironment;


//--- general object ---//

struct ybObject {
	union {
		struct ybAny    	 any;
		struct ybError  	 error;
		struct ybNumber    	 number;
		struct ybString 	 string;
		struct ybSymbol 	 symbol;
		//struct ybBool		 boolean;
		struct ybCons   	 cons;
		struct ybBuiltinFunction  builtinFct;
		struct ybBuiltinSyntax    builtinSyntax;
		ybEnvironment             environment;
		//struct ybUserFuntion 	fctUser;
	} u;
};

//macro
#define TYPE(x)      ((x)->u.any.type)
#define FIRST(x)     ((x)->u.cons.first)
#define REST(x)      ((x)->u.cons.rest)
#define IS_TRUE(x)   ((x)->u.any.type == T_TRUE)
#define IS_FALSE(x)  ((x)->u.any.type == T_FALSE)

OBJ globalNil;
OBJ globalTrue;
OBJ globalFalse;

// prototypen

void initGlobals();

OBJ newYbError(const char *, ...);
OBJ newYbIntNumber(long);
OBJ newYbFloatNumber(double);
OBJ newYbString(char *);
OBJ newYbSymbol(char *);
OBJ newYbBool(bool);
OBJ newYbCons(OBJ, OBJ);
OBJ newYbBuiltinFunction(char *, ybFctPtr);
OBJ newYbBuiltinSyntax(char *, ybSyntaxPtr);
OBJ newYbEnvironment(int, OBJ);



void ybThrowError(int, const char *, ...);





#endif /* GLOBAL_H_ */
