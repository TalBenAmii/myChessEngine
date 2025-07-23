#include "move.h"

void getMoves(Board &board, std::vector<char> &moves, bool turn) // turn: white=false, black=true
{
    for (int rank = FIRST_RANK; rank < RANK_SIZE; rank++)
    {
        for (int file = 0; file < RANK_SIZE; file++)
        {
            char piece = get_piece_at_square(board, rank * file);
            if ((turn == false && is_black_piece(piece)) || (turn == true && is_white_piece(piece)))
                continue;
            switch (piece)
            {
            case WHITE_PAWN:
            case BLACK_PAWN:
                return getPawnMoves(board, moves, turn, rank * file);
            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                return getKnightMoves(board, moves, turn, rank * file);
            case WHITE_BISHOP:
            case BLACK_BISHOP:
                return getBishopMoves(board, moves, turn, rank * file);
            case WHITE_ROOK:
            case BLACK_ROOK:
                return getRookMoves(board, moves, turn, rank * file);
            case WHITE_QUEEN:
            case BLACK_QUEEN:
                return getQueenMoves(board, moves, turn, rank * file);
            case WHITE_KING:
            case BLACK_KING:
                return getKingMoves(board, moves, turn, rank * file);
            default:
                assert(false);
                break;
            }
        }
    }
}

void getPawnMoves(Board &board, std::vector<char> &moves, bool turn, char square)
{
    // moves
    if (square + RANK_SIZE < BOARD_SIZE && get_piece_at_square(board, square + RANK_SIZE) == '.')
    {
        moves.push_back(square + RANK_SIZE);
        if (SECOND_RANK_START <= square && square <= SECOND_RANK_END &&
            get_piece_at_square(board, square + 2 * RANK_SIZE) == '.')
            moves.push_back(square + 2 * RANK_SIZE);
    }
    // take moves
}
void getKnightMoves(Board &board, std::vector<char> &moves, bool turn, char square);
void getBishopMoves(Board &board, std::vector<char> &moves, bool turn, char square);
void getRookMoves(Board &board, std::vector<char> &moves, bool turn, char square);
void getQueenMoves(Board &board, std::vector<char> &moves, bool turn, char square);
void getKingMoves(Board &board, std::vector<char> &moves, bool turn, char square);
