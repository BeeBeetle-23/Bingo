#pragma once
#include "Board/board.h"
#include "move/move.h"
#include <cstdint>
enum TTflag{
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
};
struct TTEntry{
    uint64_t hash;
    int score;
    int depth;
    TTflag flag;
    Move best_move;
};
constexpr size_t TT_SIZE = 1 << 20;
inline TTEntry transposition_table[TT_SIZE];
namespace Zobrist {

    extern uint64_t pieces[12][64];
    extern uint64_t castling[16];
    extern uint64_t en_passant[64];
    extern uint64_t black_to_move;

    void init(uint64_t seed = 1070372531ULL);

}
inline size_t tt_index(uint64_t hash) {
    return hash & (TT_SIZE - 1);
}
TTEntry* probe(uint64_t hash);
void store(uint64_t hash,int score,int depth,TTflag flag,Move best_move);