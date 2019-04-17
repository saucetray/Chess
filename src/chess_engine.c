///
/// FILE: chess_engine.c
/// DESCRIPTION: chess engine for chess
/// AUTHOR: Justin Sostre
///


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ncurses.h>

#include "chess_engine.h"

#define PAWNS       0xFFULL
#define ROOKS       0x81ULL
#define KNIGHTS     0x42ULL
#define BISHOPS     0x24ULL
#define QUEENS      0x10ULL
#define KINGS       0x8ULL
#define FULL_BOARD  0xFFFFULL

#define PAWN_OFFSET 8
#define BLACK_OFFSET 56

#define CHESS_HEIGHT 8

#define TOGGLE_BIT(board, n) board ^= 1ULL << n
#define SET_BIT(board, n) board |= 1ULL << n
#define CLEAR_BIT(board, n) board &= ~(1ULL << n)
#define CHECK_BIT(board, n) ((board >> n) & 1ULL)
#define INDEX(x, y) ((y)*CHESS_HEIGHT+(8-x))

#define PAWN    0
#define ROOK    1
#define KNIGHT  2
#define BISHOP  3
#define QUEEN   4
#define KING    5

#define PAWN_PIECE    'P'
#define ROOK_PIECE    'R'
#define KNIGHT_PIECE  'N'
#define BISHOP_PIECE  'B'
#define QUEEN_PIECE   'Q'
#define KING_PIECE    'K'

#define INFO_START 70
#define BOARD_START 20

#define PLAYER_ONE 1
#define PLAYER_TWO 2


/// end_game - ends the game and deallocates memory
/// arguments:     chess_game - pointer to chess_game
///
/// returns:       NONE
//static void end_game(Chess_Game *chess_game) {
//
//    free(chess_game->board->p1_pieces);
//    free(chess_game->board->p2_pieces);
//    free(chess_game->board);
//    free(chess_game);
//
//}


/// capture_piece - removes the piece from its respective board
/// arguments:     pieces - pointer to the pieces of the enemy player
///                destination - destination location to check
///
/// returns:       NONE
static void capture_piece(Pieces *pieces, int destination) {

    // checking which board the piece to capture is on!
    // king doesn't need to be checked because it can only be in check
    // mate.
    if (CHECK_BIT(pieces->pawns, destination)) {
        CLEAR_BIT(pieces->pawns, destination);
    } else if (CHECK_BIT(pieces->rooks, destination)) {
        CLEAR_BIT(pieces->rooks, destination);
    } else if (CHECK_BIT(pieces->bishops, destination)) {
        CLEAR_BIT(pieces->bishops, destination);
    } else if (CHECK_BIT(pieces->knights, destination)) {
        CLEAR_BIT(pieces->knights, destination);
    } else if (CHECK_BIT(pieces->queen, destination)) {
        CLEAR_BIT(pieces->queen, destination);
    }
}


/// check_mate - checks if the king is in checkmate
/// arguments:     player - player number
///                board - pointer to the board
///                cord - coordinate struct
///
/// returns:       true or false(0 for check, -1 for checkmate) 
///                if king is safe move
//static int check_mate(short player, Chess_Board *board, int x, int y) {
//    int index = INDEX(x, y);
//
//    return 1;
//}


/// check_diagnal - checks diagnal for pieces in the way
/// arguments:     fboard - full entire board
///                cord - coordinate struct
///
/// returns:       true or false if there are pieces in the way
static int check_diagnal(bitboard fboard, Coordinate cord) {
    int x = cord.x1 - cord.x2;
    int y = cord.y1 - cord.y2;

    // CHECK IF PIECES IND DIAGNAL
    if (x > 0 && y > 0) {
        for (int i = cord.x1-1; i > cord.x2; i--) {
           if (CHECK_BIT(fboard, INDEX(cord.x1-i, cord.y1-i))) return 1; 
        }
    } else if ( x < 0 && y < 0 ) {
        for (int i = cord.x1+1; i < cord.x2; i++) {
            if (CHECK_BIT(fboard, INDEX(cord.x1+i, cord.y1+i))) return 1;  
        }
    } else if ( x > 0 && y < 0 ) {
        for (int i = cord.x1-1; i > cord.x2; i--) {
            if (CHECK_BIT(fboard, INDEX(cord.x1-i, cord.y1+i))) return 1;
        }
    } else {
        for (int i = cord.x1+1; i < cord.x2; i++) {
            if (CHECK_BIT(fboard, INDEX(cord.x1+i, cord.y1-i))) return 1; 
       }
    }

    return 0;
}


/// check_straight - checks if pieces are in the way of path (straight)
/// arguments:     fboard - full entire board
///                cord - coordinate struct
///
/// returns:       true or false if there are pieces in the way;
static int check_straight(bitboard fboard, Coordinate cord) {
    int x = cord.x1 - cord.x2;
    int y = cord.y1 - cord.y2;
    // CHECK IF PIECES ARE IN THE WAY SOME PIECES CAN'T JUMP OVER THINGS
    if (x > 0) {
        for (int i = cord.x1-1; i > cord.x2; i--) {
            if (CHECK_BIT(fboard, INDEX(i, cord.y1))) return 1;
        }
    } else if (x < 0) {
        for (int i = cord.x1+1; i < cord.x2; i++) {
            if (CHECK_BIT(fboard, INDEX(i, cord.y1))) return 1;  
        }
    } else if (y > 0) {
        for (int i = cord.y1-1; i > cord.y2; i--) {
            if (CHECK_BIT(fboard, INDEX(cord.x1, i))) return 1;  
        }
    } else {
        for (int i = cord.y1+1; i < cord.y2; i++) {
            if (CHECK_BIT(fboard, INDEX(cord.x1, i))) return 1;
        }
    }

    return 0;
}


/// validate_king - validates king movement
/// arguments:     cord - coordinate struct
///
/// returns:       true or false if king can move
static int validate_king(Coordinate cord) {
    int x = abs(cord.x1 - cord.x2);
    int y = abs(cord.y1 - cord.y2);

    // can only move one
    if (x == 1 && y == 1) return 1;

    return 0;
}


/// validate_queen - validates queen movement
/// arguments:     cord - coordinate struct
///
/// returns:       true or false if queen can move
static int validate_queen(Chess_Board *board, Coordinate cord) {
    int x = abs(cord.x1 - cord.x2);
    int y = abs(cord.y1 - cord.y2);

    // check if move is diagnal or straight
    int diagnal = ((x > 0 && y ==0) || (x == 0 && y > 0));
    int straight = ((abs(cord.x1 - cord.x2) != abs(cord.y1 - cord.y2)));
    
    // can't be neither and can't be both
    if (diagnal ^ straight) return 1;
    
    bitboard fboard = board->p1_pieces->full_board | board->p2_pieces->full_board;
    if (diagnal) check_diagnal(fboard, cord);
    else check_straight(fboard, cord);
    return 0;
}


/// validate_rook - validates rook movement
/// arguments:     board - pointer to the board
///                cord - coordinate struct
///
/// returns:       true or false if bishop can move
static int validate_rook(Chess_Board *board, Coordinate cord) {
    int x = cord.x1 - cord.x2;
    int y = cord.y1 - cord.y2;

    // can only go up or sideways. Not both.
    if (!(abs(x) > 0 && abs(y) == 0) && !(abs(x) == 0 && abs(y) > 0)) return 0;

    bitboard fboard = board->p1_pieces->full_board | board->p2_pieces->full_board;
    if (check_straight(fboard, cord)) return 0;
    return 1;
}


/// validate_kight - validates knight movement 
/// arguments:     cord - coordinate struct
///
/// returns:       true or false if knight can move
static int validate_knight(Coordinate cord) {
    int x = abs(cord.x1 - cord.x2);
    int y = abs(cord.y1 - cord.y2);

    // vectors can be 1 or 2 but not both can be same value
    if ((x == 1 && y == 2) || (x == 2 && y == 1)) return 1;

    return 0;
}


/// validate_bishop - validates bishop movement
/// arguments:     cord - coordinate struct 
///
/// returns:       true or false if bishop can move
static int validate_bishop(Chess_Board *board, Coordinate cord) {
    int x = cord.x1 - cord.x2;
    int y = cord.y1 - cord.y2;

    if (abs(x) != abs(y)) return 0;

    bitboard fboard = board->p1_pieces->full_board | board->p2_pieces->full_board;

    if (check_diagnal(fboard, cord)) return 0;
    return 1;
}


/// validate_pawn - validates pawn movements
/// arguments:     player - player number
///                board - pointer to the board
///                coord - coorinate struct
///
///  returns:      true or false if pawn can move
static int validate_pawn(short player, Chess_Board *board, Coordinate cord) {
    int x = abs(cord.x1 - cord.x2);
    int y = abs(cord.y1 - cord.y2);

    int dest = INDEX(cord.x2, cord.y2);

    if (player == PLAYER_ONE) {
        if (cord.y1 > cord.y2) return 0;  // can't move backwards
        if (cord.y1 != 1 && y == 2) return 0; // can't go two spaces if not at home
        if ((x == 1 && y == 1) && !CHECK_BIT(board->p2_pieces->full_board,
              dest)) return 0;  // can't go sideways if there's no bit there
        if ((y == 1 && CHECK_BIT(board->p2_pieces->full_board, dest))) return 0;
    } else if (player == PLAYER_TWO) {  
        if (cord.y1 < cord.y2) return 0;  // can't go backwards
        if (cord.y1 != 6 && y == 2) return 0;  // can't go two spaces if not at home
        if ((x == 1 && y == 1) && !CHECK_BIT(board->p1_pieces->full_board,
              dest)) return 0;  // can't go sideways if not there
        if ((y == 1 && CHECK_BIT(board->p1_pieces->full_board, dest))) return 0;
    }
    return 1; // normal return
}


/// validate_move - validates the move made
/// arguments:     player - player number
///                board - pointer to the board
///                cord - coordinate struct
///
/// returns:       true or false if the piece can move
static int validate_move(short player, short piece_t, Chess_Board *board, Coordinate cord) {
    int start = INDEX(cord.x1, cord.y1);
    int destination = INDEX(cord.x2, cord.y2);

    // CAN'T RUN OVER YOUR OWN PIECES
    if (player == PLAYER_ONE) { 
        if (CHECK_BIT(board->p1_pieces->full_board, destination)) {
            return 0;
        }
    } else if (player == PLAYER_TWO) {
        if (CHECK_BIT(board->p2_pieces->full_board, start)) {
            return 0;
        }
    }

    // VALIDES ALL PIECES  
    switch(piece_t) {
        case PAWN: if (!validate_pawn(player, board, cord)) return 0; break;
        case KNIGHT: if (!validate_knight(cord)) return 0; break;
        case BISHOP: if (!validate_bishop(board, cord)) return 0; break;
        case ROOK:  if (!validate_rook(board, cord)) return 0; break;
        case QUEEN: if (!validate_queen(board, cord)) return 0; break;
        case KING:  if (!validate_king(cord)) return 0; break;              
        default: return -1;
    }
    return 1;
}


int move_piece(short player, short piece_t, Chess_Board *board, Coordinate cord) {

    int start = INDEX(cord.x1, cord.y1);        // gets location for the start
    int destination = INDEX(cord.x2, cord.y2);  // gets the location for destination

    if (start == destination) return 0;
    if (!validate_move(player, piece_t, board, cord)) return 0;

    Pieces *pieces;     // sets this to hold what piece location

    if (player == PLAYER_ONE) {
        pieces = board->p1_pieces;
        if (CHECK_BIT(board->p2_pieces->full_board, destination)) {
            capture_piece(board->p2_pieces, destination);  // capture the piece sitting in the board
        }

    } else if (player == PLAYER_TWO) { 
        pieces = board->p2_pieces;
        if (CHECK_BIT(board->p1_pieces->full_board, destination)) {
            capture_piece(board->p1_pieces, destination);  // capture the piece sitting in the board
        }
    }

    CLEAR_BIT(pieces->full_board, start);
    SET_BIT(pieces->full_board, destination);

    // THIS BLOCK SETS BOARD'S RESPECTIVE BITBOARD
    if (piece_t == PAWN) {         
        CLEAR_BIT(pieces->pawns, start);
        SET_BIT(pieces->pawns, destination);
    } else if (piece_t == ROOK) {
        CLEAR_BIT(pieces->rooks, start);
        SET_BIT(pieces->rooks, destination);
    } else if (piece_t == BISHOP) {
        CLEAR_BIT(pieces->bishops, start);
        SET_BIT(pieces->bishops, destination);
    } else if (piece_t == KNIGHT) {
        CLEAR_BIT(pieces->knights, start);
        SET_BIT(pieces->knights, destination);
    } else if (piece_t == QUEEN) {
        CLEAR_BIT(pieces->queen, start);
        SET_BIT(pieces->queen, destination);
    } else if (piece_t == KING) {
        CLEAR_BIT(pieces->king, start);
        SET_BIT(pieces->king, destination);
    } else {
        return -1;
    }
    
    return 0;
}


/// create_chess_board - creates the chess_board and initializes all bitboards
/// arguments:       NONE
///
/// returns:         pointer to the Chess_Board stuct on the heap
static Chess_Board *create_chess_board() {
    Chess_Board *board = malloc(sizeof(Chess_Board));
    board->p1_pieces = malloc(sizeof(Pieces));
    board->p2_pieces = malloc(sizeof(Pieces));

    // PUTS ALL THE PIECES IN THE SPOTS
    board->p1_pieces->pawns = PAWNS << PAWN_OFFSET;  
    board->p1_pieces->rooks = ROOKS;
    board->p1_pieces->knights = KNIGHTS;
    board->p1_pieces->bishops = BISHOPS;
    board->p1_pieces->queen = QUEENS;
    board->p1_pieces->king = KINGS;
    board->p2_pieces->pawns = PAWNS << (BLACK_OFFSET - PAWN_OFFSET); 
    board->p2_pieces->rooks = ROOKS << BLACK_OFFSET;
    board->p2_pieces->knights = KNIGHTS << BLACK_OFFSET;
    board->p2_pieces->bishops = BISHOPS << BLACK_OFFSET;
    board->p2_pieces->queen = QUEENS << BLACK_OFFSET;
    board->p2_pieces->king = KINGS << BLACK_OFFSET;

    // GETS THE FULL BOARD!
    board->p1_pieces->full_board = FULL_BOARD;
    board->p2_pieces->full_board = FULL_BOARD << (BLACK_OFFSET - PAWN_OFFSET);
    return board;
}


Chess_Game *create_chess_game() {
    Chess_Game *game = malloc(sizeof(Chess_Game*));
    game->board = create_chess_board();
    return game;
}


static void test_print_board(bitboard board) {
    for (int i = 63; i >= 0; i--) {
        bitboard bit = CHECK_BIT(board, i) & 0x1;
        if (i % 8 == 7 && i != 63) {
            printf("\n");
        }
        printf("%llu", bit);
    }
    printf("\n");
}


void print_board(Chess_Board *board) {

    char *label = "*****       BOARD       *****";
    mvprintw(4, BOARD_START + 4, "%s", label);

    char *numbers = "1   2   3   4   5   6   7   8\n";
    char *border = "---------------------------------";
    char bar = '|';
    
    char letters[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}; // labels for rows
    mvprintw(5, BOARD_START + 4, "%s", numbers);
    mvprintw(6, BOARD_START + 2, "%s", border);

    Pieces *pieces;

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK); // inits colors for the board
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    for (int i = 7; i >= 0; i--) {
        mvprintw(21-2*i, BOARD_START, "%c", letters[7-i]);
        printw("%c", ' ');
        for (int j = 0; j < 8; j++) {   // some indexing for the board to print
            int index = INDEX(j-1, i);
            printw("%c", bar);
            printw("%c", ' ');
           
            int p1 = CHECK_BIT(board->p1_pieces->full_board, index); // checks if piece
            int p2 = CHECK_BIT(board->p2_pieces->full_board, index); // exits

            if (p1) {
                attron(COLOR_PAIR(1));            
                pieces = board->p1_pieces;
            } else if (p2) {
                attron(COLOR_PAIR(2)); 
                pieces = board->p2_pieces;
            } else {
                pieces = NULL;
            }

            if (pieces) { // handles printing the piece if it exists
                if (CHECK_BIT(pieces->pawns, index)) printw("%c", PAWN_PIECE);
                else if (CHECK_BIT(pieces->rooks, index)) printw("%c", ROOK_PIECE);
                else if (CHECK_BIT(pieces->knights, index)) printw("%c", KNIGHT_PIECE);
                else if (CHECK_BIT(pieces->bishops, index)) printw("%c", BISHOP_PIECE);
                else if (CHECK_BIT(pieces->queen, index)) printw("%c", QUEEN_PIECE);
                else if (CHECK_BIT(pieces->king, index)) printw("%c", KING_PIECE);
            } else {
                printw("%c", ' ');
            }

            if (p1) attroff(COLOR_PAIR(1)); // turns off colorz!
            else if (p2) attroff(COLOR_PAIR(2));

            printw("%c", ' ');
        }
      printw("%c", '|');
      printw("%c", ' ');
      printw("%c", letters[7-i]);
      mvprintw(21-2*i-1, BOARD_START + 2, "%s", border); // border printing
    }
  
    mvprintw(21-2*-1-1, BOARD_START + 2, "%s", border);
    mvprintw(21-2*-2-2, BOARD_START + 4, "%s", numbers);
}


/// print_help - prints the info page for all pieces symbols and colors
void print_help() {
    mvprintw(10, INFO_START, "%s", "INFO PAGE");
    mvprintw(11, INFO_START, "%s", "PLAYER ONE - RED");
    mvprintw(12, INFO_START, "%s", "PLAYER TWO - BLUE");

    mvprintw(14, INFO_START, "%s", "K - KING");
    mvprintw(15, INFO_START, "%s", "Q - QUEEN");
    mvprintw(16, INFO_START, "%s", "B - BISHOP");
    mvprintw(17, INFO_START, "%s", "N - KNIGHT");
    mvprintw(18, INFO_START, "%s", "R - ROOK");
    mvprintw(19, INFO_START, "%s", "P - PAWN");
}


int main() {
    Chess_Game *game = create_chess_game();
    initscr();
    print_board(game->board);
    print_help();

    getch();
    Coordinate cord = {8, 1, 8, 3};

    move_piece(PLAYER_ONE, PAWN, game->board, cord);
    print_board(game->board);
    getch();
    Coordinate cord2 = {8, 0, 8, 2};
    move_piece(PLAYER_ONE, ROOK, game->board, cord2);
    print_board(game->board);
    getch();
    Coordinate cord3 = {8, 2, 7, 2};
    move_piece(PLAYER_ONE, ROOK, game->board, cord3);
    print_board(game->board);
    getch();
    Coordinate cord4 = {7, 2, 7, 6};
    move_piece(PLAYER_ONE, ROOK, game->board, cord4);
    print_board(game->board);

    refresh();
    getch();
    endwin();
    return 0;
}
