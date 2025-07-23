#ifndef MOVE_H
#define MOVE_H
#include "board.h"
#include <vector>
void getPawnMoves(Board &board, std::vector<char> &moves,bool turn,char square);
void getKnightMoves(Board &board, std::vector<char> &moves, bool turn,char square);
void getBishopMoves(Board &board, std::vector<char> &moves, bool turn,char square);
void getRookMoves(Board &board, std::vector<char> &moves, bool turn,char square);
void getQueenMoves(Board &board, std::vector<char> &moves, bool turn,char square);
void getKingMoves(Board &board, std::vector<char> &moves, bool turn,char square);
#endif /* MOVE_H */
