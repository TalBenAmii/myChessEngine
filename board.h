#ifndef BOARD_H
#define BOARD_H
#include "defs.h"

constexpr Board initialWhiteBoard = {
    0x000000000000FF00ULL, // white pawns
    0x0000000000000042ULL, // white knights
    0x0000000000000024ULL, // white bishops
    0x0000000000000081ULL, // white rooks
    0x0000000000000008ULL, // white queen
    0x0000000000000010ULL, // white king

    0x00FF000000000000ULL, // black pawns
    0x4200000000000000ULL, // black knights
    0x2400000000000000ULL, // black bishops
    0x8100000000000000ULL, // black rooks
    0x0800000000000000ULL, // black queen
    0x1000000000000000ULL  // black king
};

extern bool enPassantWhite[8];
extern bool enPassantBlack[8];

void printFullBoard(const Board &board);
void initBoard();
char get_piece_at_square(const Board &board, char square);

#endif /* BOARD_H */
