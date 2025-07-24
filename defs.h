#ifndef DEFS_H
#define DEFS_H
#include <cassert>
#include <stdio.h>
#include <string>
#include <vector>

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
#define FILE_A 0
#define FILE_B 1
#define FILE_C 2
#define FILE_D 3
#define FILE_E 4
#define FILE_F 5
#define FILE_G 6
#define FILE_H 7

#define RANK_1 0
#define RANK_2 1
#define RANK_3 2
#define RANK_4 3
#define RANK_5 4
#define RANK_6 5
#define RANK_7 6
#define RANK_8 7

#define u64 unsigned long long
#define u16 unsigned short

#define exists_piece(pieceBoard, square) (((pieceBoard) >> (square)) & 1ULL)
#define get_piece(pieceBoard, square) ((pieceBoard) & (1ULL << (square)))
#define add_piece(pieceBoard, square) (pieceBoard) |= (1ULL << (square))
#define remove_piece(pieceBoard, square) (pieceBoard) ^= get_piece((pieceBoard), (square))

#define is_black_piece(piece) ((piece) >= BLACK_PAWN && (piece) <= BLACK_KING)
#define is_white_piece(piece) ((piece) >= WHITE_PAWN && (piece) <= WHITE_KING)

#define square_to_rank(square) ((square) / 8)
#define square_to_file(square) ((square) % 8)

#define is_square_in_rank(square, rank) ((rank) == square_to_rank(square))

#define up_square(square) (((square) >= 56) ? -1 : ((square) + 8))
#define down_square(square) (((square) < 8) ? -1 : ((square) - 8))
#define left_square(square) (((square) % 8 == 0) ? -1 : (square) - 1)
#define right_square(square) (((square) % 8 == 7) ? -1 : (square) + 1)

#define get_move_from(move) (((move) >> 8) & 0xFF)
#define get_move_to(move) ((move) & 0xFF)
#define make_move(from, to) (((from) << 8) | (to))

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
    u64 white_pawns;
    u64 white_knights;
    u64 white_bishops;
    u64 white_rooks;
    u64 white_queens;
    u64 white_king;

    u64 black_pawns;
    u64 black_knights;
    u64 black_bishops;
    u64 black_rooks;
    u64 black_queens;
    u64 black_king;
} Board;

// Function to convert square number to algebraic notation
inline const char *square_to_string(int square)
{
    static const char *square_names[64] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2",
                                           "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a4", "b4",
                                           "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5",
                                           "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7",
                                           "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};
    return (square >= 0 && square < 64) ? square_names[square] : "??";
}

inline std::string move_to_string(u16 move)
{
    int from = get_move_from(move);
    int to = get_move_to(move);
    return std::string(square_to_string(from)) + square_to_string(to);
}

// Function to convert algebraic notation to square number
inline int string_to_square(const std::string &square_str)
{
    if (square_str.length() != 2)
        return -1;

    char file = square_str[0];
    char rank = square_str[1];

    if (file < 'a' || file > 'h' || rank < '1' || rank > '8')
        return -1;

    return (rank - '1') * 8 + (file - 'a');
}

// Function to convert algebraic notation to u16 move
inline u16 string_to_move(const std::string &move_str)
{
    if (move_str.length() != 4)
        return 0;

    std::string from_str = move_str.substr(0, 2);
    std::string to_str = move_str.substr(2, 2);

    int from = string_to_square(from_str);
    int to = string_to_square(to_str);

    if (from == -1 || to == -1)
        return 0;

    return make_move(from, to);
}

#endif /* DEFS_H */
