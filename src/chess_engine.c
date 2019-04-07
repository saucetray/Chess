///
/// FILE: chess_engine.c
/// DESCRIPTION: chess engine for chess
/// AUTHOR: Justin Sostre
///


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define PAWNS   0x81
#define ROOKS   0x81
#define KNIGHTS 0x42
#define BISHOPS 0x24
#define QUEENS   0x10
#define KINGS    0x8

#define TOGGLE_BIT(board, n) board ^= 1UL << n
#define SET_BIT(board, n) board |= 1UL << n
#define CLEAR_BIT(board, n) board &= ~(1UL << n)
#define CHECK_BIT(board, n) (board >> n) & 1UL

#define PAWN    0
#define ROOK    1
#define KNIGHT  2
#define BISHOP  3
#define QUEEN   4
#define KING    5


typedef uint64_t bitboard;


typedef struct {
    bitboard *pawns;
    bitboard *rooks;
    bitboard *knights;
    bitboard *bishops;
    bitboard *queen;
    bitboard *king;
} Pieces;


typedef struct {
    Pieces *p1_pieces;
    Pieces *p2_pieces; 
} Chess_Board;


typedef struct {
    char col;
    char row;
} Coordinate;


static uint64_t reverse_bits(uint64_t bit) { 
    unsigned int count = sizeof(bit) * 8 - 1; 
    unsigned int reverse_bit = bit; 
    
    bit >>= 1;
    while(bit) { 
           reverse_bit <<= 1;        
           reverse_bit |= bit & 1; 
           bit >>= 1; 
           count--; 
    }

    reverse_bit <<= count; 
    return reverse_bit; 
} 



static int capture_piece(short piece_t, bitboard board, Coordinate cord) {

    return 0;

}


static int validate_move(short piece_t, bitboard board, Coordinate cord) {

    return 0;
}


int move_piece(short piece_t, bitboard board, Coordinate cord) {

    // check if there is a piece there
    // if enemy piece, it can be captured
    // if friendly piece, it can't be captured


    // switch case to check if move is valid with piece type

    // move the piece
    
    return 0;
}


Chess_Board *create_chess_board() {
    Chess_Board *board = malloc(sizeof(Chess_Board*));
    Pieces *p1_pieces = malloc(sizeof(Pieces*));
    *(p1_pieces->pawns) = PAWNS;
    *(p1_pieces->rooks) = (uint64_t) ROOKS;
    *(p1_pieces->knights) = KNIGHTS;
    *(p1_pieces->bishops) = BISHOPS;
    *(p1_pieces->queen) = QUEENS;
    *(p1_pieces->king) = KINGS;
    Pieces *p2_pieces = malloc(sizeof(Pieces*));
    *(p2_pieces->pawns) = reverse_bits(PAWNS); 
    *(p2_pieces->rooks) = reverse_bits(ROOKS);
    *(p2_pieces->knights) = reverse_bits(KNIGHTS);
    *(p2_pieces->bishops) = reverse_bits(BISHOPS);
    *(p2_pieces->queen) = reverse_bits(QUEENS);
    *(p2_pieces->king) = reverse_bits(KINGS);
    board->p1_pieces = p1_pieces;
    board->p2_pieces = p2_pieces;

    return board;
}



