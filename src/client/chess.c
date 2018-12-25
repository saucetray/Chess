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


int main() {
    
    int row, col;

    initscr();
    raw();

    keypad(stdscr, TRUE);
    noecho();
    
    getmaxyx(stdscr, row, col);
    
    if (row < 80 || col < 80) {
        char* resize = "Your screen needs to be atleast 80x80";
        mvprintw(row/2, (col-strlen(resize))/2, "%s", resize);
        refresh();
        getch();
        endwin();
        return 1;
    }

    char hostname[200];
    char *host = "Enter Hostname:";
    char *welcome = "Welcome to Chess.";
    attron(A_BOLD);
    mvprintw(1, (col-strlen(welcome))/2, "%s", welcome);
    mvprintw(row/2, (col-strlen(host))/2-10, "%s", host);
    getstr(hostname);
    
    endwin();
    printf("%s", hostname);
    return 0;
}
