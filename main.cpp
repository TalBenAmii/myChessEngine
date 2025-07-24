#include "board.h"
#include "defs.h"
#include "move.h"

Board whiteBoard = initialWhiteBoard, blackBoard = initialBlackBoard;

int main()
{
    // initBoard();
    unordered_set<string> moves;
    bool turn = 0;
    string strMove;
    
    while (true)
    {
        Board &board = !turn ? whiteBoard : blackBoard;
        printFullBoard(board);
        moves.clear();
        getMoves(moves, turn);
        for (auto a : moves)
            printf("%s ", a.c_str());
        cout << endl;
        cin >> strMove;
        while (!moves.count(strMove))
        {
            cout << "Illegal move" << endl;
            cin >> strMove;
        }
        makeMove(string_to_move(strMove), turn);
        turn = !turn;
    }

    return 0;
}