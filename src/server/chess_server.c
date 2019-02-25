// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12000

pthread_t *threads;

typedef struct {
    int id;
    int socket;
} Socket_Info;


void  *handle_player(void *args) {
    
    printf("Handling the player.\n");
    fflush(stdout);
    sleep(2);
    int valread;
    char buffer[1024] = {0};
    char *hello = "Dog.";
    Socket_Info *info = (Socket_Info*) args;
    valread = read(info->socket, buffer, 1024); 
    printf("%s\n",buffer); 
    
    // Sending the buffer to the connected client.
    send(info->socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 

    return 0;
}


int main() { 
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1;
	int addrlen = sizeof(address); 
	
        int length = 10;
        threads = calloc(sizeof(pthread_t), length);
        // Server is made.
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Setting socket options.
	if (setsockopt(server_fd, SOL_SOCKET, 
                    SO_REUSEADDR, &opt, sizeof(opt))) { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT); // Attaching the socket to defined PORT 
	
	// Forcefully attaching socket to the PORT 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	// Start listening for incoming connections. Will deny if 10 in queue
	if (listen(server_fd, 10) < 0) { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 

        printf("Accepting Connections to Chess.\n");

        while(1) {
	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) { 
                fprintf(stderr, "Failure to accept incoming connection.");
            } else {
                printf("Connection found!\n");
                fflush(stdout);
                int thread_index = -1;
                for (int i = 0; i < length; i++) {
                    if (threads[i] == 0) {
                        thread_index = i;
                        break;
                    } 
                }

                printf("Conenction is being made into a thread.\n");
                if (thread_index == -1) {
                    thread_index = length;
                    length *= 2;
                    threads = realloc(threads, sizeof(threads) * length);
                }

                Socket_Info *info = NULL;
                info->id = thread_index;
                info->socket = new_socket;
                printf("Dog.\n");
                fflush(stdout);
                pthread_create(&threads[thread_index], NULL, 
                        handle_player, (void*) info);
                printf("Dog.\n");
	    }

        }

	return 0; 
} 
