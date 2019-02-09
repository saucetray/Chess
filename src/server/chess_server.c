///
/// Chess Server Implementation
/// Author: Justin Sostre
///

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>

#define PORT 12000

int main(int argc, char const *argv[]) {

	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; // Making this socket available to all available interfaces.
	address.sin_port = htons(PORT); // Attaching the socket to defined PORT 
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	if (listen(server_fd, 3) < 0) { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) { 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	char usr[40] = {0};
    char pswd[40] = {0};

    printf("Reading usr\n");
    if ((valread = read(new_socket, usr, sizeof(usr))) < 0) {
        return 1;
    }
    printf("%s\n", usr);
    printf("Reading pswd\n");
    if ((valread = read(new_socket, pswd, sizeof(pswd)) < 0)) {
        return 1;
    }

    int32_t accepted = 3;
    printf("Okay...\n");
    if (strcmp(usr, "justin") == 0 && strcmp(pswd, "sostre") == 0) {
        printf("Sending accepted");
        send(new_socket, &accepted, sizeof(int32_t), 0);
    }
	// Sending the buffer to the connected client.
	printf("Hello message sent\n"); 

	return 0;

} 
