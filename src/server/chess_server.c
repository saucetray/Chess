/// FILENAME: chess_server.c
/// DESCRIPTION: Backend for Server
/// CONTRIBUTORS: Justin Sostre

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 12345

#define TRUE 1
#define FALSE 0

#define MAX_BUFFER 120
#define MAX_FILES 200

#define USER_DNE 0
#define USER_EXISTS 1
#define CREDENTIALS_FILE "credentials.csv"
#define CREDENTIAL_LENGTH 42


/// create_user - creates a new entry for the new user account
/// arguments:       username - username for the new account
///                  password - password for the new account
///
/// returns:         NONE
void create_user(char username[CREDENTIAL_LENGTH], char password[CREDENTIAL_LENGTH]) {
    FILE *creds = fopen(CREDENTIALS_FILE, "a+");
    char buffer[CREDENTIAL_LENGTH * 2 + 4];
    strncpy(buffer, username, CREDENTIAL_LENGTH);
    strncat(buffer, ",", 1);
    strncat(buffer, password, CREDENTIAL_LENGTH);
    strncat(buffer, "\n\0", 2);
    printf("%s\n", buffer);

    fputs(buffer, creds);
}


/// username_validation - validates if a username exists for registering purposes
/// arguments:       username - username string to look for
///
/// returns:         WHETHER USER EXISTS OR NOT
int username_validation(char username[CREDENTIAL_LENGTH]) {
    char buffer[2*CREDENTIAL_LENGTH + 10];
    FILE *creds = fopen(CREDENTIALS_FILE, "r");
    while (fgets(buffer, 2*CREDENTIAL_LENGTH + 10, creds) != NULL) {
         char *temp_username = strtok(buffer, ",");
         if (username == temp_username) {
             return USER_EXISTS;
         }
    }

    return USER_DNE;
}


/// handle_request - handle's the request from the client
/// arguments:       socket - socket that the client is on
///                  buffer - buffer
///
/// returns:         code for running    
int handle_request(int fd, char buffer[MAX_BUFFER]) {
    char *tok = strtok(buffer, ":");

    if (strcmp(tok, "login")) {  // login handling 
        char *username = strtok(buffer, ",");  // gets username
        char *password = strtok(buffer, "");  // gets password
        if (username_validation(username) != USER_EXISTS) {
            create_user(username, password); // creates a user if the user does not exist
        } else {
            // validate_credentials(username, password); // validates the credentials
        }
    } else if (strcmp(tok,"challenge")) {  // challenge request to a player
        // challenge_player();
    } else if (strcmp(tok, "stats")) { // stats request
        // send_stats(); 
    } else if (strcmp(tok, "logout")) { // logout
        // logout();
    } else {
        // unknown_request(); 
    }

    return 1;
}


/// main - entry point into the program
///
/// returns:          error code
int main() {
    int len, rc, on = 1;
    int listen_sd = -1, new_sd = -1;

    int end_server = FALSE, compress_array = FALSE;
    int close_conn;
    char buffer[MAX_BUFFER];
    struct sockaddr_in6 addr;
    int timeout;
    struct pollfd fds[MAX_FILES];
    int nfds=1, current_size = 0, i, j;

    listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sd < 0) {
        perror("Socket creation failed.");
        return 1;
    }

    // sets up socket options
    rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, 
            (char*)&on, sizeof(on));

    if (rc < 0) {
        perror("Setsockopt() failed.");
        close(listen_sd);
        return 1;
    }

    // sets listen_sd to not be blocking
    rc = ioctl(listen_sd, FIONBIO, (char*)&on);

    if (rc < 0) {
        perror("ioctl() failed.");
        close(listen_sd);
        return 1;
    }

    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port = htons(PORT);

    // binds the listening socket to the address
    rc = bind(listen_sd, (struct sockaddr*)&addr, sizeof(addr)); 

    if (rc < 0) {
        perror("Bind() failed.");
        close(listen_sd);
        return 1;
    }

    rc = listen(listen_sd, 32);  // Sets up listening
    if (rc < 0) {
        perror("Listen failed.");
        close(listen_sd);
        return 1;
    }

    memset(fds, 0, sizeof(fds));

    fds[0].fd = listen_sd;      // Listens for new connections 
    fds[0].events = POLLIN;

    timeout = -1; // Indefinetly polling

    // Now you can poll
    printf("Chess Server started up and created socket on port %d and is"
            " accepting new connections.\n", PORT);
    fflush(stdout);

    // Now the server loop 
    do {

        // polls the file descriptors for any changes
        rc = poll(fds, nfds, timeout);
        if (rc < 0) {
            perror("Polling failed.");
            break;
        }

        // decorative because it won't time out 
        if (rc == 0) {
            printf("Poll() timed out.\n");
            break;
        }

        // keeps track of current file descriptors
        current_size = nfds;
        for (i = 0; i < current_size; i++) {
            if (fds[i].revents == 0) continue;
            if (fds[i].revents != POLLIN) {
                printf("Error! Revents = %d\n", fds[i].revents);
                end_server = TRUE;
                break;
            }

            // Listening socket is readable so that means clients are connecting.
            if (fds[i].fd == listen_sd) {
                printf("A new client is connecting to the server.\n");
                do {
                    new_sd = accept(listen_sd, NULL, NULL);
                    if (new_sd < 0) {
                        if (errno != EWOULDBLOCK) {
                            perror("Accept() failed.");
                            end_server = TRUE;
                        }
                        break;
                    }

                    // handles adding the new file descriptor and polling it
                    printf("New incoming connection to server - %d\n", new_sd);
                    fds[nfds].fd = new_sd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                } while (new_sd != -1);
            }

            else {
                printf("Descripter %d is readable\n", fds[i].fd);
                close_conn = FALSE;

                do {
                    // gets whatver is in the socket from the client
                    rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (rc < 0) {
                        if (errno != EWOULDBLOCK) {
                            perror("recv() failed.");
                            close_conn = TRUE;
                        }
                        break;
                    }
                    if (rc == 0) {
                        printf("Connection closed.");
                        close_conn = TRUE;
                        break;
                    }

                    len = rc;
                    printf("%d bytes recieved\n", len);
                    
                    handle_request(fds[i].fd, buffer);

                    // sends back whatever was recieved
                    rc = send(fds[i].fd, buffer, len, 0);
                    if (rc < 0) {
                        perror("send() failed.");
                        close_conn = TRUE;
                        break;
                    }
                } while (TRUE);

                if (close_conn) {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    compress_array = TRUE;
                }
            }
        }

        if (compress_array) {
            compress_array = FALSE;
            for (i = 0; i < nfds; i++) {
                if (fds[i].fd == -1) {
                    for (j = i; j < nfds; j++) {
                        fds[j].fd = fds[j+1].fd;
                    }
                    i--;
                    nfds--;
                }
            }
        }
    } while (end_server == FALSE);

    for (i = 0; i < nfds; i++) {
        if (fds[i].fd >= 0) close(fds[i].fd);
    }

}
