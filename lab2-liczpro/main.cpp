/*
 * main.cpp
 *
 *  Created on: 2013-04-11
 *      Author: s184725
 */

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
//#define _UNIX
#ifdef _UNIX

#include <time.h>
#include <sys/time.h>

struct timespec start, end;

double timedelta(struct timespec &t2, struct timespec &t3) {
	return (t3.tv_sec - t2.tv_sec) + (double) (t3.tv_nsec - t2.tv_nsec) * 1e-9;
}

#endif

using namespace std;

typedef int lint;

bool isPrime(lint n) {
	lint max=sqrt(n)+1;
	if(n<2) return false;
	//cout<<"max="<<max<<" dla n="<<n<<endl;
	for (lint i = 2; i < max; ++i) {
		//printf("%d/%d=mod %d\n", n, i, n%i);
		if(n%i==0)
			return false;
	}
	return true;
}

int main(int argc, char **argv) {
	if (argc-1<3) {
		cerr<<"za malo argumentow, \n"<<argv[0]<<" "<<"poczatek przedzialu:int koniec_przedzialu:int liczba_rdzeni:int";
		return -1;
	}
	int P,K,N;
	sscanf(argv[1], "%d", &P);
	sscanf(argv[2], "%d", &K);
	//if(P>K) std::swap(P,K);
	sscanf(argv[3], "%d", &N);
	cout<<P<<" "<<K<<" "<<N<<endl;
#ifdef _UNIX
	clock_gettime(CLOCK_MONOTONIC,  &start);
#endif
	int t1=time(NULL);
	clock_t start = clock();
	for (int i = 0; i < N; ++i) {
		if(fork()==0) {
			//cout<<i<<" podprocees\n";
			int pp,kk;
			pp=(P+(K-P)*i/N);
			kk=(P+(K-P)*(i+1)/N);
			stringstream p; p<<pp;
			stringstream k; k<<kk;
			//exec("licz",);
			//cout<<"liczymy na przedziale "<<p.str()<<"-"<<k.str()<<endl;
			int cnt=0;
			for (int i = pp; i <= kk; ++i) {
				if(isPrime(i)) {
					++cnt;
//					cout<<"PIERWSZA! "<<i<<endl;
				}
			}
			return cnt;
			exit(-1); //-1 indicates error!
		}
	}
	int ile=0;
	for (int i = 0; i < N; ++i) {
		int status;
		wait(&status);
		//cout<<WEXITSTATUS(status)<<endl;
		ile+=WEXITSTATUS(status);
	}
	//ile/=2;
	clock_t end = clock();
	int t2=time(NULL);
#ifdef _UNIX
	clock_gettime(CLOCK_MONOTONIC,  &end);
#endif
	cerr<<"RAPORT: miedzy "<<P<<" a "<<K<<" jest "<<ile<<" liczb pierwszych. "<<N<<" rdzeni liczylo to "
			<<((double)(end - start) / CLOCKS_PER_SEC)<<" sekund"<<endl;
}
