/// FILENAME: chess_server.c
/// DESCRIPTION: Backend for Server
/// CONTRIBUTORS: Justin Sostre

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 10245
#define LOGIN_INFO_LIM 42

pthread_t *threads;

typedef struct {
    int id;
    int socket;
} Socket_Info;


/// login_handle - Handles authentication of player
/// socket - the socket used for the connection.
/// return - the username for later use
char *login_handle(int socket) {
    char *username = malloc(sizeof(char) * LOGIN_INFO_LIM);
    char password[42] = {0};

    int protocol;
    read(socket, username, LOGIN_INFO_LIM * sizeof(char)); 
    puts(username);
    send(socket, &protocol, sizeof(int), 0); 
    read(socket, password, LOGIN_INFO_LIM * sizeof(char));
    puts(password);
    send(socket, &protocol, sizeof(int), 0);

    return username;
}


/// handle_player - player's own thread running
/// args - arguments for the thread
/// *** Authentication Is Not Implemented ***
void *handle_player(void *args) {

    Socket_Info *info = (Socket_Info*) args;
    
    char *username;
    if ((username = login_handle(info->socket)) == NULL) {
        free(username);
        free(info);
        return 0;
    }
    
    return 0;
}


/// main - handles starting up handling connections for the server
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
            int thread_index = -1;
            for (int i = 0; i < length; i++) {
                if (threads[i] == 0) {
                    thread_index = i;
                    break;
                } 
            }
            if (thread_index == -1) {
                thread_index = length;
                length *= 2;
                threads = realloc(threads, sizeof(threads) * length);
            }

            Socket_Info *info = malloc(sizeof(info));
            info->id = thread_index;
            info->socket = new_socket;
            pthread_create(&threads[thread_index], NULL, 
                    handle_player, (void*) info);
	    }
    }
	return 0; 
} 

