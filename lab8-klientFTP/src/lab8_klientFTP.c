/*
 ============================================================================
 Name        : lab8_klientFTP.c
 Author      : azochniak
 Version     :
 Copyright   : My copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
// Proces odbierajacy komunikaty - wysyla udp_cli
// Wspolpracuje z udp_cli
// Kompilacja gcc udp_serw.c -o udp_serw -lrt
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define BUFLEN 80
#define KROKI 10
#define PORT 9950
#define SRV_IP "127.0.0.1"
typedef struct {
	int typ;
	int fh;
	int len;
	char buf[BUFLEN];
} msgt;
void blad(char *s) {
	perror(s);
	_exit(1);
}
int main(void) {
	struct sockaddr_in adr_moj, adr_serw, adr_x;
	int s, i, snd, rec;
	unsigned int slen = sizeof(adr_serw), blen = sizeof(msgt);
	char buf[BUFLEN];
	msgt msg;

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0)
		blad("socket");
	printf("Gniazdko %d utworzone\n", s);
	memset((char *) &adr_serw, 0, sizeof(adr_serw));
	adr_serw.sin_family = AF_INET;
	adr_serw.sin_port = htons(PORT);
	if (inet_aton(SRV_IP, &adr_serw.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() failed\n");
		_exit(1);
	}

	enum MSGTYPE {
		OPENWR = 0,
		OPENRD = 1,
		WRITE = 2,
		READ = 3,
		OPENDIR = 4,
		READDIR = 5,
		FCLOSE = 6,
		STAT = 7,
		FSTAT = 8,
		WELCOME = 9
	};
	const *CMDS[20] = { "OPEN", "OPENRD", "WRITE", "READ", "OPENDIR",
			"READDIR", "FCLOSE", "STAT", "FSTAT", "WELCOME" };

	sprintf(msg.buf, "WELCOME");
	msg.typ = WELCOME;
	snd = sendto(s, &msg, blen, 0, (struct sockaddr *) &adr_serw, slen);
	if (snd < 0)
		blad("sendto()");
	printf("wyslano\n");

	again:
//	while (1) {
		rec = recvfrom(s, &msg, blen, 0, (struct sockaddr *) &adr_serw, &slen);
		if (rec < 0)
			blad("recvfrom()");
//		if (rec != 0)
//			break;
//	}
	printf("odebralem komunikat\n");
	printf("Odebrano komunikat z %s:%d reclen %d\n Typ: %d %s\n", inet_ntoa(
			adr_serw.sin_addr), ntohs(adr_serw.sin_port), rec, msg.typ, msg.buf);
	char arg[100];
	char cmd[100];
	char arg2[100];
	printf("PLESE FEED ME WITH S**T\n");
	int ile=scanf("%s %s %s", cmd, arg, arg2);
	msg.buf[0]=0;
	strcat(msg.buf, cmd);
	strcat(msg.buf, " ");
	strcat(msg.buf, arg);
	strcat(msg.buf, " ");
	strcat(msg.buf, arg2);
	printf("wczytano %d pol\n", ile);
	msg.typ = 100;
	for (i = 0; i < 10; ++i)
		if (strcmp(CMDS[i], cmd)==0)
			msg.typ = i;
	//msg.typ=WELCOME;
	if (msg.typ != 100) {
		snd = sendto(s, &msg, blen, 0, (struct sockaddr *) &adr_serw, slen);
		if (snd < 0)
			blad("sendto()");
		goto again;
	}

	printf("Koniec");
	close(s);
	return 0;
}
