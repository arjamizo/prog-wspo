// Program producenta-konsumenta (C) J. Ulasiewicz 2013
// Demonstruje dzialanie pamieci dzielonej i semaforow
// Kompilacja gcc prod_kons.c -o prod_kons -lrt
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <ipk_sharedmem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>     /* atexit */
#define BSIZE 4
#define ITISMOTHERPROCESS 1
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

bufor_t * wbuf;

void exiting() {
	printf("konczenie!\n");
	shm_unlink("bufor22");
	sem_close(&(wbuf->mutex));
	sem_close(&(wbuf->empty));
	sem_close(&(wbuf->full));
}

int main(int argc, char *argv[]) {
	int i, stat, k, pid, size, fd, res;

	char c;
	// Utworzenie segmentu ---------------------------
	shm_unlink(BUFNAME);
	fd = shm_open(BUFNAME, O_RDWR | O_CREAT, 0774);
	if (fd == -1) {
		perror("open");
		_exit(-1);
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
	// Inicjacja obszaru --------------------------------
	wbuf-> cnt = 0;
	wbuf->head = 0;
	wbuf->tail = 0;
	if (sem_init(&(wbuf->mutex), 1, 1)) {
		perror("mutex");
		_exit(0);
	}
	if (sem_init(&(wbuf->empty),1,BSIZE)) {
		perror("empty");
		_exit(0);
	}
	if (sem_init(&(wbuf->full), 1, 0)) {
		perror("full");
		_exit(0);
	}
	int max;
	for (i = 0; i < (max=120); ++i) {
		printf("czekanie %d/%d\n", i, max);
		sleep(1);
	}
	atexit(exiting);
	return 0;
}
