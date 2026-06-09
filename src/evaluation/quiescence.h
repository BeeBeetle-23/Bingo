#pragma once
#include "Board/board.h"
#include "move/move.h"
int getMVVLVA(const Board& board, Move move);
int getCapturedPieceValue(Board &board, Move move);
int Quiesce(Board &board, int alpha, int beta,bool is_pv);