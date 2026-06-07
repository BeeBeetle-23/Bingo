#pragma once
#include <cstdint>
#include "move/move.h"
#include "board/board.h"
#include "bitboard/bitboard.h"
using u64 = uint64_t;
extern u64 knight_attacks[64];
extern u64 king_attacks[64];
extern u64 pawn_attacks[2][64];
extern u64 sliding_piece_attacks[108'000];

struct SMagic {
   u64* ptr;  // pointer to attack_table for each particular square
   u64 mask;  // to mask relevant squares of both lines (no outer squares)
   u64 magic; // magic 64-bit factor
   int shift; // shift right
};

extern SMagic mBishopTbl[64];
extern SMagic mRookTbl[64];
u64 bishopAttacks(u64 occ, Square sq);
u64 rookAttacks(u64 occ, Square sq);
u64 setOccupancy(int index, int bits_in_mask, u64 mask);
void initMagicTables();
void initAttackTables();
u64 maskBishopAttacks(Square sq);
u64 maskRookAttacks(Square sq);
u64 maskKnightAttacks(Square sq);
u64 maskKingAttacks(Square sq);
u64 maskPawnAttacks(Square sq, Colour colour);