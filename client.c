// Client Program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct sockaddr_in server;
char *deststr;
char buf[256];
int sock;
FILE *fp;

void signalHandler(int signal);

int main(int argc, char *argv[]) {
	// Check command line arguments (address)
	if (argc != 2) {
		printf("Usage : %s dest\n", argv[0]);
		return 1;
	}

	// Set destination address
	deststr = argv[1];

	//Set signal handler
	if (signal(SIGINT, signalHandler) == SIG_ERR) {
		printf("Failed to set signal handler\n");
		exit(1);
	}

	// Open log file
	if((fp = fopen("log.txt", "w")) == NULL) {
		printf("Failed to open file\n");
		exit(1);
	}

	// Generate socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		return 1;
	}

	// Ser socket
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	server.sin_addr.s_addr = inet_addr(deststr);

	// Connect to the server
	connect(sock, (struct sockaddr *)&server, sizeof(server));

	// Set message to buf
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "This is a client!\n");

	// Send message to the server
	int n = write(sock, buf, (int)strlen(buf));
	if (n < 0) {
		perror("write");
		return 1;
	}

	// Receive message from the server
	while (n > 0) {
		memset(buf, 0, sizeof(buf));
		n = read(sock, buf, sizeof(buf));
		if (n < 0) {
			perror("read");
			return 1;
		}

		write(1, buf, n);

		// Write to log file
		fprintf(fp, "%s", buf);
}

	// Close socket
	close(sock);

	// Save log file
	fclose(fp);

	return 0;
}

void signalHandler(int signal) {
	printf("\nCatch signal -> %d\n", signal);

	// Set message to buf
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "Disconnect!\n");

	// Send message to the server
	int n = write(sock, buf, (int)strlen(buf));
	if (n < 0) {
		perror("write");
	}

	//End TCP session
	close(sock);

	//Save log file
	fclose(fp);

	exit(1);
}
