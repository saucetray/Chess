///
/// FILE: chess_engine.h
/// DESCRIPTION: interface for chess_engine.c
/// AUTHOR: Justin Sostre
///

#include <stdint.h>

typedef uint64_t bitboard;


/// Pieces - holds all pieces bitboards for a player
typedef struct {
    bitboard pawns;
    bitboard rooks;
    bitboard bishops;
    bitboard knights;
    bitboard queen;
    bitboard king;
    bitboard full_board;
} Pieces;


/// Chess_Board - holds all the bitboards of the game
typedef struct {
    Pieces *p1_pieces;
    Pieces *p2_pieces;
} Chess_Board;


/// Coordinate - holds start and destination coordinates in (x,y); 
typedef struct {
    char x1;
    char y1;
    char x2;
    char y2;
} Coordinate;


/// Chess_Game - holds the chess game information
typedef struct {
    Chess_Board *board;
    int player1;
    int player2;
} Chess_Game;


/// create_chess_game - creates the chess game and all the structs necessary
/// arguments:       NONE
///
/// returns:         pointer to the Chess_Game struct on the heap
Chess_Game *create_chess_game(void);


/// move_piece - moves the piece to a location if it validates the move
/// arguments:       player - player number
///                  board - pointer to the chess_board
///                  cord - coordinates for the move
///
/// returns:         true or false if you can move
int move_piece(short player, short piece_t, Chess_Board *board, Coordinate cord);


/// print_board - prints the new board with ncurses
/// arguments:       board - the board to print
///
/// returns:         NONE
void print_board(Chess_Board *board);
