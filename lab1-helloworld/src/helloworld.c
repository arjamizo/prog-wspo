/*
 ============================================================================
 Name        : helloworld.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
struct {int a;} a;
int main(void) {
	int i;
	for (i = 0; i < 10; ++i) {
		char *text="!!!Hello World %d !!!\n";
		printf(text, i); /* prints !!!Hello World!!! */
		a.a=i;
		sleep(1);
	}
	return EXIT_SUCCESS;
}
