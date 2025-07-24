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
            if ((turn == false && is_black_piece(piece)) || (turn == true && is_white_piece(piece)))
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
    char from = (move >> 8) & 0xFF;
    char to = move & 0xFF;

    Board &ourBoard = !turn ? whiteBoard : blackBoard;
    Board &opponentBoard = !turn ? blackBoard : whiteBoard;

    char piece = get_piece_at_square(ourBoard, from);

    // Remove piece from source square on both boards
    if (piece == WHITE_PAWN)
    {
        remove_piece(ourBoard.white_pawns, from);
        remove_piece(opponentBoard.white_pawns, from);
    }
    else if (piece == WHITE_KNIGHT)
    {
        remove_piece(ourBoard.white_knights, from);
        remove_piece(opponentBoard.white_knights, from);
    }
    else if (piece == WHITE_BISHOP)
    {
        remove_piece(ourBoard.white_bishops, from);
        remove_piece(opponentBoard.white_bishops, from);
    }
    else if (piece == WHITE_ROOK)
    {
        remove_piece(ourBoard.white_rooks, from);
        remove_piece(opponentBoard.white_rooks, from);
    }
    else if (piece == WHITE_QUEEN)
    {
        remove_piece(ourBoard.white_queens, from);
        remove_piece(opponentBoard.white_queens, from);
    }
    else if (piece == WHITE_KING)
    {
        remove_piece(ourBoard.white_king, from);
        remove_piece(opponentBoard.white_king, from);
    }
    else if (piece == BLACK_PAWN)
    {
        remove_piece(ourBoard.black_pawns, from);
        remove_piece(opponentBoard.black_pawns, from);
    }
    else if (piece == BLACK_KNIGHT)
    {
        remove_piece(ourBoard.black_knights, from);
        remove_piece(opponentBoard.black_knights, from);
    }
    else if (piece == BLACK_BISHOP)
    {
        remove_piece(ourBoard.black_bishops, from);
        remove_piece(opponentBoard.black_bishops, from);
    }
    else if (piece == BLACK_ROOK)
    {
        remove_piece(ourBoard.black_rooks, from);
        remove_piece(opponentBoard.black_rooks, from);
    }
    else if (piece == BLACK_QUEEN)
    {
        remove_piece(ourBoard.black_queens, from);
        remove_piece(opponentBoard.black_queens, from);
    }
    else if (piece == BLACK_KING)
    {
        remove_piece(ourBoard.black_king, from);
        remove_piece(opponentBoard.black_king, from);
    }

    // Handle captures - remove opponent piece from destination
    char capturedPiece = get_piece_at_square(opponentBoard, to);
    if (capturedPiece != EMPTY_SQUARE)
    {
        if (is_white_piece(capturedPiece))
        {
            if (capturedPiece == WHITE_PAWN)
                remove_piece(ourBoard.white_pawns, to), remove_piece(opponentBoard.white_pawns, to);
            else if (capturedPiece == WHITE_KNIGHT)
                remove_piece(ourBoard.white_knights, to), remove_piece(opponentBoard.white_knights, to);
            else if (capturedPiece == WHITE_BISHOP)
                remove_piece(ourBoard.white_bishops, to), remove_piece(opponentBoard.white_bishops, to);
            else if (capturedPiece == WHITE_ROOK)
                remove_piece(ourBoard.white_rooks, to), remove_piece(opponentBoard.white_rooks, to);
            else if (capturedPiece == WHITE_QUEEN)
                remove_piece(ourBoard.white_queens, to), remove_piece(opponentBoard.white_queens, to);
            // King capture is not handled here as it ends the game.
        }
        else // is_black_piece
        {
            if (capturedPiece == BLACK_PAWN)
                remove_piece(ourBoard.black_pawns, to), remove_piece(opponentBoard.black_pawns, to);
            else if (capturedPiece == BLACK_KNIGHT)
                remove_piece(ourBoard.black_knights, to), remove_piece(opponentBoard.black_knights, to);
            else if (capturedPiece == BLACK_BISHOP)
                remove_piece(ourBoard.black_bishops, to), remove_piece(opponentBoard.black_bishops, to);
            else if (capturedPiece == BLACK_ROOK)
                remove_piece(ourBoard.black_rooks, to), remove_piece(opponentBoard.black_rooks, to);
            else if (capturedPiece == BLACK_QUEEN)
                remove_piece(ourBoard.black_queens, to), remove_piece(opponentBoard.black_queens, to);
        }
    }

    // Handle en passant capture
    if ((piece == WHITE_PAWN || piece == BLACK_PAWN))
    {
        int fromFile = square_to_file(from);
        int toFile = square_to_file(to);
        if (fromFile != toFile && capturedPiece == EMPTY_SQUARE)
        { // diagonal pawn move to empty square is en-passant
            char capturedPawnSquare = turn ? (to + 8) : (to - 8);
            // Remove the en passant captured pawn
            if (turn) // black moved, captured white pawn
            {
                remove_piece(ourBoard.white_pawns, capturedPawnSquare);
                remove_piece(opponentBoard.white_pawns, capturedPawnSquare);
            }
            else // white moved, captured black pawn
            {
                remove_piece(ourBoard.black_pawns, capturedPawnSquare);
                remove_piece(opponentBoard.black_pawns, capturedPawnSquare);
            }
        }
    }

    // Place piece on destination square on both boards
    if (is_white_piece(piece))
    {
        if (piece == WHITE_PAWN)
            add_piece(ourBoard.white_pawns, to), add_piece(opponentBoard.white_pawns, to);
        else if (piece == WHITE_KNIGHT)
            add_piece(ourBoard.white_knights, to), add_piece(opponentBoard.white_knights, to);
        else if (piece == WHITE_BISHOP)
            add_piece(ourBoard.white_bishops, to), add_piece(opponentBoard.white_bishops, to);
        else if (piece == WHITE_ROOK)
            add_piece(ourBoard.white_rooks, to), add_piece(opponentBoard.white_rooks, to);
        else if (piece == WHITE_QUEEN)
            add_piece(ourBoard.white_queens, to), add_piece(opponentBoard.white_queens, to);
        else if (piece == WHITE_KING)
            add_piece(ourBoard.white_king, to), add_piece(opponentBoard.white_king, to);
    }
    else // is_black_piece
    {
        if (piece == BLACK_PAWN)
            add_piece(ourBoard.black_pawns, to), add_piece(opponentBoard.black_pawns, to);
        else if (piece == BLACK_KNIGHT)
            add_piece(ourBoard.black_knights, to), add_piece(opponentBoard.black_knights, to);
        else if (piece == BLACK_BISHOP)
            add_piece(ourBoard.black_bishops, to), add_piece(opponentBoard.black_bishops, to);
        else if (piece == BLACK_ROOK)
            add_piece(ourBoard.black_rooks, to), add_piece(opponentBoard.black_rooks, to);
        else if (piece == BLACK_QUEEN)
            add_piece(ourBoard.black_queens, to), add_piece(opponentBoard.black_queens, to);
        else if (piece == BLACK_KING)
            add_piece(ourBoard.black_king, to), add_piece(opponentBoard.black_king, to);
    }
}
