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
	T_ERROR=-1,
	T_NIL=0,
	T_INT,
	T_STRING,
	T_SYMBOL,
	T_CONS,
	T_FCT_BUILTIN,
	T_FCT_USER,
	T_KEYWORD //zb. define, if, else...
}objType;

//--- object definitions ---//

typedef struct ybObject *OBJ;
typedef OBJ (*ybFctPtr)(int); //Function pointer


struct ybAny {
	objType type;
};

struct ybError {
	objType type;
	char *message;
};

struct ybInt {
	objType type;
	int value;
};

struct ybSymbol {
	objType type;
	char *name;
};

struct ybString {
	objType type;
	char *string;
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


//--- general object ---//

struct ybObject {
	union {
		struct ybAny    any;
		struct ybError  error;
		struct ybInt    integer;
		struct ybString string;
		struct ybSymbol symbol;
		struct ybCons   cons;
		struct ybFctBuiltin fctBuiltin;
		//struct ybFctUser fctUser;
	} u;
};

// prototypen
OBJ newYbNil();
OBJ newYbError(char *);
OBJ newYbInteger(long);
OBJ newYbString(char *);
OBJ newYbSymbol(char *);
OBJ newYbCons(OBJ, OBJ);
OBJ newYbFctBuiltin(char *, ybFctPtr);



void ybThrowError(int, const char *, ...);





#endif /* GLOBAL_H_ */
