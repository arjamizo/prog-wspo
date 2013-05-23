/*
 ============================================================================
 Name        : getNumberOfPrimes.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdbool.h>
#include <time.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h> //remove()
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/times.h>
#include <sys/types.h>
#include <stdint.h> //intmax_t
#include <math.h>


bool isPrime(double n) {
	int i,max;
	max=sqrt(n)+1;
	if(n<2) return false;
	//cout<<"max="<<max<<" dla n="<<n<<endl;
	for (i = 2; i < max; ++i) {
		//printf("%d/%d=mod %d\n", n, i, n%i);
		if(n%i==0)
			return false;
	}
	return true;
}

int main(int argc, char **argv, char *env) {

	int P,K,N,n; //poczatek zakresu, koniec zakresu, liczba procesow, identyfikator procesora tego procesu
	sscanf(argv[1], "%d", &P);
	sscanf(argv[2], "%d", &K);
	//if(P>K) std::swap(P,K);
	sscanf(argv[3], "%d", &N);
	sscanf(argv[4], "%d", &n);
	printf("uruchomiono dla P=%d K=%d N=%d n=%d\n", P, K, N, n);
	int pp,kk;
	pp=(P+(K-P)*n/N);
	kk=(P+(K-P)*(n+1)/N)-1;

	int cntr=0;
	int i;
	for (i = pp; i <= kk; ++i) {
		if(isPrime(i)) {
			cntr++;
		}
	}

	return cntr;
}
