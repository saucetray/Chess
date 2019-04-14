///
/// FILE: chess_engine.c
/// DESCRIPTION: chess engine for chess
/// AUTHOR: Justin Sostre
///


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "chess_engine.h"

#define PAWNS    0xFFULL
#define ROOKS    0x81ULL
#define KNIGHTS  0x42ULL
#define BISHOPS  0x24ULL
#define QUEENS   0x10ULL
#define KINGS    0x8ULL

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

#define PLAYER_ONE 1
#define PLAYER_TWO 2


static bitboard get_full_board(Pieces *pieces) { 
    bitboard full_board = 0;
    full_board |= pieces->pawns;
    full_board |= pieces->rooks;
    full_board |= pieces->knights;
    full_board |= pieces->bishops;
    full_board |= pieces->queen;
    full_board |= pieces->king;
    return full_board;
}


static int capture_piece(short piece_t, bitboard board, Coordinate cord) {


    return 0;
}


static int validate_pawn(short player, Chess_Board *board, Coordinate cord) {

    if (player == PLAYER_ONE) {
        if (cord.x1 > cord.x2) return -1;
        if (CHECK_BIT(board->p2_pieces->full_board, 
              INDEX(cord.x2, cord.y2))) return -1;
    } else if (player == PLAYER_TWO) {
        if (cord.x1 < cord.x2) return -1;
    }
  
    return 0;
}


int validate_move(short player, short piece_t, Chess_Board *board, Coordinate cord) {
    board->p1_pieces->full_board = get_full_board(board->p1_pieces);
    board->p2_pieces->full_board = get_full_board(board->p2_pieces);

    if (player == PLAYER_ONE) {
        if (CHECK_BIT(board->p1_pieces->full_board, cord.x2 * cord.y2) == 1) {
            return -1;
        }
    } else if (player == PLAYER_TWO) {
        if (CHECK_BIT(board->p2_pieces->full_board, cord.x2 * cord.y2) == 1) {
            return -1;
        }
    }

    switch(piece_t) {
        case PAWN: break;
        default: break;
    }
    return 0;
}


int move_piece(short player, short piece_t, bitboard board, Coordinate cord) {


    // check if there is a piece there
    // if enemy piece, it can be captured
    // if friendly piece, it can't be captured

    // switch case to check if move is valid with piece type

    // move the piece
    
    return 0;
}


Chess_Board *create_chess_board() {
    Chess_Board *board = malloc(sizeof(Chess_Board));
    board->p1_pieces = malloc(sizeof(Pieces));
    board->p2_pieces = malloc(sizeof(Pieces));
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

    board->p1_pieces->full_board = get_full_board(board->p1_pieces);
    board->p2_pieces->full_board = get_full_board(board->p2_pieces);
    return board;
}


Chess_Game *create_chess_game() {
    Chess_Game *game = malloc(sizeof(Chess_Game*));
    game->board = create_chess_board();
    return game;
}


void print_board(bitboard board) {
    for (int i = 63; i >= 0; i--) {
        bitboard bit = CHECK_BIT(board, i) & 0x1;
        if (i % 8 == 7 && i != 63) {
            printf("\n");
        }
        printf("%llu", bit);
    }
    printf("\n");
}


int main() {
    Chess_Game *game = create_chess_game();

    SET_BIT(game->board->p1_pieces->full_board, INDEX(2,7));
    print_board(game->board->p2_pieces->full_board);
}
