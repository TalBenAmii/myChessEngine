#ifndef MOVE_H
#define MOVE_H
#include "board.h"

void getMoves(std::vector<u16> &moves, bool turn);
void getPawnMoves(std::vector<u16> &moves, bool turn, char square);
void getKnightMoves(std::vector<u16> &moves, bool turn, char square);
void getBishopMoves(std::vector<u16> &moves, bool turn, char square);
void getRookMoves(std::vector<u16> &moves, bool turn, char square);
void getQueenMoves(std::vector<u16> &moves, bool turn, char square);
void getKingMoves(std::vector<u16> &moves, bool turn, char square);
void makeMove(u16 move, bool turn);

#endif /* MOVE_H */
