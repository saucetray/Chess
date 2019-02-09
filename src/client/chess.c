///;;
/// chess.c
/// client program main to handle the chess game
/// Author: Justin Sostre
///


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ncurses.h>
#include <unistd.h>

#include "chess_network.h"

#define HOST_SIZE 60
#define MIN_SIZE_SCREEN 40
#define RESIZE_SCREEN = "Your screen needs to be atleast 40x40. Press Enter to End"

#define ENTER_KEY 10
#define DELETE_KEY 127

#define NO_ERROR 0
#define NOT_CHESS 2
#define HOST_EXISTS 0
#define HOST_INVALID 1


/// clear_row - clears the specified row
/// row - the row numbers
static inline void clear_row(int row) {
    move(row, 0);
    clrtoeol();
}


/// create_newin - creates a new window with a border
/// height - height of the window
/// width - the width of the window
/// starty - the start y of the window
/// startx - the start x of the window
WINDOW *create_newwin(int height, int width, int starty, int startx) {	
    
    WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, starty , startx);		
	wrefresh(local_win);		
	return local_win;
}


/// host_query - asks user for a hostname and port
/// row - the rows of the terminal
/// col - the columns of the terminal
/// error - error code if failure to connect/use server
/// return - returns hostname
static char *host_query(int row, int col, int error) {
    clear();
    char *host = "Enter Hostname and Port: ";
    char *welcome = "Welcome to Chess.";
    attron(A_BOLD); // bolds

    if (error == HOST_INVALID || error == NOT_CHESS) {      // handles errors for connecting if possible
        char *error_msg;
        if (error == HOST_INVALID) error_msg = "The host is not responding because it does not exist or it is offline.";
        else error_msg = "The host's name and/or port does not support a chess server.";     
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        mvprintw(row/2 - 3, (col-strlen(error_msg))/2, "%s", error_msg);
        refresh();
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


/// connect_to_server - handles connecting to the chess server
/// row - rows of the terminal
/// col - columns of the terminal
/// return - socket integer
int connect_to_server(int row, int col) {
    char ip[16]; // ip 
    char *host; // the hostname
    char *port; // the port number
    int error = NO_ERROR, sock;
    struct sockaddr_in serv_addr; 
    
    do {

        char *hostname = host_query(row, col, error);
        if (strlen(hostname) == 0) {
            return -1;
        } // Exit by just pressing enter
        host = strtok(hostname, ":");
        port = strtok(NULL, ":");
    
        if (hostname_to_ip(hostname, ip) == HOST_INVALID) {
            error = HOST_INVALID;
        } else {
            init_pair(2, COLOR_BLUE, COLOR_BLACK);
            attron(COLOR_PAIR(2));
            int print_offset = 3;
            mvprintw(print_offset, 0, "%s", "Connecting to...");
            print_offset++;
            if (strcmp(ip, hostname) != 0) {
                mvprintw(4, 0, "%s", "Host: ");
                printw("%s", host);
                print_offset++;
            }
            mvprintw(print_offset, 0, "%s", "IP: " );
            printw("%s", ip);
            print_offset++;
            mvprintw(print_offset, 0, "%s", "Port: ");
            printw("%s", port);
            refresh();
            attroff(COLOR_PAIR(2));
            memset(&serv_addr, '0', sizeof(serv_addr));
            
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // create a socket
                return -1;
            }

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(strtol(port, NULL, 0));

            inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); 
                
            sleep(1);
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                error = NOT_CHESS;
            }
        }
    } while (error != NO_ERROR); 
    return sock;
}


/// login_server - handles logging into the server
/// socket - socket number
/// row - the row of the main window
/// col - the col of the main window
void login_server(int socket, int row, int col) {

    create_newwin(10, 10, 15, 15);

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
    
    int socket = connect_to_server(row, col);
     
    //handle_server(socket, row, col);

    getch();
    refresh();
    endwin();

    return 0;
}

