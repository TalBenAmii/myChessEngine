#include "move.h"

// The external whiteBoard is the single source of truth for the game state.
extern Board whiteBoard;

// Generates all legal moves for the current player.
// It iterates through the board, identifies pieces of the current player,
// and calls the appropriate move generation function for that piece.
void getMoves(unordered_set<string> &moves, bool turn) // turn: white=false, black=true
{
    for (int sq = 0; sq < BOARD_SIZE; sq++)
    {
        char piece = get_piece_at_square(whiteBoard, sq);

        if (piece == EMPTY_SQUARE)
            continue;

        if ((!turn && is_white_piece(piece)) || (turn && is_black_piece(piece)))
        {
            switch (piece)
            {
            case WHITE_PAWN:
            case BLACK_PAWN:
                getPawnMoves(moves, turn, sq);
                break;
            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                getKnightMoves(moves, turn, sq);
                break;
            case WHITE_BISHOP:
            case BLACK_BISHOP:
                getBishopMoves(moves, turn, sq);
                break;
            case WHITE_ROOK:
            case BLACK_ROOK:
                getRookMoves(moves, turn, sq);
                break;
            case WHITE_QUEEN:
            case BLACK_QUEEN:
                getQueenMoves(moves, turn, sq);
                break;
            case WHITE_KING:
            case BLACK_KING:
                getKingMoves(moves, turn, sq);
                break;
            }
        }
    }
}

// Generates pawn moves, including forward, double forward, captures, and en passant.
void getPawnMoves(unordered_set<string> &moves, bool turn, char square)
{
    int direction = !turn ? 1 : -1;
    int startRank = !turn ? RANK_2 : RANK_7;
    int enPassantRank = !turn ? RANK_5 : RANK_4;

    // Single step forward
    char to = square + 8 * direction;
    if (to >= a1 && to <= h8 && get_piece_at_square(whiteBoard, to) == EMPTY_SQUARE)
    {
        moves.insert(move_to_string(make_move(square, to)));
        // Double step forward
        if (square_to_rank(square) == startRank)
        {
            to = square + 16 * direction;
            if (get_piece_at_square(whiteBoard, to) == EMPTY_SQUARE)
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
        }
    }

    // Captures
    for (int file_offset = -1; file_offset <= 1; file_offset += 2)
    {
        if ((square_to_file(square) == FILE_A && file_offset == -1) || (square_to_file(square) == FILE_H && file_offset == 1))
        {
            continue;
        }
        to = square + 8 * direction + file_offset;
        if (to >= a1 && to <= h8)
        {
            char target_piece = get_piece_at_square(whiteBoard, to);
            if (target_piece != EMPTY_SQUARE && is_white_piece(target_piece) != is_white_piece(get_piece_at_square(whiteBoard, square)))
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
        }
    }
    // En Passant
    if (square_to_rank(square) == enPassantRank)
    {
        bool *enPassantFile = turn ? enPassantBlack : enPassantWhite;
        int currentFile = square_to_file(square);
        if (currentFile > FILE_A && enPassantFile[currentFile - 1])
        {
            moves.insert(move_to_string(make_move(square, square + 8 * direction - 1)));
        }
        if (currentFile < FILE_H && enPassantFile[currentFile + 1])
        {
            moves.insert(move_to_string(make_move(square, square + 8 * direction + 1)));
        }
    }
}

// Generates knight moves using a pre-calculated move table.
void getKnightMoves(unordered_set<string> &moves, bool turn, char square)
{
    static const int knight_moves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    int rank = square_to_rank(square);
    int file = square_to_file(square);

    for (const auto &move : knight_moves)
    {
        int new_rank = rank + move[0];
        int new_file = file + move[1];
        if (new_rank >= FIRST_RANK && new_rank <= LAST_RANK && new_file >= FILE_A && new_file <= FILE_H)
        {
            char to = new_rank * 8 + new_file;
            char target_piece = get_piece_at_square(whiteBoard, to);
            if (target_piece == EMPTY_SQUARE || (turn != is_white_piece(target_piece)))
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
        }
    }
}

// Generates bishop moves by sliding along diagonals.
void getBishopMoves(unordered_set<string> &moves, bool turn, char square)
{
    const int directions[] = {-9, -7, 7, 9};
    for (int dir : directions)
    {
        char to = square + dir;
        while (to >= a1 && to <= h8)
        {
            if (abs(square_to_file(to) - square_to_file(to - dir)) != 1)
                break;
            char target_piece = get_piece_at_square(whiteBoard, to);
            if (target_piece == EMPTY_SQUARE)
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
            else
            {
                if (turn != is_white_piece(target_piece))
                {
                    moves.insert(move_to_string(make_move(square, to)));
                }
                break;
            }
            to += dir;
        }
    }
}

// Generates rook moves by sliding along ranks and files.
void getRookMoves(unordered_set<string> &moves, bool turn, char square)
{
    const int directions[] = {-8, -1, 1, 8};
    for (int dir : directions)
    {
        char to = square + dir;
        while (to >= a1 && to <= h8)
        {
            if (abs(square_to_file(square) - square_to_file(to)) > 0 && abs(square_to_rank(square) - square_to_rank(to)) > 0)
            {
                if (abs(square_to_file(to) - square_to_file(to - dir)) > 1)
                    break;
            }
            char target_piece = get_piece_at_square(whiteBoard, to);
            if (target_piece == EMPTY_SQUARE)
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
            else
            {
                if (turn != is_white_piece(target_piece))
                {
                    moves.insert(move_to_string(make_move(square, to)));
                }
                break;
            }
            to += dir;
        }
    }
}

// Generates queen moves by combining bishop and rook moves.
void getQueenMoves(unordered_set<string> &moves, bool turn, char square)
{
    getBishopMoves(moves, turn, square);
    getRookMoves(moves, turn, square);
}

// Generates king moves, one step in any direction.
void getKingMoves(unordered_set<string> &moves, bool turn, char square)
{
    const int directions[] = {-9, -8, -7, -1, 1, 7, 8, 9};
    for (int dir : directions)
    {
        char to = square + dir;
        if (to >= a1 && to <= h8)
        {
            if (abs(square_to_file(square) - square_to_file(to)) > 1)
                continue;
            char target_piece = get_piece_at_square(whiteBoard, to);
            if (target_piece == EMPTY_SQUARE || (turn != is_white_piece(target_piece)))
            {
                moves.insert(move_to_string(make_move(square, to)));
            }
        }
    }
}

// Updates the single whiteBoard based on the move made.
void makeMove(u16 move, bool turn)
{
    char from = get_move_from(move);
    char to = get_move_to(move);
    char piece = get_piece_at_square(whiteBoard, from);

    // Remove piece from the 'from' square
    u64 *from_bb = nullptr;
    if (is_white_piece(piece))
    {
        if (piece == WHITE_PAWN)
            from_bb = &whiteBoard.white_pawns;
        else if (piece == WHITE_KNIGHT)
            from_bb = &whiteBoard.white_knights;
        else if (piece == WHITE_BISHOP)
            from_bb = &whiteBoard.white_bishops;
        else if (piece == WHITE_ROOK)
            from_bb = &whiteBoard.white_rooks;
        else if (piece == WHITE_QUEEN)
            from_bb = &whiteBoard.white_queens;
        else if (piece == WHITE_KING)
            from_bb = &whiteBoard.white_king;
    }
    else
    {
        if (piece == BLACK_PAWN)
            from_bb = &whiteBoard.black_pawns;
        else if (piece == BLACK_KNIGHT)
            from_bb = &whiteBoard.black_knights;
        else if (piece == BLACK_BISHOP)
            from_bb = &whiteBoard.black_bishops;
        else if (piece == BLACK_ROOK)
            from_bb = &whiteBoard.black_rooks;
        else if (piece == BLACK_QUEEN)
            from_bb = &whiteBoard.black_queens;
        else if (piece == BLACK_KING)
            from_bb = &whiteBoard.black_king;
    }
    if (from_bb)
        remove_piece(*from_bb, from);

    // If it's a capture, remove the piece from the 'to' square
    char captured_piece = get_piece_at_square(whiteBoard, to);
    if (captured_piece != EMPTY_SQUARE)
    {
        u64 *captured_bb = nullptr;
        if (is_white_piece(captured_piece))
        {
            if (captured_piece == WHITE_PAWN)
                captured_bb = &whiteBoard.white_pawns;
            else if (captured_piece == WHITE_KNIGHT)
                captured_bb = &whiteBoard.white_knights;
            else if (captured_piece == WHITE_BISHOP)
                captured_bb = &whiteBoard.white_bishops;
            else if (captured_piece == WHITE_ROOK)
                captured_bb = &whiteBoard.white_rooks;
            else if (captured_piece == WHITE_QUEEN)
                captured_bb = &whiteBoard.white_queens;
        }
        else
        {
            if (captured_piece == BLACK_PAWN)
                captured_bb = &whiteBoard.black_pawns;
            else if (captured_piece == BLACK_KNIGHT)
                captured_bb = &whiteBoard.black_knights;
            else if (captured_piece == BLACK_BISHOP)
                captured_bb = &whiteBoard.black_bishops;
            else if (captured_piece == BLACK_ROOK)
                captured_bb = &whiteBoard.black_rooks;
            else if (captured_piece == BLACK_QUEEN)
                captured_bb = &whiteBoard.black_queens;
        }
        if (captured_bb)
            remove_piece(*captured_bb, to);
    }

    // Add piece to the 'to' square
    if (from_bb)
        add_piece(*from_bb, to);

    // Reset en passant arrays
    bool *enPassantFile = turn ? enPassantWhite : enPassantBlack;
    for (int i = 0; i < 8; ++i)
        enPassantFile[i] = false;

    // Set en passant flag if pawn moved two squares
    if ((piece == WHITE_PAWN || piece == BLACK_PAWN) && abs(square_to_rank(from) - square_to_rank(to)) == 2)
    {
        bool *currentEnPassant = turn ? enPassantBlack : enPassantWhite;
        currentEnPassant[square_to_file(from)] = true;
    }
}