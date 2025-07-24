#include "board.h"
#include "defs.h"
#include "move.h"

Board whiteBoard = initialWhiteBoard;

int main()
{
    // initBoard();
    unordered_set<string> moves;
    bool turn = 0;
    string strMove;
    
    while (true)
    {
        printFullBoard(whiteBoard);
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