#ifndef MOVE_H
#define MOVE_H
#include "board.h"

void getMoves(unordered_set<string> &moves, bool turn);
void getPawnMoves(unordered_set<string> &moves, bool turn, char square);
void getKnightMoves(unordered_set<string> &moves, bool turn, char square);
void getBishopMoves(unordered_set<string> &moves, bool turn, char square);
void getRookMoves(unordered_set<string> &moves, bool turn, char square);
void getQueenMoves(unordered_set<string> &moves, bool turn, char square);
void getKingMoves(unordered_set<string> &moves, bool turn, char square);
void makeMove(u16 move, bool turn);

#endif /* MOVE_H */
