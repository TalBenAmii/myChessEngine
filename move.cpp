#include "move.h"

void getMoves(Board &board, unordered_set<string> &moves, bool turn) // turn: white=false, black=true
{
    for (int sq = 0; sq < BOARD_SIZE; sq++)
    {
        char piece = get_piece_at_square(board, sq);

        if (piece == EMPTY_SQUARE || (turn == WHITE_TURN && is_black_piece(piece)) ||
            (turn == BLACK_TURN && is_white_piece(piece)))
            continue;

        switch (piece)
        {
        case WHITE_PAWN:
        case BLACK_PAWN:
            getPawnMoves(board, moves, turn, sq);
            break;
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            getKnightMoves(board, moves, sq);
            break;
        case WHITE_BISHOP:
        case BLACK_BISHOP:
        case WHITE_ROOK:
        case BLACK_ROOK:
        case WHITE_QUEEN:
        case BLACK_QUEEN:
        case WHITE_KING:
        case BLACK_KING:
            getSlidingMoves(board, moves, sq);
            break;
        default:
            assert(false);
        }
    }
}

void getPawnMoves(Board &board, unordered_set<string> &moves, bool turn, char square)
{
    int direction = !turn ? 1 : -1;
    int startRank = !turn ? RANK_2 : RANK_7;
    int enPassantRank = !turn ? RANK_5 : RANK_4;
    bool *enPassant = !turn ? enPassantBlack : enPassantWhite;

    // Single step forward
    char to = square + 8 * direction;
    assert(to >= a1 && to <= h8);

    if (get_piece_at_square(board, to) == EMPTY_SQUARE)
    {
        moves.insert(move_to_string(make_move(square, to)));
        // Double step forward
        if (square_to_rank(square) == startRank)
        {
            to = square + 16 * direction;
            if (get_piece_at_square(board, to) == EMPTY_SQUARE)
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
        }
    }

    // Captures
    for (int file_offset = -1; file_offset <= 1; file_offset += 2)
    {
        if ((square_to_file(square) == FILE_A && file_offset == -1) ||
            (square_to_file(square) == FILE_H && file_offset == 1))
        {
            continue;
        }
        to = square + 8 * direction + file_offset;

        char target_piece = get_piece_at_square(board, to);
        if ((target_piece != EMPTY_SQUARE &&
             is_white_piece(target_piece) != is_white_piece(get_piece_at_square(board, square))) ||
            (square_to_rank(square) == enPassantRank && enPassant[square_to_file(square) + file_offset]))
        {
            moves.insert(move_to_string(make_move(square, to)));
        }
    }
}

// Generates knight moves using a pre-calculated move table.
void getKnightMoves(Board &board, unordered_set<string> &moves, char square)
{
    static const int knight_moves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    int rank = square_to_rank(square);
    int file = square_to_file(square);

    for (const auto &move : knight_moves)
    {
        int new_rank = rank + move[0];
        int new_file = file + move[1];
        if (new_rank < FIRST_RANK || new_rank > LAST_RANK || new_file < FILE_A || new_file > FILE_H)
            continue;

        char to = new_rank * 8 + new_file;
        char piece = get_piece_at_square(board, square);
        char target_piece = get_piece_at_square(board, to);
        if (!are_same_team(piece, target_piece))
        {
            moves.insert(move_to_string(make_move(square, to)));
        }
    }
}

void getSlidingMoves(Board &board, unordered_set<string> &moves, char square)
{
    char piece = get_piece_at_square(board, square);

    const char *directions = piece == WHITE_ROOK || piece == BLACK_ROOK       ? rookDirections
                             : piece == WHITE_BISHOP || piece == BLACK_BISHOP ? bishopDirections
                                                                              : queenDirections;
    bool isKing = WHITE_KING || piece == BLACK_KING;
    for (int i = 0; directions[i] != 0; ++i)
    {
        char dir = directions[i];
        char to = square + dir;
        while (to >= a1 && to <= h8)
        {
            char target_piece = get_piece_at_square(board, to);
            if (target_piece == EMPTY_SQUARE)
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
            else
            {
                if (!are_same_team(piece, target_piece))
                {
                    moves.insert(move_to_string(make_move(square, to)));
                }
                break;
            }
            if (isKing)
                break;
            to += dir;
        }
    }
}

void makeMove(Board &board, u16 move, bool turn)
{
    unordered_map<char, u64 *> pieceToPieceBoard = {
        {WHITE_PAWN, &board.white_pawns}, {WHITE_KNIGHT, &board.white_knights}, {WHITE_BISHOP, &board.white_bishops},
        {WHITE_ROOK, &board.white_rooks}, {WHITE_QUEEN, &board.white_queens},   {WHITE_KING, &board.white_king},
        {BLACK_PAWN, &board.black_pawns}, {BLACK_KNIGHT, &board.black_knights}, {BLACK_BISHOP, &board.black_bishops},
        {BLACK_ROOK, &board.black_rooks}, {BLACK_QUEEN, &board.black_queens},   {BLACK_KING, &board.black_king},
        {EMPTY_SQUARE, nullptr}};

    char from = get_move_from(move);
    char to = get_move_to(move);
    char piece = get_piece_at_square(board, from);
    char captured_piece = get_piece_at_square(board, to);

    u64 *fromPieceBoard = pieceToPieceBoard[piece];
    u64 *toPieceBoard = pieceToPieceBoard[captured_piece];

    // TODO: check if is legal move

    remove_piece(*fromPieceBoard, from);
    add_piece(*fromPieceBoard, to);

    if (toPieceBoard)
        remove_piece(*toPieceBoard, to);

    bool *currentEnPassant = !turn ? enPassantWhite : enPassantBlack;

    if (piece == WHITE_PAWN || piece == BLACK_PAWN)
    {
        if (abs(square_to_rank(from) - square_to_rank(to)) == 2)
            currentEnPassant[square_to_file(to)] = true;
        else
            currentEnPassant[square_to_file(to)] = false;
    }
}