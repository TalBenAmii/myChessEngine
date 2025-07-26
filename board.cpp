#include "board.h"

bool enPassantWhite[8];
bool enPassantBlack[8];

void printFullBoard(const Board &board)
{
    printf("\n");
    for (int rank = LAST_RANK; rank >= FIRST_RANK; rank--)
    {
        printf("%d ", rank + 1);
        for (int file = 0; file < RANK_SIZE; file++)
        {
            int sq = rank * RANK_SIZE + file;
            printf("%c ", get_piece_at_square(board, sq));
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

char get_piece_at_square(const Board &board, char square)
{
    if (exists_piece(board.white_pawns, square))
        return WHITE_PAWN;
    if (exists_piece(board.white_knights, square))
        return WHITE_KNIGHT;
    if (exists_piece(board.white_bishops, square))
        return WHITE_BISHOP;
    if (exists_piece(board.white_rooks, square))
        return WHITE_ROOK;
    if (exists_piece(board.white_queens, square))
        return WHITE_QUEEN;
    if (exists_piece(board.white_king, square))
        return WHITE_KING;
    if (exists_piece(board.black_pawns, square))
        return BLACK_PAWN;
    if (exists_piece(board.black_knights, square))
        return BLACK_KNIGHT;
    if (exists_piece(board.black_bishops, square))
        return BLACK_BISHOP;
    if (exists_piece(board.black_rooks, square))
        return BLACK_ROOK;
    if (exists_piece(board.black_queens, square))
        return BLACK_QUEEN;
    if (exists_piece(board.black_king, square))
        return BLACK_KING;
    return EMPTY_SQUARE;
}
bool isSquareBeingAttacked(const Board &board, char square,bool attackedBy)
{
    
}
