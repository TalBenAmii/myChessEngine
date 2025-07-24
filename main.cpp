#include "board.h"
#include "move.h"
#include "defs.h"

Board whiteBoard = initialWhiteBoard, blackBoard = initialBlackBoard;

int main()
{
    // initBoard();
    printFullBoard(whiteBoard);
    std::vector<u16> whiteMoves,blackMoves;
    getMoves(whiteMoves,0);
    for(auto a:whiteMoves)printf("%s ",move_to_string(a).c_str());
    makeMove(whiteMoves[0],0);
    makeMove(string_to_move("d2d3"),0);
    makeMove(string_to_move("e2e4"),0);
    printFullBoard(whiteBoard);

    return 0;
}