///
/// chess.c
/// client program main to handle the chess game
/// Author: Justin Sostre
///


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <ncurses.h>


int host_query(int row, int col, char *hostname) {
    char *host = "Enter Hostname and Port:";
    char *welcome = "Welcome to Chess.";
    attron(A_BOLD);
    mvprintw(1, (col-strlen(welcome))/2, "%s", welcome);
    mvprintw(row/2, (col-strlen(host))/2, "%s", host);
    
    char letter;
    char lt[2];

    while((letter = getch()) != 10) {
        int length = strlen(hostname);
        lt[0] = letter;
        switch(letter) {
            case 127:
                if (length != 0) {
                    hostname[length-1] = '\0';
                }
                break;
            default:
                if (length < 199)
                    strcat(hostname, lt);
        }
        move(row/2, 0);
        clrtoeol();
        mvprintw(row/2, (col-strlen(host)-length)/2, "%s", host);
        printw("%s", hostname);
    }

    return 0;
}


int main() {
    
    int row, col;

    initscr();
    raw();

    keypad(stdscr, TRUE);
    noecho();
    
    getmaxyx(stdscr, row, col);
    
    if (row < 40 || col < 40) {
        char* resize = "Your screen needs to be atleast 40x40. "
                       "Press Enter to End";
        mvprintw(row/2, (col-strlen(resize))/2, "%s", resize);
        refresh();
        getch();
        endwin();
        return 1;
    }

    char hostname[200];
    host_query(row, col, hostname);

    refresh();
    endwin();
    return 0;
}
