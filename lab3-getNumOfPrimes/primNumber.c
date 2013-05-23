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

int ile(int P, int K, int N, int n) {
	int pp, kk;
	pp = (P + (K - P) * n / N);
	kk = (P + (K - P) * (n + 1) / N);

	int cntr = 0;
	int i;
	for (i = pp; i <= kk; ++i) {
		if (isPrime(i)) {
			cntr++;
		}
	}
	printf("miedzy %d a %d jest %d liczb pierwszych\n", pp, kk, cntr);
	return cntr;
}

struct MSG {
	int p;
	int k;
	int proc;
	int N;
	int suma;
};
struct MSG msg;
int main(int argc, char **argv, char **env) {
	if (argc >= 4) {
		int P, K, N, n; //poczatek zakresu, koniec zakresu, liczba procesow, identyfikator procesora tego procesu
		sscanf(argv[1], "%d", &P);
		sscanf(argv[2], "%d", &K);
		//if(P>K) std::swap(P,K);
		sscanf(argv[3], "%d", &N);
		sscanf(argv[4], "%d", &n);
		printf("uruchomiono dla P=%d K=%d N=%d n=%d\n", P, K, N, n);

		return -1 + 0 * ile(P, K, N, n);
	} else {
		int i;

		//for(i=0; i < 6 ;i++)
		{
			//sprintf(msg.text,"Proces 1 komunikat %d",i);
			// Odbir komunikatu ----------------

			struct mq_attr attr;
			// Ustalenie atrybutw kolejki ----------------
			attr.mq_msgsize = sizeof(msg);
			attr.mq_maxmsg = 8;
			int res;

			{
				mqd_t mq2;
				mq2 = mq_open("/MQ2", O_RDONLY | O_CREAT, 0666, &attr);
				int pr;
				res = mq_receive(mq2, &msg, sizeof(msg), &pr);
				if(0)if (res<0) {
					perror("MRECEIVE");
					exit(-111);
				}
				printf("ODEBRANO KOMUNIKAT\n");
				mq_close(mq2);
			}
			printf("uruchomiono dla P=%d K=%d N=%d n=%d\n", msg.p, msg.k,
					msg.N, msg.proc);

			//msg.suma = ile(1, 100, 4, 0);
			msg.suma = ile(msg.p, msg.k, msg.N, msg.proc);
			//msg.suma=100;
			{
				mqd_t mq;
				mq = mq_open("/MQ1", O_WRONLY | O_CREAT, 0666, &attr);
				if (mq < 0) {
					perror("MQ1");
					exit(-1);
				}
				res = mq_send(mq, &msg, sizeof(msg), 10);
				printf("res=%d\n", res);
				//sleep(1);
				printf("SLAVE said: %d\n", msg.suma);
				mq_close(mq);
			}

		}
	}
}
