#pragma once
#include <string>
#include "board/board.h"
#include "move/move.h"
#include <cstdint>
class Board;
struct Move;
using u64 = uint64_t;
inline bool getBit(u64 bb, int sq)
{
    return (bb >> sq) & 1ULL;
}
inline Colour get_piece_colour(Piece p){
    if(p>= WP && p<=WK) return WHITE;
    return BLACK;
}
inline int pop_lsb(u64 &bb)
{
    int sq = __builtin_ctzll(bb);
    bb &= bb - 1;
    return sq;
}
inline u64 setBitZero(u64 num, int pos){
    return (num & ~(1ULL<<pos));
}
inline u64 setBitOne(u64 num, int pos){
    return (num | (1Ull<<pos));
}
Move algebraic_into_move(Board &board, std::string &uci_move);