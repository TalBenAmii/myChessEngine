#ifndef MOVE_H
#define MOVE_H
#include "board.h"

void getMoves(Board &board,unordered_set<string> &moves, bool turn);
void getPawnMoves(Board &board,unordered_set<string> &moves, bool turn, char square);
void getKnightMoves(Board &board,unordered_set<string> &moves, char square);
void getSlidingMoves(Board &board,unordered_set<string> &moves, char square);
void makeMove(Board &board,u16 move, bool turn);

#endif /* MOVE_H */
