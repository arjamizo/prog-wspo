/*
 ============================================================================
 Name        : lab8_serwerFTP.c
 Author      : azochniak
 Version     :
 Copyright   : My copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
// Proces wysyla a potem odbiera komunikaty udp
// Wspolpracuje z udp_serw
// Kompilacja gcc udp_cli.c -o udp_cli -lrt
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFLEN 80
#define KROKI 10
#define PORT 9950
#define SRV_IP "127.0.0.1"

typedef struct {
	int typ;
	int fh;
	int ile;
	char buf[BUFLEN];
} msgt;

void blad(char *s) {
	perror(s);
	_exit(1);
}

int soc;
void closeSocket() {
	close(soc);
	soc = 0;
}

int main(int argc, char * argv[]) {
	int fhbuf;
	struct sockaddr_in adr_moj, adr_cli;
	int s, i;
	unsigned int slen = sizeof(adr_cli), blen = sizeof(msgt);
	int snd, rec;
	char buf[BUFLEN];
	msgt msg;

	gethostname(buf, sizeof(buf));
	printf("Host: %s\n", buf);

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0)
		blad("socket");
	printf("Gniazdko %d utworzone\n", s);
	// Ustalenie adresu IP nadawcy
	memset((char *) &adr_moj, 0, sizeof(adr_moj));
	adr_moj.sin_family = AF_INET;
	adr_moj.sin_port = htons(PORT);
	adr_moj.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, (struct sockaddr *) &adr_moj, sizeof(adr_moj)) == -1)
		blad("bind");

	enum MSGTYPE {
		WELCOME = 9,
		OPENWR = 0,
		OPENRD = 1,
		WRITE = 2,
		READ = 3,
		OPENDIR = 4,
		READDIR = 5,
		FCLOSE = 6,
		STAT = 7,
		FSTAT = 8
	};
	const *CMDS[20] = { "OPEN", "OPENRD", "WRITE", "READ", "OPENDIR",
			"READDIR", "FCLOSE", "STAT", "FSTAT", "WELCOME" };

	while (1) {
		rec = recvfrom(s, &msg, blen, 0, (struct sockaddr *) &adr_cli, &slen);
		if (rec < 0)
			blad("recvfrom()");
		//if (rec == 0)
		//	continue;
		printf("odebrano cos\n");
		if (0)
			printf(
					msg.buf,
					"SERWER: Odebrano komunikat z %s:%d reclen %d\n Typ: %d %s\n",
					inet_ntoa(adr_cli.sin_addr), ntohs(adr_cli.sin_port), rec,
					msg.typ, msg.buf);
		char cmd[100], arg[100], arg2[100];
		msg.typ = 100;
		printf("probuje sparsowac komende %s\n", msg.buf);
		sscanf(msg.buf, "%s %s %s", cmd, arg, arg2);
		printf("udalo mi sie wczytac komende: %s z argumentami %s %s\n", cmd, arg, arg2);
		for (i = 0; i < 10; ++i)
			if (strcmp(CMDS[i], cmd)==0)
				msg.typ = i;
		system("pwd");
		switch (msg.typ) {
		case WELCOME:
			sprintf(
					msg.buf,
					"Hi %s talking to me on %d port. I've received %d bytes. \n Typ: %d\n",
					inet_ntoa(adr_cli.sin_addr), ntohs(adr_cli.sin_port), rec,
					msg.typ//, msg.buf
			);
			printf(msg.buf);
			break;
		case OPENWR:
			msg.fh = open((const char*) arg, O_RDWR);
			break;
		case OPENRD:
			msg.fh = open((const char*) arg, O_RDONLY);
			break;
		case WRITE:
			fhbuf = msg.fh;
			printf("probuje zapisac do pliku %s tresc %s\n", arg, arg2);
			msg.fh = open((const char*) arg, O_RDWR | O_CREAT);
			msg.ile = write(msg.fh, (const char*) arg2, BUFLEN);
			close(msg.fh);
			msg.fh = fhbuf;
			break;
		case READ:
			fhbuf = msg.fh;
			msg.fh = open((const char*) arg, O_RDONLY);
			printf("KTOS CHCE PLIK Z KOMENDY (POLICJI) %s\n", msg.buf);
			if (msg.fh > 0){
				msg.ile = read(msg.fh, (const char*) msg.buf, BUFLEN);
				printf("odczytalem z pliku %s\n", msg.buf);
			}
			else
				perror("ERROR IN CASE READ!");
			close(msg.fh);
			msg.fh = fhbuf;
			break;
		case OPENDIR:
			msg.fh = opendir(arg);
			break;
		case READDIR:
//			fhbuf = msg.fh;
//			DIR *dir = opendir(arg);
//			do {
//		        errno = 0;
//		        if ((dp = readdir(dirp)) != NULL) {
//		            if (strcmp(dp->d_name, arg) != 0)
//		                continue;
//
//
//		            (void) printf("found %s\n", arg);
//		            (void) closedir(dirp);
//		                return;
//
//
//		        }
//				msg.fh = fhbuf;
			break;
		case FCLOSE:
			close(msg.fh);
		case STAT:
			perror("BRAK KOMENDY... \7\7POLICJI\n");
			break;
		case FSTAT:
			perror("BRAK KOMENDY... \7\7POLICJI\n");
			break;
		}
		snd = sendto(s, &msg, blen, 0, (struct sockaddr *) &adr_cli, slen);
		if (snd < 0)
			blad("sendto()");
		else {
			printf("wyslano");
		}
	}

	atexit(closeSocket);
	return 0;
}
