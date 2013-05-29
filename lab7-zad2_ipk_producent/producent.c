// Program producenta-konsumenta (C) J. Ulasiewicz 2013
// Demonstruje dzialanie pamieci dzielonej i semaforow
// Kompilacja gcc prod_kons.c -o prod_kons -lrt
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <ipk_sharedmem.h>
#include <unistd.h>
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
main(int argc, char *argv[]) {
	int i, stat, k, pid, size, fd, res;
	int KROKI;
	sscanf(argv[1], "%d", &KROKI);
	printf("uruchamianie producenta z %d krokami\n", KROKI);
	bufor_t * wbuf;
	char c;
	// Utworzenie segmentu ---------------------------
	while(1) {
		printf("INIT\n");

	fd = shm_open(BUFNAME, O_RDWR, 0774);
	if (fd == -1) {
		perror("open");
		sleep(1);
		//_exit(-1);
	} else break;
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

	shm_unlink("bufor");
		for (i = 0; i < KROKI; i++) {
			// printf("Producent: %i\n",i);
			printf("Producent - cnt:%d head: %d tail: %d\n", wbuf-> cnt,
					wbuf->head, wbuf->tail);
			sem_wait(&(wbuf->empty));
			sem_wait(&(wbuf->mutex));
			sprintf(wbuf->buf[wbuf->head],"Komunikat %d",i);
			wbuf-> cnt++;
			wbuf->head = (wbuf->head +1) % BSIZE;
			sem_post(&(wbuf->mutex));
			sem_post(&(wbuf->full));
			sleep(1);
		}
	return 0;
}
