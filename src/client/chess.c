///
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
#include "chess_engine.h"

#define HOST_SIZE 60
#define MIN_SIZE_SCREEN 50
#define RESIZE_SCREEN = "Your screen needs to be atleast 50x50." \
                        "Press Enter to End"


#define USERNAME_SIZE 42
#define PASSWORD_SIZE 42
#define ENTER_KEY 10
#define DELETE_KEY 127

#define NO_ERROR 0
#define NOT_CHESS 2
#define HOST_EXISTS 0
#define HOST_INVALID 1

#define FAILED_LOGIN -5

#define LOCAL_GAME = 0
#define ONLINE_GAME = 1
#define AI_GAME = 2

#define COORDINATE_MAX 5

/// handle_sigint - handles the signal for interrupt
/// sig - the number of the signal
//void handle_sigint(int sig) {     
//    refresh();
//    endwin();
//    id_t iPid = getpid();
//    kill(iPid, SIGINT);
//}



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
	  wrefresh(local_win);		
	  return local_win;
}


/// print_board - prints the board
/// arguments:    board - the pointer to the board
///
/// return:       NONE
static void print_board(Chess_Board *board) {
    char *label = "****     BOARD     ****";
    mvprintw(4, BOARD_START + 7, "%s", label);  // board numbers and label

    char *numbers = "1   2   3   4   5   6   7   8\n";
    char *border = "+-------------------------------+";
    char bar = '|';

    char letters[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    mvprintw(5, BOARD_START + 4, "%s", numbers);  // PRINT NUMBERS
    mvprintw(6, BOARD_START + 2, "%s", border);

    Pieces *pieces;
    for (int i = 7; i >=0; i--) { // loops through board to print
        mvprintw(21-2*i, BOARD_START, "%c", letters[7-i]);
        printw("%c", ' ');
        for (int j = 0; j < 8; j++) {
            int index = INDEX(j-1, i);
            printw("%c", bar);
            printw("%c", ' ');

            // if piece exists then we gotta print that
            int p1 = CHECK_BIT(board->p1_pieces->full_board, index);
            int p2 = CHECK_BIT(board->p2_pieces->full_board, index);

            if (p1) {
                attron(COLOR_PAIR(1));
                pieces = board->p1_pieces;
            } else if (p2) {
                attron(COLOR_PAIR(2));
                pieces = board->p2_pieces;
            } else {
                pieces = NULL;
            }

            // prints the piece and its respective symbol
            if (pieces) {
                if (CHECK_BIT(pieces->pawns, index)) printw("%c", PAWN_PIECE);
                else if (CHECK_BIT(pieces->rooks, index)) printw("%c", ROOK_PIECE);
                else if (CHECK_BIT(pieces->knights, index)) printw("%c", KNIGHT_PIECE);
                else if (CHECK_BIT(pieces->bishops, index)) printw("%c", BISHOP_PIECE);
                else if (CHECK_BIT(pieces->queen, index)) printw("%c", QUEEN_PIECE);
                else if (CHECK_BIT(pieces->king, index)) printw("%c", KING_PIECE);
                else printw("%c", ' ');
            } else {
                printw("%c", ' ');
            }

            // turns off color
            if (p1) attroff(COLOR_PAIR(1));
            else if (p2) attroff(COLOR_PAIR(2));

            printw("%c", ' ');
        }

        // finishes the right print of the board
        printw("%c", bar);
        printw("%c", ' ');
        printw("%c", letters[7-i]);
        mvprintw(21-2*i-1, BOARD_START + 2, "%s", border);
    }

    // finishes the board print
    mvprintw(21-2*-1-1, BOARD_START + 2, "%s", border);
    mvprintw(21-2*-2-2, BOARD_START + 4, "%s", numbers);
}


/// validate_input - valides the input from the user
/// arguments:     pieces - pointer to the pieces
///                buffer - input buffer
///                board - pointer to the board
///
/// returns:       piece_id
static int validate_input(Pieces *pieces, char buffer[COORDINATE_MAX], Coordinate *cord) {

    // this is just checking bounds are in good and converting it to an int
    int num;
    if ((num = 'H' - buffer[0]) < 0 || num > 7) return -1;
    cord->y1 = num;

    if ((num = buffer[1] - '0') < 0 || num > 7) return -1;
    cord->x1 = num;

    if ((num ='H' - buffer[3]) < 0 || num > 7) return -1;
    cord->y2 = num;

    if ((num = buffer[4] - '0') < 0 || num > 7) return -1;
    cord->x2 = num;

    int start = INDEX(cord->x1, cord->y1);

    if (!CHECK_BIT(pieces->full_board, start)) return -1;

    if (CHECK_BIT(pieces->pawns, start)) return PAWN;
    if (CHECK_BIT(pieces->rooks, start)) return ROOK;
    if (CHECK_BIT(pieces->bishops, start)) return BISHOP;
    if (CHECK_BIT(pieces->knights, start)) return KNIGHT;
    if (CHECK_BIT(pieces->queen, start)) return QUEEN;
    if (CHECK_BIT(pieces->king, start)) return KING;
    return -1;

}


/// local_game_loop - simulates a game when the player wants to locally play
/// arguments:     NONE
///
/// returns:       NONE
static void local_game_loop() {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    attron(A_BOLD);

    Chess_Game *game = create_chess_game();

    short piece_t;
    Coordinate cord;

    char buffer[COORDINATE_MAX];
    int game_status = 1;
    while (game_status) {
        memset(buffer, 0, COORDINATE_MAX);
        attron(COLOR_PAIR(1));
        mvprintw(1, 1, "%s", "Local Game!");
        attroff(COLOR_PAIR(2));

        int move;
        print_board(game->board);
        // PLAYER 1 MOVE
        do {
            clear_row(28);
            mvprintw(28, BOARD_START, "%s", "Player 1 Move ([Letter][Number],[Letter][Number]):");
            getstr(buffer);

            if ((piece_t = validate_input(game->board->p1_pieces, buffer, &cord)) >= 0)
                move = move_piece(PLAYER_ONE, piece_t, game->board, cord);
            else move = 0;  
        } while (move == 0);

        move_piece(PLAYER_ONE, piece_t, game->board, cord);
        print_board(game->board);

        memset(buffer, 0, COORDINATE_MAX);
        // PLAYER 2 MOVE
        do { 
            clear_row(28);
            mvprintw(28, BOARD_START, "%s", "Player 2 Move ([Letter][Number],[Letter][Number]):");
            getstr(buffer);

            if ((piece_t = validate_input(game->board->p2_pieces, buffer, &cord)) >= 0)
                move = move_piece(PLAYER_TWO, piece_t, game->board, cord);
            else move = 0;  
        } while (move == 0);

        print_board(game->board);
    }

    end_game(game);
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

    // handles errors for connecting if possible
    if (error == HOST_INVALID || error == NOT_CHESS) {             
        char *error_msg;
        if (error == HOST_INVALID) 
            error_msg = "The host is not responding " \
                        "because it does not exist or it is offline.";
        else error_msg = "The host's name and/or port does not " \
                         "support a chess server.";     
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        mvprintw(row/2 - 3, (col-strlen(error_msg))/2, "%s", error_msg);
        refresh();
        attroff(COLOR_PAIR(1));
    }

    mvprintw(1, (col-strlen(welcome))/2, "%s", welcome);
    clear_row(row/2);

    mvprintw(row/2, (col-strlen(host))/3, "%s", host);

    char *hostname = calloc(sizeof(char), HOST_SIZE); // hostname
    hostname[0] = '\0'; // starts null terminator for concatenations 
    refresh();
    getstr(hostname);
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
        } else if (port) {
            init_pair(2, COLOR_BLUE, COLOR_BLACK);
            attron(COLOR_PAIR(2));
            int print_offset = 0;
            mvprintw(print_offset, 0, "%s", "SERVER INFO");
            print_offset++;
            mvprintw(print_offset, 0, "%s", "                     ");

            mvprintw(print_offset, 0, "%s", "Connecting to...");
            print_offset++;
            if (strcmp(ip, hostname) != 0) {
                mvprintw(print_offset, 0, "%s", "Host: ");
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
            
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                return -1;
            }

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(strtol(port, NULL, 0));
            inet_pton(AF_INET, ip, &serv_addr.sin_addr); 
    
            if (connect(sock, (struct sockaddr *)&serv_addr, 
                        sizeof(serv_addr)) < 0) {
                error = NOT_CHESS;
                attron(COLOR_PAIR(1));
                mvprintw(1, 0, "%s", "Failed To Conenct!");
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                mvprintw(1, 0, "%s", "Connection Sucessful!");
                attroff(COLOR_PAIR(2));
                error = NO_ERROR;
            }
        }
    } while (error != NO_ERROR); 
    return sock;
}


/// send_credentials - sends username and password for authentication
/// username - the user's username
/// password - the user's password
int send_credentials(int socket, char username[USERNAME_SIZE], 
        char password[PASSWORD_SIZE]) {

    char credentials[100];
    strncpy(credentials, username, USERNAME_SIZE);
    strncat(credentials, "|", 1);
    strncat(credentials, password, PASSWORD_SIZE);
    char response[100];

    write(socket, credentials, USERNAME_SIZE+PASSWORD_SIZE);
    read(socket, response, 100);
    
    printf(response);
    fflush(stdout);
    return -1;

    sleep(2);
    return 1;
}


/// login_server - handles logging into the server
/// socket - socket number
/// row - the row of the main window
/// col - the col of the main window
int login_server(int socket, int row, int col) {

    int notAuthenticated = 0;
    do {
        char *login_prompt = "Login";
        char *username_prompt = "Username: ";
        char *password_prompt = "Password: ";
        char username[USERNAME_SIZE] = {0};
        char password[PASSWORD_SIZE] = {0};
        mvprintw(row/2-3, (col-strlen(login_prompt))/2, 
                "%s", login_prompt);
        mvprintw(row/2-1, (col-strlen(username_prompt))/2, 
                "%s", username_prompt);
        mvprintw(row/2+1, (col-strlen(password_prompt))/2, 
                "%s", password_prompt);
        clear_row(row/2);
        clear_row(row/2-3);
        move(row/2-1, (col+strlen(username_prompt))/2);
        getstr(username);
        if (username[0]=='\0') return -1;
        move(row/2+1, (col+strlen(password_prompt))/2);
        getstr(password);
        if (password[0]=='\0') return -1;

        if (send_credentials(socket, username, password) < 0) {
            notAuthenticated = 1;
        }

    } while(notAuthenticated);

    return 1;
}

/// main - starting point of program and handles initializing ncurses 
///        and calling functions
int main() {
    //signal(SIGINT, handle_sigint);

    int row, col;
    initscr();

    raw();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, row, col); // gets terminal size
    
    if (row < MIN_SIZE_SCREEN || col < MIN_SIZE_SCREEN) {  // minimum size 
        attron(A_BOLD);
        char* resize = "Your screen needs to be atleast 50x50. " \
                       "Press Enter to End";
        mvprintw(row/2, (col-strlen(resize))/2, "%s", resize);
        refresh();
        getch();
        endwin();
        return 1;
    }

    // terminal also needs color support to see different pieces
    if (has_colors() == FALSE) {   
        refresh();
        endwin();
        printf("Your terminal does not support color. " 
                "You need color for this application.\n");
        return 2;
    }

    Chess_Game *game = create_chess_game();

    start_color(); // allows color for ncurses
    if (1) {
        local_game_loop();    
    } else {
        int socket = connect_to_server(row, col); 
        //handle_server(socket, row, col);
        login_server(socket, row, col);
    }
    refresh();
    getch();
    endwin();

    return 0;
}

