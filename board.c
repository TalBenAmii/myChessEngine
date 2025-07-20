#include <stdio.h>
#define ull unsigned long long

// White pieces
ull white_pawns;
ull white_knights;
ull white_bishops;
ull white_rooks;
ull white_queens;
ull white_king;

// Black pieces
ull black_pawns;
ull black_knights;
ull black_bishops;
ull black_rooks;
ull black_queens;
ull black_king;

#define BOARD_SIZE 64
enum Square
{
    a1 = 0,
    b1,
    c1,
    d1,
    e1,
    f1,
    g1,
    h1,
    a2,
    b2,
    c2,
    d2,
    e2,
    f2,
    g2,
    h2,
    a3,
    b3,
    c3,
    d3,
    e3,
    f3,
    g3,
    h3,
    a4,
    b4,
    c4,
    d4,
    e4,
    f4,
    g4,
    h4,
    a5,
    b5,
    c5,
    d5,
    e5,
    f5,
    g5,
    h5,
    a6,
    b6,
    c6,
    d6,
    e6,
    f6,
    g6,
    h6,
    a7,
    b7,
    c7,
    d7,
    e7,
    f7,
    g7,
    h7,
    a8,
    b8,
    c8,
    d8,
    e8,
    f8,
    g8,
    h8
};

#define exists_piece(pieceBoard, square) ((pieceBoard >> square) & 1ULL)
#define get_piece(pieceBoard, square) (pieceBoard & (1ULL << square))
#define add_piece(pieceBoard, square) pieceBoard |= (1ULL << square)
#define remove_piece(pieceBoard, square) pieceBoard ^= get_piece(pieceBoard, square)

char get_piece_at_square(int square)
{
    if (exists_piece(white_pawns, square))
        return 'P';
    if (exists_piece(white_knights, square))
        return 'N';
    if (exists_piece(white_bishops, square))
        return 'B';
    if (exists_piece(white_rooks, square))
        return 'R';
    if (exists_piece(white_queens, square))
        return 'Q';
    if (exists_piece(white_king, square))
        return 'K';
    if (exists_piece(black_pawns, square))
        return 'p';
    if (exists_piece(black_knights, square))
        return 'n';
    if (exists_piece(black_bishops, square))
        return 'b';
    if (exists_piece(black_rooks, square))
        return 'r';
    if (exists_piece(black_queens, square))
        return 'q';
    if (exists_piece(black_king, square))
        return 'k';
    return '.';
}

void printFullBoard()
{
    printf("\n");
    for (int rank = 7; rank >= 0; rank--)
    {
        printf("%d ", rank + 1);
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            printf("%c ", get_piece_at_square(sq));
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

void initBoard()
{
    for (int i = 0; i < 8; i++)
    {
        add_piece(white_pawns, a2 + i);
        add_piece(black_pawns, a7 + i);
        if (i == 0 || i == 7)
        {
            add_piece(white_rooks, a1 + i);
            add_piece(black_rooks, a8 + i);
        }
        else if (i == 1 || i == 6)
        {
            add_piece(white_knights, a1 + i);
            add_piece(black_knights, a8 + i);
        }
        else if (i == 2 || i == 5)
        {
            add_piece(white_bishops, a1 + i);
            add_piece(black_bishops, a8 + i);
        }
        else if (i == 3)
        {
            add_piece(white_queens, a1 + i);
            add_piece(black_queens, a8 + i);
        }
        else
        {
            add_piece(white_king, a1 + i);
            add_piece(black_king, a8 + i);
        }
    }
}

int main()
{
    initBoard();
    printFullBoard();
    return 0;
}