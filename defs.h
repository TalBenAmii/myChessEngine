#ifndef DEFS_H
#define DEFS_H
#include <stdio.h>
#include <cassert>

#define BOARD_SIZE 64
#define RANK_SIZE 8
#define FIRST_RANK 0
#define LAST_RANK 7
#define SECOND_RANK_START a2
#define SECOND_RANK_END h2

#define WHITE_PAWN 'P'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_ROOK 'R'
#define WHITE_QUEEN 'Q'
#define WHITE_KING 'K'
#define BLACK_PAWN 'p'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_ROOK 'r'
#define BLACK_QUEEN 'q'
#define BLACK_KING 'k'
#define EMPTY_SQUARE '.'

#define QUEEN_FILE 3
#define ROOK_FILE_1 0
#define ROOK_FILE_2 7
#define KNIGHT_FILE_1 1
#define KNIGHT_FILE_2 6
#define BISHOP_FILE_1 2
#define BISHOP_FILE_2 5

#define ull unsigned long long

#define exists_piece(pieceBoard, square) (((pieceBoard) >> (square)) & 1ULL)
#define get_piece(pieceBoard, square) ((pieceBoard) & (1ULL << (square)))
#define add_piece(pieceBoard, square) (pieceBoard) |= (1ULL << (square))
#define remove_piece(pieceBoard, square) (pieceBoard) ^= get_piece((pieceBoard), (square))

#define is_black_piece(piece) ((piece) >= BLACK_PAWN && (piece) <= BLACK_KING)
#define is_white_piece(piece) ((piece) >= WHITE_PAWN && (piece) <= WHITE_KING)

// clang-format off
enum Square : char
{
    a1 = 0, b1, c1, d1, e1, f1, g1, h1,
    a2,     b2, c2, d2, e2, f2, g2, h2,
    a3,     b3, c3, d3, e3, f3, g3, h3,
    a4,     b4, c4, d4, e4, f4, g4, h4,
    a5,     b5, c5, d5, e5, f5, g5, h5,
    a6,     b6, c6, d6, e6, f6, g6, h6,
    a7,     b7, c7, d7, e7, f7, g7, h7,
    a8,     b8, c8, d8, e8, f8, g8, h8
};
// clang-format on

typedef struct Board
{
    ull white_pawns;
    ull white_knights;
    ull white_bishops;
    ull white_rooks;
    ull white_queens;
    ull white_king;

    ull black_pawns;
    ull black_knights;
    ull black_bishops;
    ull black_rooks;
    ull black_queens;
    ull black_king;
} Board;


#endif /* DEFS_H */
