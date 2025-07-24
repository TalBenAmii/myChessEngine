#include "move.h"

extern Board whiteBoard,blackBoard;

void getMoves(std::vector<u16> &moves, bool turn) // turn: white=false, black=true
{
    Board &board = !turn ? whiteBoard : blackBoard;

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
                getPawnMoves(moves, turn, rank * file);
                break;
            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                getKnightMoves(moves, turn, rank * file);
                break;
            case WHITE_BISHOP:
            case BLACK_BISHOP:
                getBishopMoves(moves, turn, rank * file);
                break;
            case WHITE_ROOK:
            case BLACK_ROOK:
                getRookMoves(moves, turn, rank * file);
                break;
            case WHITE_QUEEN:
            case BLACK_QUEEN:
                getQueenMoves(moves, turn, rank * file);
                break;
            case WHITE_KING:
            case BLACK_KING:
                getKingMoves(moves, turn, rank * file);
                break;
            default:
                assert(false);
            }
        }
    }
}

void getPawnMoves(std::vector<u16> &moves, bool turn, char square)
{
    Board &board = !turn ? whiteBoard : blackBoard;

    // moves
    assert(up_square(square) != -1);
    if (get_piece_at_square(board, up_square(square)) == '.')
    {
        moves.push_back((square) << 8 | up_square(square));
        if (is_square_in_rank(square, RANK_2) && get_piece_at_square(board, square + 2 * RANK_SIZE) == '.')
            moves.push_back((square) << 8 | (square + 2 * RANK_SIZE));
    }
    char upRight = square_to_file(square) == FILE_H ? -1 : up_square(square) + 1,
         upLeft = square_to_file(square) == FILE_A ? -1 : up_square(square) - 1;

    if (upRight != -1 &&
        ((!turn && (is_black_piece(get_piece_at_square(board, upRight)) || enPassantBlack[square_to_file(upRight)])) ||
         (turn && (is_white_piece(get_piece_at_square(board, upRight)) || enPassantWhite[square_to_file(upRight)]))))
        moves.push_back((square) << 8 | upRight);

    if (upLeft != -1 &&
            ((!turn && (is_black_piece(get_piece_at_square(board, upLeft)) || enPassantBlack[square_to_file(upLeft)])) ||
        (turn && (is_white_piece(get_piece_at_square(board, upLeft)) || enPassantWhite[square_to_file(upLeft)]))))
        moves.push_back((square) << 8 | upLeft);

    // take moves
}
void getKnightMoves(std::vector<u16> &moves, bool turn, char square)
{
    Board &board = !turn ? whiteBoard : blackBoard;

    static std::vector<std::vector<char>> knightMoveTable = []() {
        std::vector<std::vector<char>> table(64);
        for (int sq = 0; sq < 64; ++sq)
        {
            int rank = sq / 8, file = sq % 8;
            for (int i = 0; i < 8; ++i)
            {
                int dr = 0, df = 0;
                switch (i)
                {
                case 0:
                    dr = 2;
                    df = 1;
                    break;
                case 1:
                    dr = 2;
                    df = -1;
                    break;
                case 2:
                    dr = 1;
                    df = 2;
                    break;
                case 3:
                    dr = 1;
                    df = -2;
                    break;
                case 4:
                    dr = -2;
                    df = 1;
                    break;
                case 5:
                    dr = -2;
                    df = -1;
                    break;
                case 6:
                    dr = -1;
                    df = 2;
                    break;
                case 7:
                    dr = -1;
                    df = -2;
                    break;
                }
                int r2 = rank + dr, f2 = file + df;
                if (r2 >= 0 && r2 < 8 && f2 >= 0 && f2 < 8)
                    table[sq].push_back(r2 * 8 + f2);
            }
        }
        return table;
    }();

    for (char target : knightMoveTable[(int)square])
    {
        char targetPiece = get_piece_at_square(board, target);
        if (targetPiece == '.' || (!turn && is_black_piece(targetPiece)) || (turn && is_white_piece(targetPiece)))
        {
            moves.push_back((square << 8) | target);
        }
    }
}
void getBishopMoves(std::vector<u16> &moves, bool turn, char square)
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
                moves.push_back((square << 8) | target);
            }
            else
            {
                if ((!turn && is_black_piece(piece)) || (turn && is_white_piece(piece)))
                    moves.push_back((square << 8) | target);
                break; // blocked
            }
        }
    }
}

void getRookMoves(std::vector<u16> &moves, bool turn, char square)
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
                moves.push_back((square << 8) | target);
            }
            else
            {
                if ((!turn && is_black_piece(piece)) || (turn && is_white_piece(piece)))
                    moves.push_back((square << 8) | target);
                break; // blocked
            }
        }
    }
}

void getQueenMoves(std::vector<u16> &moves, bool turn, char square)
{
    getBishopMoves(moves, turn, square);
    getRookMoves(moves, turn, square);
}

void getKingMoves(std::vector<u16> &moves, bool turn, char square)
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
            moves.push_back((square << 8) | target);
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
    if (exists_piece(ourBoard.white_pawns, from))
    {
        remove_piece(ourBoard.white_pawns, from);
        remove_piece(opponentBoard.black_pawns, from);
    }
    else if (exists_piece(ourBoard.white_knights, from))
    {
        remove_piece(ourBoard.white_knights, from);
        remove_piece(opponentBoard.black_knights, from);
    }
    else if (exists_piece(ourBoard.white_bishops, from))
    {
        remove_piece(ourBoard.white_bishops, from);
        remove_piece(opponentBoard.black_bishops, from);
    }
    else if (exists_piece(ourBoard.white_rooks, from))
    {
        remove_piece(ourBoard.white_rooks, from);
        remove_piece(opponentBoard.black_rooks, from);
    }
    else if (exists_piece(ourBoard.white_queens, from))
    {
        remove_piece(ourBoard.white_queens, from);
        remove_piece(opponentBoard.black_queens, from);
    }
    else if (exists_piece(ourBoard.white_king, from))
    {
        remove_piece(ourBoard.white_king, from);
        remove_piece(opponentBoard.black_king, from);
    }

    // Handle captures - remove opponent piece from destination
    if (exists_piece(opponentBoard.white_pawns, to))
    {
        remove_piece(opponentBoard.white_pawns, to);
        remove_piece(ourBoard.black_pawns, to);
    }
    else if (exists_piece(opponentBoard.white_knights, to))
    {
        remove_piece(opponentBoard.white_knights, to);
        remove_piece(ourBoard.black_knights, to);
    }
    else if (exists_piece(opponentBoard.white_bishops, to))
    {
        remove_piece(opponentBoard.white_bishops, to);
        remove_piece(ourBoard.black_bishops, to);
    }
    else if (exists_piece(opponentBoard.white_rooks, to))
    {
        remove_piece(opponentBoard.white_rooks, to);
        remove_piece(ourBoard.black_rooks, to);
    }
    else if (exists_piece(opponentBoard.white_queens, to))
    {
        remove_piece(opponentBoard.white_queens, to);
        remove_piece(ourBoard.black_queens, to);
    }
    else if (exists_piece(opponentBoard.white_king, to))
    {
        remove_piece(opponentBoard.white_king, to);
        remove_piece(ourBoard.black_king, to);
    }

    // Handle en passant capture
    if ((piece == WHITE_PAWN || piece == BLACK_PAWN))
    {
        int fromFile = square_to_file(from);
        int toFile = square_to_file(to);
        if (fromFile != toFile)
        { // diagonal pawn move
            bool *enPassantArray = turn ? enPassantBlack : enPassantWhite;
            if (enPassantArray[toFile])
            {
                char capturedPawnSquare = turn ? (to - 8) : (to + 8);
                // Remove the en passant captured pawn
                if (turn)
                {
                    remove_piece(opponentBoard.white_pawns, capturedPawnSquare);
                    remove_piece(ourBoard.black_pawns, capturedPawnSquare);
                }
                else
                {
                    remove_piece(opponentBoard.white_pawns, capturedPawnSquare);
                    remove_piece(ourBoard.black_pawns, capturedPawnSquare);
                }
            }
        }
    }

    // Place piece on destination square on both boards
    switch (piece)
    {
    case WHITE_PAWN:
    case BLACK_PAWN:
        add_piece(ourBoard.white_pawns, to);
        add_piece(opponentBoard.black_pawns, to);
        break;
    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
        add_piece(ourBoard.white_knights, to);
        add_piece(opponentBoard.black_knights, to);
        break;
    case WHITE_BISHOP:
    case BLACK_BISHOP:
        add_piece(ourBoard.white_bishops, to);
        add_piece(opponentBoard.black_bishops, to);
        break;
    case WHITE_ROOK:
    case BLACK_ROOK:
        add_piece(ourBoard.white_rooks, to);
        add_piece(opponentBoard.black_rooks, to);
        break;
    case WHITE_QUEEN:
    case BLACK_QUEEN:
        add_piece(ourBoard.white_queens, to);
        add_piece(opponentBoard.black_queens, to);
        break;
    case WHITE_KING:
    case BLACK_KING:
        add_piece(ourBoard.white_king, to);
        add_piece(opponentBoard.black_king, to);
        break;
    }
}
