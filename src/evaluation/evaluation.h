#pragma once
#include "Board/board.h"
#include "move/move.h"
struct EvalInfo{
    int piece = 0;
    int psqt = 0;
    int mobility = 0;
    int king_zone_attacks = 0;
    int doubled_pawns = 0;
    int passed_pawns = 0;
    int isolated_pawns = 0;
    int phalanx_pawns = 0;
    int threats = 0;
    int bishop_pair = 0; 
    int rooks_semi = 0;
};

int evaluate(Board &board,EvalInfo *info);
void scoreMoves(const Board &board, ScoredMove moves,int count, Move pvmove,Move ttmove);
