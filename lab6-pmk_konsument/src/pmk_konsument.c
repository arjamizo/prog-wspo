/*
 ============================================================================
 Name        : pmk_konsument.c
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
int main() {
	printf("cze\n");
	int i;
	struct mq_attr attr;
	// Ustalenie atrybutw kolejki ----------------
	attr.mq_msgsize = sizeof(msg);
	attr.mq_maxmsg = 8;
	for (i = 0; i < 100; ++i) {
		mqd_t mq;
		mq = mq_open("/MQ_pmk", O_RDWR, 0666, &attr);
		if (mq < 0) {
			perror("MQ1");
			exit(-1);
		}
		msg.produkt_id = id++;
		int res;
		int pr;
		res = mq_receive(mq, &msg, sizeof(msg), &pr);
		mq_close(mq);
		//res = mq_send(mq, &msg, sizeof(msg), 10);
		printf("res=%d\n", res);
		//sleep(1);
		printf("KONSUMENT SKONSUMOWAL said: %d\n", msg.produkt_id);
		sleep(1);
	}
}
