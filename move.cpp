#include "move.h"

extern Board whiteBoard, blackBoard;

void getMoves(unordered_set<string> &moves, bool turn) // turn: white=false, black=true
{
    Board &board = !turn ? whiteBoard : blackBoard;

    for (int rank = FIRST_RANK; rank < RANK_SIZE; rank++)
    {
        for (int file = 0; file < RANK_SIZE; file++)
        {
            char square = rank * RANK_SIZE + file;
            char piece = get_piece_at_square(board, square);
            if ((turn == false && is_black_piece(piece)) || (turn == true && is_white_piece(piece)) || piece == EMPTY_SQUARE)
                continue;
            switch (piece)
            {
            case WHITE_PAWN:
            case BLACK_PAWN:
                getPawnMoves(moves, turn, square);
                break;
            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                getKnightMoves(moves, turn, square);
                break;
            case WHITE_BISHOP:
            case BLACK_BISHOP:
                getBishopMoves(moves, turn, square);
                break;
            case WHITE_ROOK:
            case BLACK_ROOK:
                getRookMoves(moves, turn, square);
                break;
            case WHITE_QUEEN:
            case BLACK_QUEEN:
                getQueenMoves(moves, turn, square);
                break;
            case WHITE_KING:
            case BLACK_KING:
                getKingMoves(moves, turn, square);
                break;
            default:
                break;
            }
        }
    }
}

void getPawnMoves(unordered_set<string> &moves, bool turn, char square)
{
    Board &board = !turn ? whiteBoard : blackBoard;

    int direction = turn ? -1 : 1; // white moves up (+1), black moves down (-1)
    int startRank = turn ? RANK_7 : RANK_2;
    int enPassantRank = turn ? RANK_4 : RANK_5;

    // Forward move
    char forward = square + direction * 8;
    if (forward >= 0 && forward < 64 && get_piece_at_square(board, forward) == '.')
    {
        moves.insert(move_to_string(make_move(square, forward)));

        // Double forward move from starting position
        if (square_to_rank(square) == startRank)
        {
            char doubleForward = square + direction * 16;
            if (doubleForward >= 0 && doubleForward < 64 && get_piece_at_square(board, doubleForward) == '.')
            {
                moves.insert(move_to_string(make_move(square, doubleForward)));
            }
        }
    }

    // Capture moves
    char leftCapture = square + direction * 8 - 1;
    char rightCapture = square + direction * 8 + 1;

    if (leftCapture >= 0 && leftCapture < 64 && square_to_file(square) != FILE_A)
    {
        char piece = get_piece_at_square(board, leftCapture);
        if (piece != '.' && ((turn && is_white_piece(piece)) || (!turn && is_black_piece(piece))))
        {
            moves.insert(move_to_string(make_move(square, leftCapture)));
        }
    }

    if (rightCapture >= 0 && rightCapture < 64 && square_to_file(square) != FILE_H)
    {
        char piece = get_piece_at_square(board, rightCapture);
        if (piece != '.' && ((turn && is_white_piece(piece)) || (!turn && is_black_piece(piece))))
        {
            moves.insert(move_to_string(make_move(square, rightCapture)));
        }
    }

    // En passant
    if (square_to_rank(square) == enPassantRank)
    {
        bool *enPassantArray = turn ? enPassantBlack : enPassantWhite;
        int file = square_to_file(square);

        // En passant left
        if (file > FILE_A && enPassantArray[file - 1])
        {
            moves.insert(move_to_string(make_move(square, leftCapture)));
        }
        // En passant right
        if (file < FILE_H && enPassantArray[file + 1])
        {
            moves.insert(move_to_string(make_move(square, rightCapture)));
        }
    }
}

void getKnightMoves(unordered_set<string> &moves, bool turn, char square)
{
    Board &board = !turn ? whiteBoard : blackBoard;

    static vector<vector<char>> knightMoveTable = []() {
        vector<vector<char>> table(64);
        for (int sq = 0; sq < 64; ++sq)
        {
            int rank = sq / 8, file = sq % 8;
            int knightMoves[8][2] = {{2, 1}, {2, -1}, {1, 2}, {1, -2}, {-2, 1}, {-2, -1}, {-1, 2}, {-1, -2}};

            for (int i = 0; i < 8; ++i)
            {
                int newRank = rank + knightMoves[i][0];
                int newFile = file + knightMoves[i][1];
                if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8)
                    table[sq].push_back(newRank * 8 + newFile);
            }
        }
        return table;
    }();

    for (char target : knightMoveTable[(int)square])
    {
        char piece = get_piece_at_square(board, target);

        if (piece == '.' || (turn && is_white_piece(piece)) || (!turn && is_black_piece(piece)))
        {
            moves.insert(move_to_string(make_move(square, target)));
        }
    }
}
void getBishopMoves(unordered_set<string> &moves, bool turn, char square)
{
    Board &board = !turn ? whiteBoard : blackBoard;
    static const int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    int rank = square / 8, file = square % 8;

    for (int dir = 0; dir < 4; dir++)
    {
        int dr = directions[dir][0], df = directions[dir][1];
        for (int step = 1; step < 8; step++)
        {
            int newRank = rank + step * dr, newFile = file + step * df;
            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8)
                break;

            char target = newRank * 8 + newFile;
            char piece = get_piece_at_square(board, target);

            if (piece == '.')
            {
                moves.insert(move_to_string(make_move(square, target)));
            }
            else
            {
                if ((!turn && is_black_piece(piece)) || (turn && is_white_piece(piece)))
                    moves.insert(move_to_string(make_move(square, target)));
                break; // blocked
            }
        }
    }
}

void getRookMoves(unordered_set<string> &moves, bool turn, char square)
{
    Board &board = !turn ? whiteBoard : blackBoard;
    static const int directions[4][2] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};

    int rank = square / 8, file = square % 8;

    for (int dir = 0; dir < 4; dir++)
    {
        int dr = directions[dir][0], df = directions[dir][1];
        for (int step = 1; step < 8; step++)
        {
            int newRank = rank + step * dr, newFile = file + step * df;
            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8)
                break;

            char target = newRank * 8 + newFile;
            char piece = get_piece_at_square(board, target);

            if (piece == '.')
            {
                moves.insert(move_to_string(make_move(square, target)));
            }
            else
            {
                if ((!turn && is_black_piece(piece)) || (turn && is_white_piece(piece)))
                    moves.insert(move_to_string(make_move(square, target)));
                break; // blocked
            }
        }
    }
}

void getQueenMoves(unordered_set<string> &moves, bool turn, char square)
{
    getBishopMoves(moves, turn, square);
    getRookMoves(moves, turn, square);
}

void getKingMoves(unordered_set<string> &moves, bool turn, char square)
{
    Board &board = !turn ? whiteBoard : blackBoard;

    static const int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    int rank = square / 8, file = square % 8;

    for (int dir = 0; dir < 8; dir++)
    {
        int newRank = rank + directions[dir][0], newFile = file + directions[dir][1];
        if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8)
            continue;

        char target = newRank * 8 + newFile;
        char piece = get_piece_at_square(board, target);

        if (piece == '.' || (!turn && is_black_piece(piece)) || (turn && is_white_piece(piece)))
        {
            moves.insert(move_to_string(make_move(square, target)));
        }
    }
}

void makeMove(u16 move, bool turn)
{
    char from = get_move_from(move);
    char to = get_move_to(move);

    Board &currentBoard = turn ? blackBoard : whiteBoard;
    Board &otherBoard = turn ? whiteBoard : blackBoard;

    char piece = get_piece_at_square(currentBoard, from);

    // Get pointers to the bitboards
    u64 *our_piece_bb = nullptr, *opponent_piece_bb = nullptr;
    u64 *our_piece_bb_other = nullptr, *opponent_piece_bb_other = nullptr;

    char capturedPiece = get_piece_at_square(currentBoard, to);

    // Remove the captured piece
    if (capturedPiece != EMPTY_SQUARE)
    {
        if (is_white_piece(capturedPiece))
        {
            if (capturedPiece == WHITE_PAWN)
                opponent_piece_bb = &currentBoard.white_pawns, opponent_piece_bb_other = &otherBoard.white_pawns;
            else if (capturedPiece == WHITE_KNIGHT)
                opponent_piece_bb = &currentBoard.white_knights, opponent_piece_bb_other = &otherBoard.white_knights;
            else if (capturedPiece == WHITE_BISHOP)
                opponent_piece_bb = &currentBoard.white_bishops, opponent_piece_bb_other = &otherBoard.white_bishops;
            else if (capturedPiece == WHITE_ROOK)
                opponent_piece_bb = &currentBoard.white_rooks, opponent_piece_bb_other = &otherBoard.white_rooks;
            else if (capturedPiece == WHITE_QUEEN)
                opponent_piece_bb = &currentBoard.white_queens, opponent_piece_bb_other = &otherBoard.white_queens;
        }
        else
        {
            if (capturedPiece == BLACK_PAWN)
                opponent_piece_bb = &currentBoard.black_pawns, opponent_piece_bb_other = &otherBoard.black_pawns;
            else if (capturedPiece == BLACK_KNIGHT)
                opponent_piece_bb = &currentBoard.black_knights, opponent_piece_bb_other = &otherBoard.black_knights;
            else if (capturedPiece == BLACK_BISHOP)
                opponent_piece_bb = &currentBoard.black_bishops, opponent_piece_bb_other = &otherBoard.black_bishops;
            else if (capturedPiece == BLACK_ROOK)
                opponent_piece_bb = &currentBoard.black_rooks, opponent_piece_bb_other = &otherBoard.black_rooks;
            else if (capturedPiece == BLACK_QUEEN)
                opponent_piece_bb = &currentBoard.black_queens, opponent_piece_bb_other = &otherBoard.black_queens;
        }
        if (opponent_piece_bb)
        {
            remove_piece(*opponent_piece_bb, to);
            remove_piece(*opponent_piece_bb_other, 64-to);
        }
    }

    // Identify the moving piece and its bitboard
    if (is_white_piece(piece))
    {
        if (piece == WHITE_PAWN)
            our_piece_bb = &currentBoard.white_pawns, our_piece_bb_other = &otherBoard.white_pawns;
        else if (piece == WHITE_KNIGHT)
            our_piece_bb = &currentBoard.white_knights, our_piece_bb_other = &otherBoard.white_knights;
        else if (piece == WHITE_BISHOP)
            our_piece_bb = &currentBoard.white_bishops, our_piece_bb_other = &otherBoard.white_bishops;
        else if (piece == WHITE_ROOK)
            our_piece_bb = &currentBoard.white_rooks, our_piece_bb_other = &otherBoard.white_rooks;
        else if (piece == WHITE_QUEEN)
            our_piece_bb = &currentBoard.white_queens, our_piece_bb_other = &otherBoard.white_queens;
        else if (piece == WHITE_KING)
            our_piece_bb = &currentBoard.white_king, our_piece_bb_other = &otherBoard.white_king;
    }
    else
    {
        if (piece == BLACK_PAWN)
            our_piece_bb = &currentBoard.black_pawns, our_piece_bb_other = &otherBoard.black_pawns;
        else if (piece == BLACK_KNIGHT)
            our_piece_bb = &currentBoard.black_knights, our_piece_bb_other = &otherBoard.black_knights;
        else if (piece == BLACK_BISHOP)
            our_piece_bb = &currentBoard.black_bishops, our_piece_bb_other = &otherBoard.black_bishops;
        else if (piece == BLACK_ROOK)
            our_piece_bb = &currentBoard.black_rooks, our_piece_bb_other = &otherBoard.black_rooks;
        else if (piece == BLACK_QUEEN)
            our_piece_bb = &currentBoard.black_queens, our_piece_bb_other = &otherBoard.black_queens;
        else if (piece == BLACK_KING)
            our_piece_bb = &currentBoard.black_king, our_piece_bb_other = &otherBoard.black_king;
    }

    // Move the piece
    if (our_piece_bb)
    {
        remove_piece(*our_piece_bb, from);
        add_piece(*our_piece_bb, to);
        remove_piece(*our_piece_bb_other, 64-from);
        add_piece(*our_piece_bb_other, 64-to);
    }
}