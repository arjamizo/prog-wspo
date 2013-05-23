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

#define SIZE 80
typedef struct {
	int type;
	// Typ komunikatu
	char text[SIZE]; // Tekst komunikatu
} msg_tp;

struct MSG {
	int p;
	int k;
	int proc;
	int N;
	int suma;
};
struct MSG msg;

int main(int argc, char **argv, char **env) {

	if (argc < 3 + 1) {
		perror("za malo argumentow");
		exit(-100);
	}
	int P, K, N; //poczatek zakresu, koniec zakresu, liczba procesow, identyfikator procesora tego procesu
	sscanf(argv[1], "%d", &P);
	sscanf(argv[2], "%d", &K);
	//if(P>K) std::swap(P,K);
	sscanf(argv[3], "%d", &N);
	printf("uruchomiono MASTERA dla P=%d K=%d N=%d\n", P, K, N);
	int i;
	int res;
	//msg_tp msg;
	struct mq_attr attr;
	// Ustalenie atrybutw kolejki ----------------
	attr.mq_msgsize = sizeof(msg);
	attr.mq_maxmsg = 8;

	mq_unlink("/MQ1");
	mq_unlink("/MQ2");
	for (i = 0; i < N; i++) {
		//sleep(1);
		printf("RUNNING\n");
		if (fork() == 0) {
			char aP[10], aK[10], aN[10], an[10];
			sprintf(aP, "%d", P);
			sprintf(aK, "%d", K);
			sprintf(aN, "%d", N);
			sprintf(an, "%d", i);
			printf("running with %s %s %s %s\n", aP, aK, aN, an);
			msg.p = P;
			msg.k = K;
			msg.proc = i;
			msg.N = N;
			int res;
			printf("przed\n");
			mqd_t mq2;
			mq2 = mq_open("/MQ2", O_RDWR | O_CREAT, 0666, &attr);
			if (mq2 < 0) {
				perror("mq2");
				exit(-1);
			}
			res = mq_send(mq2, &msg, sizeof(msg), 10);
			if (res < 0) {
				perror("mq2");
				exit(-1);
			}
			mq_close(mq2);
			printf("po\n");
			system("pwd");
			sleep(1);
			//res=execl("/bin/date", "date", 0, 0);
			res = execl("../getNumOfPrimes/Debug/getNumOfPrimes",
					"getNumOfPrimes"
					//,aP,aK,aN,an
					, NULL);
			printf("res execl=%d %s\n", res, strerror(errno));
			//if(res) {
			printf("Oh dear, something went wrong with execl()! %s\n",
					strerror(errno));
			//}
			exit(-11);
		} else {
		}
	}
	int sumtot = 0;
	for (i = 0; i < N; i++) {
		printf("odbieranie\n");
		int status;
		wait(&status);
		printf("counter exited\n");
		//sprintf(msg.text, "MASTER sends 1 komunikat %d", i);
		//msg.suma=i;
		// Wysanie komunikatu ----------------
		mqd_t mq;
		mq = mq_open("/MQ1", O_RDWR | O_CREAT, 0666, &attr);
		// Utworzenie kolejki komunikatow ------------
		if (mq < 0) {
			perror("ERROR! MQ1 open in master");
			exit(-2);
		}
		int pr;
		res = mq_receive(mq, &msg, sizeof(msg), &pr);
		if (res == -1) {
			printf("res=%d MASTER RECV %d\n", res, msg.suma);
			perror("MRECEIVE");
			exit(-111);
		}
		printf("res=%d MASTER RECV %d\n", res, msg.suma);
		sumtot += msg.suma;
		mq_close(mq);
	}
	printf("TOTAL=%d\n", sumtot);
}
