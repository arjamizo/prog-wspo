/*
 ============================================================================
 Name        : pmk_magazyn.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
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
#include <mqueue.h>

struct MSG {
	int produkt_id;
};
struct MSG msg;

int id = 0;
int main(int argc, char **argv) {
	int len;
	if (argc < 2) {
		perror("not enough arguments\n");
		exit(-1);
	}
	sscanf(argv[1], "%d", &len);
	printf("kolejka dlugosci LEN=%d\n", len);
	int i;
	struct mq_attr attr;
	// Ustalenie atrybutw kolejki ----------------
	attr.mq_msgsize = sizeof(msg);
	attr.mq_maxmsg = 8;
	unlink("/MQ_pmk");
	mqd_t mq;
	mq = mq_open("/MQ_pmk", O_RDWR | O_CREAT, 0666, &attr);
	//int i;
	for (i = 0; i < 100; ++i) {
		printf("%d-ta/100 sekunda pracy\n", i);
		sleep(1);
	}
	mq_close(mq);
	unlink("/MQ_pmk");
}
