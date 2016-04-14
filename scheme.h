/*
 * scheme.h
 *
 *  Created on: 13.04.2016
 *      Author: yael
 */

#ifndef SCHEME_H_
#define SCHEME_H_

#ifdef __WIN32__
typedef int scheme_int32 ;
typedef long long scheme_int64 ;
#else
typedef int scheme_int32;
typedef long scheme_int64;
#endif

enum tag {
    T_INTEGER,
    T_STRING,
    T_CONS
};

typedef struct schemeObject *OBJ;
// #define OBJ struct schemeObject *

struct schemeInteger {
    enum tag tag;
    scheme_int64 intVal;
};

struct schemeString {
    enum tag tag;
    char *stringVal;
};

struct schemeCons {
    enum tag tag;
    OBJ car;
    OBJ cdr;
};

struct schemeObject {
    union {
        struct schemeInteger integer;
        struct schemeString string;
        struct schemeCons cons;
    } u;
};

#endif /* SCHEME_H_ */
