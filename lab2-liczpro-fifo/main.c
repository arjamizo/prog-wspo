/*
 * main.c
 *
 *  Created on: 2013-04-25
 *      Author: s184725
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




static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;

void
start_clock()
{
    st_time = times(&st_cpu);
}

/* This example assumes that the result of each subtraction
   is within the range of values that can be represented in
   an integer type. */
void
end_clock(char *msg)
{
    en_time = times(&en_cpu);

    fputs(msg,stdout);
    printf("Real Time: %jd, User Time %jd, System Time %jd\n",
        (intmax_t)(en_time - st_time),
        (intmax_t)(en_cpu.tms_utime - st_cpu.tms_utime),
        (intmax_t)(en_cpu.tms_stime - st_cpu.tms_stime));
}



struct {
	int poczatek;
	int koniec;
	int suma;
} msg;

typedef long int lint;

bool isPrime(lint n) {
	lint i,max;
	max=sqrt(n)+1;
	if(n<2) return false;
	//cout<<"max="<<max<<" dla n="<<n<<endl;
	for (i = 2; i < max; ++i) {
		//printf("%d/%d=mod %d\n", n, i, n%i);
		if(n%i==0)
			return false;
	}
	return true;
}

bool file_exists(const char * filename)
{
	FILE * file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[], const char **env) {

	bool use_fifo;
	use_fifo=false;

	int i;
	for (i = 0; i < argc; ++i) {
		if(strcmp(argv[i],"--use-fifo")==0)
			use_fifo=true;
	}
	printf("uzywanie %s\n", use_fifo?"fifo":"file");
//	for (i = 0; i < 100; ++i) {
//		if(isPrime(i+2))
//			printf("%i jest pierwsza\n", i+2);
//	}
	if(argc-1<3) {
		printf("za malo argumentow\n");
	}
	system("whoami");
	system("pwd");
	const char *fnme="a2.txt";
	system("ls -l a2.txt");
	mode_t oldmask;
	oldmask=umask(S_IRUSR);
	umask(oldmask);
	printf("UMASK=%o\n",oldmask);
	if(!use_fifo) printf("efekt usuniecia=%d\n", (int)remove(fnme));
	int P,K,N;
	sscanf(argv[1], "%d", &P);
	sscanf(argv[2], "%d", &K);
	//if(P>K) std::swap(P,K);
	sscanf(argv[3], "%d", &N);
	printf("uruchomiono dla P=%d K=%d N=%d\n", P, K, N);
	int f;
	if(use_fifo) {
		f=mkfifo(fnme, 0666);
		if(f!=0)
		{
			perror("blad kolejki\n");
			exit(-1);
		}
	}
	int t1;
	t1=time(NULL);
	clock_t start;
	start = clock();
	start_clock();
	printf("proces glowny\n");
	for (i = 0; i < N; ++i) {
		if(fork()==0) {
			//cout<<i<<" podprocees\n";
			if(!use_fifo) f=open(fnme, O_RDWR | O_CREAT | O_APPEND, 0666 & ~oldmask);
			else f=open(fnme, O_RDWR);//RDONLY byc nie moze, czeka az ktos bedzie chcial czytac
			if(f < 0) {
				perror("open");
				return 0;
			}
			printf("tutaj plik=%s\n", "CZE");
			int pp,kk;
			pp=(P+(K-P)*i/N);
			kk=(P+(K-P)*(i+1)/N)-1;
			//if(file_exists("..."))
			if(f>0) {
				printf("{\n%8s przez %d\n", "LOCK", (int)getpid());
				if(!use_fifo) lockf(f, F_LOCK, 0); //man 2 fopen: or to the end of file if size is 0
				printf("id otwartego pliku %d\n", f);
				printf("proces %d liczy na przedziale <%d,%d)\n", (int)getpid(), pp, kk);
				msg.poczatek=pp;
				msg.koniec=kk;
				msg.suma=0;
				int n;
				for (n = msg.poczatek; n < msg.koniec; ++n) {
					if(isPrime(n)) msg.suma++;
				}
				printf("zapisano {poczatek: %d; koniec: %d, suma: %d}\n", msg.poczatek, msg.koniec, msg.suma);
				write(f, &msg, sizeof msg); //man 2 write
				printf("%8s przez %d\n}\n", "UNLOCK", (int)getpid());
				if(!use_fifo) lockf(f, F_ULOCK, 0);
				if(!use_fifo)
					close(f);
			} else {printf("BLAD FOPEN\n");}
			exit(0);
		}
	}
	int status;
	for (i = 0; i < N; ++i) {
		wait(&status);//) exit(-1);
	}
	int tt;
	end_clock("END.");
	printf("wszystkie sie wykonaly, t=%d\n", tt=(time(NULL)-t1));
//	if(!use_fifo)
	f=open(fnme, O_RDWR); //RDONLY byc nie moze, czeka az ktos bedzie chcial czytac
	if(f<0) {
		perror("open przed raportem");
		exit(-2);
	}
	for (i = 0; i < N; ++i) {
		read(f, &msg, sizeof msg);
		printf("wczytano {poczatek: %d; koniec: %d, suma: %d}\n", msg.poczatek, msg.koniec, msg.suma);
	}
//	if(!use_fifo)
		close(f);
	//if(use_fifo)
		unlink(f);

	FILE *raport;
	char prepend[200];
	sprintf(prepend, "");
	if(raport=fopen("raport.txt", "r")==-1) snprintf(prepend, sizeof prepend, "%s", "P\tK\tszerokosc\tilosc_procesor\tsekundy\treal_time\n");
	raport=fopen("raport.txt", "a+");
	fprintf(raport, "%s%d\t%d\t%d\t%d\t%d\t%d\n", prepend, P, K, K-P, N, tt, (intmax_t)(en_time - st_time));
	fclose(raport);
	return 0;
}
