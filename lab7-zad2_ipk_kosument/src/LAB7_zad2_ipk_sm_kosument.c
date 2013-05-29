/*
 ============================================================================
 Name        : LAB7_zad2_ipk_sm_kosument.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
// Program producenta-konsumenta (C) J. Ulasiewicz 2013
// Demonstruje dzialanie pamieci dzielonej i semaforow
// Kompilacja gcc prod_kons.c -o prod_kons -lrt
#include <ipk_sharedmem.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
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
#define ITISMOTHERPROCESS 0
#define BSIZE 4
// Rozmiar bufora
#define LSIZE 80
// Dlugosc linii
typedef struct {
	char buf[BSIZE][LSIZE];
	int head;
	int tail;
	int cnt;
	sem_t mutex;
	sem_t empty;
	sem_t full;
} bufor_t;
int main(int argc, char *argv[]) {
	int KROKI;
	int i, stat, k, pid, size, fd, res;
	bufor_t * wbuf;
	sscanf(argv[1], "%d", &KROKI);
	printf("uruchamianie kosumenta z %d krokami\n", KROKI);
	char c;
	// Utworzenie segmentu ---------------------------
	while (1) {
		printf("INIT\n");
		printf("%o %o\n", ~umask(), 0774 & ~umask());
		fd = shm_open(BUFNAME, O_RDWR, 0774 & ~umask()); //mask
		if (fd == -1) {
			perror("open");
			//printf("");
			sleep(1);
			//_exit(-1);
		} else
			break;
	}
	printf("fd: %d\n", fd);
	size = ftruncate(fd, sizeof(bufor_t));
	if (size < 0) {
		perror("trunc");
		_exit(-1);
	}
	// Odwzorowanie segmentu fd w obszar pamieci procesow
	wbuf = (bufor_t *) mmap(0, sizeof(bufor_t), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (wbuf == NULL) {
		perror("map");
		_exit(-1);
	}

	for (i = 0; i < KROKI; i++) {
		printf("Konsument - cnt: %d odebrano %s\n", wbuf->cnt,
				wbuf->buf[wbuf->tail]);
		sem_wait(&(wbuf->full));
		sem_wait(&(wbuf->mutex));
		wbuf-> cnt--;
		wbuf->tail = (wbuf->tail + 1) % BSIZE;
		sem_post(&(wbuf->mutex));
		sem_post(&(wbuf->empty));
		sleep(1);
	}

	return 0;
}
