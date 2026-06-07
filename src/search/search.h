#pragma once
#include "board/board.h"
int negamax(Board&board, int depth, int alpha, int beta,int ply);
Move findBestMove(Board &board, int depth);