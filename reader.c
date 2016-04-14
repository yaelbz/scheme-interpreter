/*
 * reader.c
 *
 *  Created on: 13.04.2016
 *      Author: yael
 */
#include <stdio.h>
#include "scheme.h"

static int
nextChar() {
    return getchar();
}

static int
isWhiteSpace(char ch) {
    return (ch == ' ') || (ch == '\t')
           ||  (ch == '\n') || (ch == '\r');
}

static int
isDigit(char ch) {
    return (ch >= '0') && (ch <= '9');
}

int
skipSpaces() {
    int ch;

    do {
        ch = nextChar();
    } while (isWhiteSpace(ch));
    return ch;
}

OBJ
readList() {
    printf("unimpl.");
    abort();
}

OBJ
readNumber(int firstChar) {
    scheme_int64 iVal = 0;
    int ch;
    OBJ retVal;

    iVal = firstChar - '0';
    while ( isDigit( ch = nextChar())) {
        iVal = iVal * 10 + (ch - '0');
    }
    retVal = newInteger(iVal);
    return retVal;
}

OBJ
readSymbol() {
    printf("unimpl.");
    abort();
}

OBJ
readString() {
    printf("unimpl.");
    abort();
}

OBJ
read() {
    int ch;

    ch = skipSpaces();
    if (ch == '(') {
        return readList();
    }
    if (ch == '"') {
        return readString();
    }
    if (isDigit(ch)) {
        return readNumber(ch);
    }
    return readSymbol();
}



