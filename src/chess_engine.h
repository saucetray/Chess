///
/// FILE: chess_engine.h
/// DESCRIPTION: interface for chess_engine.c
/// AUTHOR: Justin Sostre
///

#ifndef _CHESS_ENGINE_H
#define _CHESS_ENGINE_H

#include <stdint.h>

#define PAWNS      0xFFULL
#define ROOKS      0x81ULL
#define KNIGHTS    0x42ULL
#define BISHOPS    0x24ULL
#define QUEENS     0x10ULL
#define KINGS      0x8ULL
#define FULL_BOARD 0xFFFFULL

#define PAWN_OFFSET 8
#define BLACK_OFFSET 56

#define CHESS_HEIGHT 8

#define PAWN 0
#define ROOK 1
#define KNIGHT 2
#define BISHOP 3
#define QUEEN 4
#define KING 5

#define PAWN_PIECE  'P'
#define ROOK_PIECE  'R'
#define KNIGHT_PIECE 'N'
#define BISHOP_PIECE 'B'
#define QUEEN_PIECE 'Q'
#define KING_PIECE 'K'

#define INFO_START 70
#define BOARD_START 20

#define PLAYER_ONE 1
#define PLAYER_TWO 2

#define TOGGLE_BIT(board, n) board ^= 1ULL << n
#define SET_BIT(board, n) board |= 1ULL << n
#define CLEAR_BIT(board, n) (board &= ~(1ULL << n))
#define CHECK_BIT(board, n) ((board >> n) & 1ULL)
#define INDEX(x, y) ((y)*CHESS_HEIGHT+(8-x))

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


/// end_game - frees the game struct and ends it
/// arguments:       board - pointer to the board to free
///
/// returns:         NONE
void end_game(Chess_Game *chess_game);

#endif
