///
/// chess.c
/// client program main to handle the chess game
/// Author: Justin Sostre
///


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ncurses.h>

#define HOST_SIZE 60
#define MIN_SIZE_SCREEN 40
#define RESIZE_SCREEN = "Your screen needs to be atleast 40x40. Press Enter to End"

#define ENTER_KEY 10
#define DELETE_KEY 127

#define NO_ERROR 0
#define NO_RESPONSE 1
#define NOT_CHESS 2

#define HOST_EXISTS 0
#define HOST_INVALID -1


/// clear_row - clears the specified row
/// row - the row numbers
static inline void clear_row(int row) {
    move(row, 0);
    clrtoeol();
}


/// host_query - asks user for a hostname and port
/// row - the rows of the terminal
/// col - the columns of the terminal
/// error - error code if failure to connect/use server
/// return - returns hostname
static char *host_query(int row, int col, int error) {
    char *host = "Enter Hostname and Port: ";
    char *welcome = "Welcome to Chess.";
    attron(A_BOLD); // bolds

    if (error == NO_RESPONSE || error == NOT_CHESS) {      // handles errors for connecting if possible
        char *error_msg;
        if (error == NO_RESPONSE) error_msg = "The host is not responding because it does not exist or it is offline.";
        else error_msg = "The host's name and/or port does not support a chess server.";     
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        mvprintw(row/2 - 3, (col-strlen(error_msg))/2, "%s", error_msg);
        attroff(COLOR_PAIR(1));
    }
    
    mvprintw(1, (col-strlen(welcome))/2, "%s", welcome);

    clear_row(row/2);

    mvprintw(row/2, (col-strlen(host))/2, "%s", host);
    char lt[2]; // use char array so concatenation works with null terminator

    char *hostname = calloc(sizeof(char), HOST_SIZE); // hostname
    hostname[0] = '\0'; // starts null terminator for concatenations

    while((lt[0] = getch()) != ENTER_KEY) { // starts getting input for hostname
        int length = strlen(hostname);
        switch(lt[0]) { // handles different key inputs
            case DELETE_KEY:
                if (length != 0) {
                    hostname[length-1] = '\0';
                }
                break;
            default: // for other character input is regular
                if (length < HOST_SIZE - 1)
                    strcat(hostname, lt);
        }
        clear_row(row/2);
        mvprintw(row/2, (col-strlen(host)-length)/2, "%s", host);
        printw("%s", hostname);
    }
    return hostname;
}


/// hostname_to_ip - converts hostname to ip address
/// hostname - hostname
/// ip - pointer to ip address
/// return - error code if it does exist
int hostname_to_ip(char *hostname , char *ip) {
	struct hostent *h;
	struct in_addr **addr_list;

	if ((h = gethostbyname(hostname)) == NULL) { // if it doesn't exist, return that it doesn't exist!
		return HOST_INVALID;
	}

	addr_list = (struct in_addr **) h->h_addr_list;

	for(int i = 0; addr_list[i] != NULL; i++) {
		strcpy(ip , inet_ntoa(*addr_list[i]));
		return HOST_EXISTS;
	}

	return HOST_INVALID;
}


/// connect_to_server - handles connecting to the chess server
/// row - rows of the terminal
/// col - columns of the terminal
/// return - error code
int connect_to_server(int row, int col) {

    char ip[100];
    char *host;
    char *port;
    int error = NO_ERROR;

    do {
        char *hostname = host_query(row, col, error);
        if (strlen(hostname) == 0) {
            return 1;
        }
        host = strtok(hostname, ":");
        port = strtok(hostname, ":");
    

        error = NO_RESPONSE;
    } while(hostname_to_ip(host, ip) == -1); 
    

    return 3;
}


/// main - starting point of program and handles initializing ncurses and calling functions
int main() {
    int row, col;

    initscr();
    raw();

    keypad(stdscr, TRUE);
    noecho(); 
    
    getmaxyx(stdscr, row, col); // gets terminal size
    
    if (row < MIN_SIZE_SCREEN || col < MIN_SIZE_SCREEN) {  // minimum size screen to gaurentee the board fits and all information
        attron(A_BOLD);
        char* resize = "Your screen needs to be atleast 40x40. "
                       "Press Enter to End";
        mvprintw(row/2, (col-strlen(resize))/2, "%s", resize);
        refresh();
        getch();
        endwin();
        return 1;
    }

    if (has_colors() == FALSE) {  // terminal also needs color support to see different pieces 
        refresh();
        endwin();
        printf("Your terminal does not support color. You need color for this application.\n");
        return 2;
    }

    start_color(); // allows color for ncurses
    
    if (connect_to_server(row, col) < 0) {
        return 4;
    }

    refresh();
    endwin();

    return 0;
}
