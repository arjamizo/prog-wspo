/*
 ============================================================================
 Name        : pmk_producent.c
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
		printf("przed\n");
		mq = mq_open("/MQ_pmk", O_RDWR, 0666, &attr);
		printf("po\n");
		if (mq < 0) {
			perror("MQ1");
			exit(-1);
		}
		msg.produkt_id = id++;
		int res;
		res = mq_send(mq, &msg, sizeof(msg), 10);
		printf("res=%d\n", res);
		//sleep(1);
		printf("PROD said: %d\n", msg.produkt_id);
		mq_close(mq);
		sleep(1);
	}
}
