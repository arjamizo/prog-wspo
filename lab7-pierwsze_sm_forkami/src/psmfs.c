/*
 ============================================================================
 Name        : psmfs - primes, shared memory, using ForkS!
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// Kompilacja gcc pam-dziel.c -o pam-dziel -lrt
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#define SIZE 60

																							#define Jesli if(
																							#define to )

bool isPrime(int n) {
	//return true;
	int i, max = n - 1;
	//max=
	//sqrt(n);
	//printf("%f",(float)sqrt(4));
	if (n < 2)
		return false;
	//cout<<"max="<<max<<" dla n="<<n<<endl;
	for (i = 2; i < max; ++i) {
		//printf("%d/%d=mod %d\n", n, i, n%i);
		if (n % i == 0)
			return false;
	}
	//printf("%d jest pierwsza\n", n);
	return true;
}



typedef struct {
	int p;
	int k;
	int cnt; //cnt mowi ile liczb pierwzzych jest w zakresie o <p,k>
} row_t;

// Rozmiar bufora
typedef struct {
	int wymiar;
	row_t dane[SIZE];
} buf_t;
int main(int argc, char *argv[]) {
	int i, stat, pid, k, res;
	buf_t *buf;
	char name[16];
	int fd; // Deskryptor segmentu
	int P,K,N;
	int suma=0;
	if (argc-1<3) {
		//cerr<<"za malo argumentow, \n"<<argv[0]<<" "<<"poczatek przedzialu:int koniec_przedzialu:int liczba_rdzeni:int";
		perror("za malo argumentow");
		return -1;
	}
	sscanf(argv[1], "%d", &P);
	sscanf(argv[2], "%d", &K);
	//if(P>K) std::swap(P,K);
	sscanf(argv[3], "%d", &N);
	//stringstream p; p<<pp;
	//stringstream k; k<<kk;

	printf("uruchomiono MASTERA dla P=%d K=%d N=%d\n", P, K, N);
	strcpy(name, "Bufor");
	shm_unlink(name);
	// Utworzenie segmentu pamieci ---------------------
	if ((fd = shm_open(name, O_RDWR | O_CREAT, 0664)) == -1) {
		perror("shm_open");
		exit(-1);
	}
	printf("fh = %d\n", fd);
	// Okreslenie rozmiaru obszaru pamieci ---------------
	res = ftruncate(fd, sizeof(buf_t));
	if (res < 0) {
		perror("ftrunc");
		return 0;
	}
	// Odwzorowanie segmentu fd w obszar pamieci procesow
	buf = (buf_t *) mmap(0, sizeof(buf_t), PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0);
	if (buf == NULL) {
		perror("mmap");
		exit(-1);
	}
	printf("start\n");
	buf->wymiar = N;
	// Proces potomny P2 - pisze do pamieci wspolnej -----
	//int i;
	for (i = 0; i < N; ++i) {
		if (fork()==0) {
			int pp,kk;
			int cnt=0;
			int j;
			pp=(P+(K-P)*i/N);
			kk=(P+(K-P)*(i+1)/N);
			printf("Uruchamianie %d proces dla %d %d\n", i, pp, kk);
			for (j = pp; j <= kk; ++j) {
				Jesli isPrime(j) to cnt++;
			}
			buf->dane[i].p=pp;
			buf->dane[i].k=kk;
			buf->dane[i].cnt=cnt;
			printf("udalo sie zapisac %d %d %d=?=%d\n", buf->dane[i].p, buf->dane[i].k,buf->dane[i].cnt, cnt);
			exit(0);
		}
	}
	for (i = 0; i < N; ++i) {
		pid = wait(&stat);
		printf("zakonczyl prace %d %s 0\n", pid, "ze statusem");
	}
	printf("Raport: \n");
	for (i = 0; i < N; ++i) {
		printf("%5d %5d %5d %5d\n", i, buf->dane[i].p, buf->dane[i].k, buf->dane[i].cnt);
		suma+=buf->dane[i].cnt;
	}
	printf("Raport koniec! (z punktu widzenia dyskretnej maszyny jaka jest komputer, liczb pierwszych jest %d, \nzapisać do pdf (T/N)? \n", suma);
	return 0;
}
