/*
 * main.c
 *
 *  Created on: 13.04.2016
 *      Author: yael
 */
#include <stdio.h>
#include "scheme.h"


main() {

    int x = 10;



    printf("Welcome to Scheme\n");
    for (;;) {
        OBJ o;

        printf("> ");
        o = read();
        printf("\n");

    }
}

