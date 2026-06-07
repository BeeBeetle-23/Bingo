#include "movegen/attacks.h"
#include "board/masks.h"
#include "move/move.h"
#include <unordered_set>
#include "board/board.h"
#include <cassert>
u64 king_attacks[64];
u64 knight_attacks[64];
u64 pawn_attacks[2][64];
u64 sliding_piece_attacks[108'000];
SMagic mRookTbl[64];
SMagic mBishopTbl[64];
u64 setOccupancy(int index, int bits_in_mask, u64 mask){
    u64 occupancy = 0ull;
    for(int i = 0; i<bits_in_mask; i++){
        int square = pop_lsb(mask);
        if(index&(1<<i)){
            occupancy |= (1ull<<square);
        }
    }
    return occupancy;
}

u64 bishopAttacks(u64 occ, Square sq) {
   u64* aptr = mBishopTbl[sq].ptr;
   occ      &= mBishopTbl[sq].mask;
   occ      *= mBishopTbl[sq].magic;
   occ     >>= mBishopTbl[sq].shift;
   return aptr[occ];
}

u64 rookAttacks(u64 occ, Square sq) {
   u64* aptr = mRookTbl[sq].ptr;
   occ      &= mRookTbl[sq].mask;
   occ      *= mRookTbl[sq].magic;
   occ     >>= mRookTbl[sq].shift;
   return aptr[occ];
}

u64 maskRookAttacks(Square sq) {
    u64 mask = 0ULL;
    int r = sq / 8;
    int f = sq % 8;
    
    for (int nr = r + 1; nr < 7; nr++) mask |= (1ULL << (nr * 8 + f));
    for (int nr = r - 1; nr > 0; nr--) mask |= (1ULL << (nr * 8 + f));
    for (int nf = f + 1; nf < 7; nf++) mask |= (1ULL << (r * 8 + nf));
    for (int nf = f - 1; nf > 0; nf--) mask |= (1ULL << (r * 8 + nf));
    return mask;
}

u64 maskBishopAttacks(Square sq) {
    u64 mask = 0ULL;
    int r = sq / 8;
    int f = sq % 8;
    
    for (int nr = r + 1, nf = f + 1; nr < 7 && nf < 7; nr++, nf++) mask |= (1ULL << (nr * 8 + nf));
    for (int nr = r + 1, nf = f - 1; nr < 7 && nf > 0; nr++, nf--) mask |= (1ULL << (nr * 8 + nf));
    for (int nr = r - 1, nf = f + 1; nr > 0 && nf < 7; nr--, nf++) mask |= (1ULL << (nr * 8 + nf));
    for (int nr = r - 1, nf = f - 1; nr > 0 && nf > 0; nr--, nf--) mask |= (1ULL << (nr * 8 + nf));
    return mask;
}

u64 maskKnightAttacks(Square sq)
{
    assert(sq >= 0 && sq < 64 && "square out of range");

    u64 attacks = 0ULL;
    u64 bit = 1ULL << sq;

    // +2 rank, +1 file: wraps off FILE_H
    if (bit & ~FILE_H)          attacks |= (bit << 17);

    // +2 rank, -1 file: wraps off FILE_A
    if (bit & ~FILE_A)          attacks |= (bit << 15);

    // +1 rank, +2 file: wraps off FILE_G or FILE_H
    if (bit & ~(FILE_G | FILE_H)) attacks |= (bit << 10);

    // +1 rank, -2 file: wraps off FILE_A or FILE_B
    if (bit & ~(FILE_A | FILE_B)) attacks |= (bit << 6);

    // -2 rank, -1 file: wraps off FILE_A
    if (bit & ~FILE_A)          attacks |= (bit >> 17);

    // -2 rank, +1 file: wraps off FILE_H
    if (bit & ~FILE_H)          attacks |= (bit >> 15);

    // -1 rank, -2 file: wraps off FILE_A or FILE_B
    if (bit & ~(FILE_A | FILE_B)) attacks |= (bit >> 10);

    // -1 rank, +2 file: wraps off FILE_G or FILE_H
    if (bit & ~(FILE_G | FILE_H)) attacks |= (bit >> 6);

    return attacks;
}

u64 maskKingAttacks(Square sq)
{
    u64 attacks = 0ULL;

    u64 bitboard = 1ULL << sq;

    // north
    attacks |= (bitboard << 8);

    // south
    attacks |= (bitboard >> 8);

    // east
    attacks |= (bitboard & ~FILE_H) << 1;

    // west
    attacks |= (bitboard & ~FILE_A) >> 1;

    // north-east
    attacks |= (bitboard & ~FILE_H) << 9;

    // north-west
    attacks |= (bitboard & ~FILE_A) << 7;

    // south-east
    attacks |= (bitboard & ~FILE_H) >> 7;

    // south-west
    attacks |= (bitboard & ~FILE_A) >> 9;

    return attacks;
}

u64 maskPawnAttacks(Square sq, Colour side)
{
    u64 attacks = 0ULL;
    u64 bitboard = 1ULL << sq;

    if (side == WHITE)
    {
        if (bitboard & ~FILE_A)
            attacks |= (bitboard << 7);

        if (bitboard & ~FILE_H)
            attacks |= (bitboard << 9);
    }
    else
    {
        if (bitboard & ~FILE_A)
            attacks |= (bitboard >> 9);

        if (bitboard & ~FILE_H)
            attacks |= (bitboard >> 7);
    }

    return attacks;
}

u64 generateRookAttacksOnTheFly(Square sq, u64 occupancy) {
    u64 attacks = 0ULL;
    int r = sq / 8;
    int f = sq % 8;

    // Ray Up
    for (int i = r + 1; i <= 7; i++) {
        attacks |= (1ULL << (i * 8 + f));
        if (occupancy & (1ULL << (i * 8 + f))) break; // Hit a blocker!
    }
    // Ray Down
    for (int i = r - 1; i >= 0; i--) {
        attacks |= (1ULL << (i * 8 + f));
        if (occupancy & (1ULL << (i * 8 + f))) break;
    }
    // Ray Right
    for (int i = f + 1; i <= 7; i++) {
        attacks |= (1ULL << (r * 8 + i));
        if (occupancy & (1ULL << (r * 8 + i))) break;
    }
    // Ray Left
    for (int i = f - 1; i >= 0; i--) {
        attacks |= (1ULL << (r * 8 + i));
        if (occupancy & (1ULL << (r * 8 + i))) break;
    }
    return attacks;
}

u64 generateBishopAttacksOnTheFly(Square sq, u64 occupancy) {
    u64 attacks = 0ULL;
    int r = sq / 8;
    int f = sq % 8;

    // Top Right
    for (int nr = r + 1, nf = f + 1; nr <= 7 && nf <= 7; nr++, nf++) {
        attacks |= (1ULL << (nr * 8 + nf));
        if (occupancy & (1ULL << (nr * 8 + nf))) break;
    }
    // Top Left
    for (int nr = r + 1, nf = f - 1; nr <= 7 && nf >= 0; nr++, nf--) {
        attacks |= (1ULL << (nr * 8 + nf));
        if (occupancy & (1ULL << (nr * 8 + nf))) break;
    }
    // Bottom Right
    for (int nr = r - 1, nf = f + 1; nr >= 0 && nf <= 7; nr--, nf++) {
        attacks |= (1ULL << (nr * 8 + nf));
        if (occupancy & (1ULL << (nr * 8 + nf))) break;
    }
    // Bottom Left
    for (int nr = r - 1, nf = f - 1; nr >= 0 && nf >= 0; nr--, nf--) {
        attacks |= (1ULL << (nr * 8 + nf));
        if (occupancy & (1ULL << (nr * 8 + nf))) break;
    }
    return attacks;
}

void initMagicTables() {
    u64* current_ptr = sliding_piece_attacks;

    for (int sq = 0; sq < 64; sq++) {
        // 1. Initialize Bishop Entry
        mBishopTbl[sq].mask = maskBishopAttacks((Square)sq);
        mBishopTbl[sq].magic = bishopmagics[sq];
        
        // CALCULATE SHIFT ON THE FLY!
        mBishopTbl[sq].shift = 64 - __builtin_popcountll(mBishopTbl[sq].mask); 
        
        mBishopTbl[sq].ptr = current_ptr;

        int bishop_bits = 64 - mBishopTbl[sq].shift; // This will equal your popcount!
        int bishop_occupancy_combos = 1 << bishop_bits;
        current_ptr += bishop_occupancy_combos; 

        for (int i = 0; i < bishop_occupancy_combos; i++) {
            u64 occ = setOccupancy(i, bishop_bits, mBishopTbl[sq].mask);
            u64 magic_index = (occ * mBishopTbl[sq].magic) >> mBishopTbl[sq].shift;
            u64 attacks = generateBishopAttacksOnTheFly((Square)sq,occ);
            if (mBishopTbl[sq].ptr[magic_index] != 0 && mBishopTbl[sq].ptr[magic_index] != attacks) {
                printf("Destructive BISHOP collision on square %d at index %llu!\n", sq, magic_index);
            }
            mBishopTbl[sq].ptr[magic_index] = attacks;
        }

        // 2. Initialize Rook Entry
        mRookTbl[sq].mask = maskRookAttacks((Square)sq);
        mRookTbl[sq].magic = rookmagics[sq];
        
        // CALCULATE SHIFT ON THE FLY!
        mRookTbl[sq].shift = 64 - __builtin_popcountll(mRookTbl[sq].mask); 
        
        mRookTbl[sq].ptr = current_ptr;

        int rook_bits = 64 - mRookTbl[sq].shift;
        int rook_occupancy_combos = 1 << rook_bits;
        current_ptr += rook_occupancy_combos;

        for (int i = 0; i < rook_occupancy_combos; i++) {
            u64 occ = setOccupancy(i, rook_bits, mRookTbl[sq].mask);
            u64 magic_index = (occ * mRookTbl[sq].magic) >> mRookTbl[sq].shift;
            u64 attacks = generateRookAttacksOnTheFly((Square)sq, occ);

            if (mRookTbl[sq].ptr[magic_index] != 0 && mRookTbl[sq].ptr[magic_index] != attacks) {
                printf("Destructive ROOK collision on square %d at index %llu!\n", sq, magic_index);
            }
            mRookTbl[sq].ptr[magic_index] = attacks;        
        }
        
    }
    
}

void initAttackTables()
{
    for (int i = 0; i < 64; i++)
    {
        knight_attacks[i] = maskKnightAttacks((Square)i);
        king_attacks[i] = maskKingAttacks((Square)i);
        pawn_attacks[WHITE][i] = maskPawnAttacks((Square)i, WHITE);
        pawn_attacks[BLACK][i] = maskPawnAttacks((Square)i, BLACK);
    }
}