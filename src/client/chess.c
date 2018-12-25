///
/// chess.c
/// client program main to handle the chess game
/// Author: Justin Sostre
///


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <ncurses.h>

#define HOST_SIZE 60
#define MIN_SIZE_SCREEN 40
#define RESIZE_SCREEN = "Your screen needs to be atleast 40x40. Press Enter to End"

#define ENTER_KEY 10
#define DELETE_KEY 127

#define NO_RESPONSE 1
#define NOT_CHESS 2


/// host_query - asks user for a hostname and port
/// row - the rows of the terminal
/// col - the columns of the terminal
/// error - error code if failure to connect/use server
/// return - returns hostname
char *host_query(int row, int col, int error) {
    char *host = "Enter Hostname and Port: ";
    char *welcome = "Welcome to Chess.";
    attron(A_BOLD); // bolds

    if (error == NO_RESPONSE || error == NOT_CHESS) {      // handles errors for connecting if possible
        char *error_msg;
        if (error == NO_RESPONSE) error_msg = "The host is no responding";
        else error_msg = "The host's name and/or port does not support a chess server.";     
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        mvprintw(row/2 - 3, (col-strlen(error_msg))/2, "%s", error_msg);
        attroff(COLOR_PAIR(1));
    }

    mvprintw(1, (col-strlen(welcome))/2, "%s", welcome);
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
        move(row/2, 0);
        clrtoeol(); // clears the row so no weird printing
        mvprintw(row/2, (col-strlen(host)-length)/2, "%s", host);
        printw("%s", hostname);
    }
    return hostname;
}


void start_game() {

    

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

    start_color();
    
    start_game();

    refresh();
    endwin();

    return 0;
}
