/*
 * global.c
 *
 *  Created on: 02.10.2016
 *      Author: yael
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "global.h"


//TODO: declare enum with errors in global header
void ybError(int ybErrNum, const char *format, ...) {
	va_list args;
	va_start(args, format);

	fprintf(stderr, "#### ERROR 0x%08x:\n", ybErrNum);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n####\n\n");
	fflush(stderr);

    va_end(args);

    //TODO: handle error, return to main execution
    exit(ybErrNum);
}
