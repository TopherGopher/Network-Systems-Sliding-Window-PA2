/* GBNclient.c */
/* This is a sample UDP client/sender using "sendto_.h" to simulate dropped packets.  */
/* This code will not work unless modified. */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>   /* memset() */
#include <sys/time.h> /* select() */
#include <signal.h>
#include <unistd.h>
#include "sendto_.h"

#define MAX_FILE_SIZE 20000
#define MAX_FILE_CHUNK_SIZE 10

int main(int argc, char *argv[]) {
    
	/* check command line args. */
	if(argc<7)
	{
		printf("usage : %s <server_ip> <server_port> <error rate> <random seed> <send_file> <send_log> \n", argv[0]);
		exit(1);
	}

	/* Note: you must initialize the network library first before calling sendto_().  The arguments are the <errorrate> and <random seed> */
	init_net_lib(atof(argv[3]), atoi(argv[4]));
	printf("error rate : %f\n",atof(argv[3]));

	/* socket creation */
	int sd;
	if((sd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		printf("%s: cannot create socket \n",argv[0]);
		exit(1);
	}

	/* get server IP address (input must be IP address, not DNS name) */
	struct sockaddr_in remoteServAddr;
	bzero(&remoteServAddr,sizeof(remoteServAddr));               //zero the struct
	remoteServAddr.sin_family = AF_INET;                 //address family
	remoteServAddr.sin_port = htons(atoi(argv[2]));      //sets port to network byte order
	remoteServAddr.sin_addr.s_addr = inet_addr(argv[1]); //sets remote IP address
	printf("%s: sending data to '%s:%s' \n", argv[0], argv[1], argv[2]);

	/* Call sendto_ in order to simulate dropped packets */
	int nbytes;
	char msg[] = "send this";
	nbytes = sendto_(sd, msg, strlen(msg),0, (struct sockaddr *) &remoteServAddr, sizeof(remoteServAddr));
}

/* Reads a file into active memory. */
char*
read_file_into_memery(char* filename) {
	char full_file[MAX_FILE_SIZE]; //todo - dynamic memory?
	char chunk[MAX_FILE_CHUNK_SIZE];
	FILE *file_pointer;

	bzero(full_file, sizeof(full_file));
	bzero(chunk, sizeof(chunk));


	file_pointer = fopen(filename, "r");
	if (file_pointer == NULL)
	{
	  fprintf(stderr, "Cannot open input file %s! Sucks to be you nerd...\n", filename);
	  exit(EXIT_FAILURE);
	}
	
	int count=0;
	while(!feof(file_pointer))
	{
		fread(chunk, MAX_FILE_CHUNK_SIZE, 1, file_pointer);
		//append chunk to file
		strncat(full_file, chunk, MAX_FILE_CHUNK_SIZE);
		bzero(chunk, MAX_FILE_CHUNK_SIZE);
		count++;
	}
	fclose(file_pointer);
	return full_file;
}

