/*
 * test.c
 *
 *  Created on: 13.04.2016
 *      Author: martin
 */
#include <stdio.h>


void test() {
	int x = 3;
	int *p;

	p = &x;


	int y = x+1;
	int z = *p+1;

	printf("x:  %d\n", x);
	printf("*p: %d\n", *p);
	printf("p:  %d = 0x%08X\n", (int)p, (int)p);
	printf("&x: %d = 0x%08X\n", (int)&x, (int)&x);
	printf("y:  %d\n", y);
	printf("z:  %d\n", z);
	(*p)++;
	(*p)++;
	printf("x:  %d\n", x);



}
