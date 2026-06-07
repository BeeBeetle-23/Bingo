#pragma once
#include "board/board.h"
#include <cstdint>
using u64 = uint64_t;
u64 perft(Board& board, int depth);
void perft_divide(Board& board, int depth);