#include "movegen/movegen.h"
#include "board/board.h"
#include "board/zobrist.h"
#include "move/move.h"
#include "board/masks.h"
#include "movegen/attacks.h"
#include "bitboard/bitboard.h"
#include <bit>
#include <cassert>
static constexpr int MAX_MOVES = 256;
using u64 = uint64_t;

void generatePawnMoves(const Board &board, Move *move_list, Colour colour, int *move_count) {
    assert(move_list  != nullptr        && "move_list is null");
    assert(move_count != nullptr        && "move_count is null");
    assert(*move_count >= 0             && "move_count is negative");
    assert(*move_count < MAX_MOVES      && "move_list already full on entry");
    assert((board.ep_square == NO_SQUARE ||board.ep_square < 64    )     && "ep_square out of range");

    const u64 ep_bitboard = (board.ep_square != NO_SQUARE)?(1ULL << board.ep_square):0ULL;
    const u64 empty_squares = ~board.occupancies[BOTH];

    const auto push_move = [&](Move m) {
        move_list[(*move_count)++] = m;
    };

    if (colour == WHITE) {
        const u64 enemy_pieces = board.occupancies[BLACK];

        // --- Pushes ---
        const u64 single_pushes = (board.pieces[WP] << 8) & empty_squares;
        const u64 double_pushes = ((single_pushes & RANK_3) << 8) & empty_squares;

        u64 normal_pushes = single_pushes & ~RANK_8;
        while (normal_pushes) {
            const int to_sq = pop_lsb(normal_pushes);
            push_move(Move((Square)(to_sq - 8), (Square)to_sq, QUIET));
        }

        u64 promo_pushes = single_pushes & RANK_8;
        while (promo_pushes) {
            const int    to_sq   = pop_lsb(promo_pushes);
            const Square from_sq = (Square)(to_sq - 8);
            push_move(Move(from_sq, (Square)to_sq, QUEEN_PROMOTION));
            push_move(Move(from_sq, (Square)to_sq, ROOK_PROMOTION));
            push_move(Move(from_sq, (Square)to_sq, BISHOP_PROMOTION));
            push_move(Move(from_sq, (Square)to_sq, KNIGHT_PROMOTION));
        }

        u64 dbl = double_pushes;
        while (dbl) {
            const int to_sq = pop_lsb(dbl);
            push_move(Move((Square)(to_sq - 16), (Square)to_sq, DOUBLE_PAWN_PUSH));
        }

        // --- Captures & En Passant (Using precalculated pawnAttacks) ---
        u64 pawns = board.pieces[WP];
        while (pawns) {
            const int from_sq = pop_lsb(pawns);
            
            // Replaced function call with fast array lookup
            const u64 attacks = pawn_attacks[WHITE][from_sq]; 

            u64 captures = attacks & enemy_pieces;
            while (captures) {
                const int to_sq = pop_lsb(captures);
                if ((1ULL << to_sq) & RANK_8) {
                    push_move(Move((Square)from_sq, (Square)to_sq, QUEEN_PROMO_CAPTURE));
                    push_move(Move((Square)from_sq, (Square)to_sq, ROOK_PROMO_CAPTURE));
                    push_move(Move((Square)from_sq, (Square)to_sq, BISHOP_PROMO_CAPTURE));
                    push_move(Move((Square)from_sq, (Square)to_sq, KNIGHT_PROMO_CAPTURE));
                } else {
                    push_move(Move((Square)from_sq, (Square)to_sq, CAPTURE));
                }
            }

            u64 ep = attacks & ep_bitboard;
            while (ep) {
                const int to_sq = pop_lsb(ep);
                assert(((1ULL << to_sq) & RANK_6) && "white EP target not on rank 6");
                push_move(Move((Square)from_sq, (Square)to_sq, EP_CAPTURE));
            }
        }

    } else { // BLACK

        const u64 enemy_pieces = board.occupancies[WHITE];

        // --- Pushes ---
        const u64 single_pushes = (board.pieces[BP] >> 8) & empty_squares;
        const u64 double_pushes = ((single_pushes & RANK_6) >> 8) & empty_squares;

        u64 normal_pushes = single_pushes & ~RANK_1;
        while (normal_pushes) {
            const int to_sq = pop_lsb(normal_pushes);
            push_move(Move((Square)(to_sq + 8), (Square)to_sq, QUIET));
        }

        u64 promo_pushes = single_pushes & RANK_1;
        while (promo_pushes) {
            const int    to_sq   = pop_lsb(promo_pushes);
            const Square from_sq = (Square)(to_sq + 8);
            push_move(Move(from_sq, (Square)to_sq, QUEEN_PROMOTION));
            push_move(Move(from_sq, (Square)to_sq, ROOK_PROMOTION));
            push_move(Move(from_sq, (Square)to_sq, BISHOP_PROMOTION));
            push_move(Move(from_sq, (Square)to_sq, KNIGHT_PROMOTION));
        }

        u64 dbl = double_pushes;
        while (dbl) {
            const int to_sq = pop_lsb(dbl);
            push_move(Move((Square)(to_sq + 16), (Square)to_sq, DOUBLE_PAWN_PUSH));
        }

        // --- Captures & En Passant (Using precalculated pawnAttacks) ---
        u64 pawns = board.pieces[BP];
        while (pawns) {
            const int from_sq = pop_lsb(pawns);
            
            // Replaced function call with fast array lookup
            const u64 attacks = pawn_attacks[BLACK][from_sq]; 

            u64 captures = attacks & enemy_pieces;
            while (captures) {
                const int to_sq = pop_lsb(captures);
                if ((1ULL << to_sq) & RANK_1) {
                    push_move(Move((Square)from_sq, (Square)to_sq, QUEEN_PROMO_CAPTURE));
                    push_move(Move((Square)from_sq, (Square)to_sq, ROOK_PROMO_CAPTURE));
                    push_move(Move((Square)from_sq, (Square)to_sq, BISHOP_PROMO_CAPTURE));
                    push_move(Move((Square)from_sq, (Square)to_sq, KNIGHT_PROMO_CAPTURE));
                } else {
                    push_move(Move((Square)from_sq, (Square)to_sq, CAPTURE));
                }
            }

            u64 ep = attacks & ep_bitboard;
            while (ep) {
                const int to_sq = pop_lsb(ep);
                assert(((1ULL << to_sq) & RANK_3) && "black EP target not on rank 3");
                push_move(Move((Square)from_sq, (Square)to_sq, EP_CAPTURE));
            }
        }
    }
}

void generateKnightMoves(const Board &board, Move *move_list, Colour colour, int *move_count) {
    assert(move_list  != nullptr   && "move_list is null");
    assert(move_count != nullptr   && "move_count is null");
    assert(*move_count >= 0        && "move_count negative");
    assert(*move_count < MAX_MOVES && "move_list full on entry");

    const auto push_move = [&](Move m) {
        assert(*move_count < MAX_MOVES && "move_list overflow in generateKnightMoves");
        move_list[(*move_count)++] = m;
    };

    const u64 us     = board.occupancies[colour];
    const u64 enemy  = board.occupancies[colour == WHITE ? BLACK : WHITE];
    u64 knights      = board.pieces[colour == WHITE ? WN : BN];

    while (knights) {
        int from_sq = pop_lsb(knights);

        u64 attacks = knight_attacks[from_sq] & ~us;
        while (attacks) {
            const int to_sq = __builtin_ctzll(attacks);
            attacks &= attacks - 1;

            const MoveFlag flag = (1ULL << to_sq) & enemy ? CAPTURE : QUIET;
            push_move(Move((Square)from_sq, (Square)to_sq, flag));
        }
    }
}

void generateKingMoves(const Board &board, Move *move_list, Colour colour, int *move_count) {
    /*assert(move_list  != nullptr   && "move_list is null");
    assert(move_count != nullptr   && "move_count is null");
    assert(*move_count >= 0        && "move_count negative");
    assert(*move_count < MAX_MOVES && "move_list full on entry");*/

    const auto push_move = [&](Move m) {
        //assert(*move_count < MAX_MOVES && "move_list overflow in generateKingMoves");
        move_list[(*move_count)++] = m;
    };

    const u64 us    = board.occupancies[colour];
    const u64 enemy = board.occupancies[colour == WHITE ? BLACK : WHITE];
    u64 king        = board.pieces[colour == WHITE ? WK : BK];

    // Exactly one king must exist
    //assert(__builtin_popcountll(king) == 1 && "king bitboard does not have exactly 1 bit");

    while (king) {
        const int from_sq = __builtin_ctzll(king);
        king &= king - 1;

        u64 attacks = king_attacks[from_sq] & ~us;
        while (attacks) {
            const int to_sq = __builtin_ctzll(attacks);
            attacks &= attacks - 1;

            const MoveFlag flag = (1ULL << to_sq) & enemy ? CAPTURE : QUIET;
            push_move(Move((Square)from_sq, (Square)to_sq, flag));
        }
    }

    // Castling — structure unchanged, just routed through push_move
    if (colour == WHITE) {
        if (board.castling_rights & WK_CASTLE) {
            if (!(board.occupancies[BOTH] & ((1ULL << f1) | (1ULL << g1)))) {
                if (!board.isSquareAttacked(e1, BLACK) &&
                    !board.isSquareAttacked(f1, BLACK) &&
                    !board.isSquareAttacked(g1, BLACK)) {
                    push_move(Move(e1, g1, KING_CASTLE));
                }
            }
        }
        if (board.castling_rights & WQ_CASTLE) {
            if (!(board.occupancies[BOTH] & ((1ULL << b1) | (1ULL << c1) | (1ULL << d1)))) {
                if (!board.isSquareAttacked(e1, BLACK) &&
                    !board.isSquareAttacked(d1, BLACK) &&
                    !board.isSquareAttacked(c1, BLACK)) {
                    push_move(Move(e1, c1, QUEEN_CASTLE));
                }
            }
        }
    } else {
        if (board.castling_rights & BK_CASTLE) {
            if (!(board.occupancies[BOTH] & ((1ULL << f8) | (1ULL << g8)))) {
                if (!board.isSquareAttacked(e8, WHITE) &&
                    !board.isSquareAttacked(f8, WHITE) &&
                    !board.isSquareAttacked(g8, WHITE)) {
                    push_move(Move(e8, g8, KING_CASTLE));
                }
            }
        }
        if (board.castling_rights & BQ_CASTLE) {
            if (!(board.occupancies[BOTH] & ((1ULL << b8) | (1ULL << c8) | (1ULL << d8)))) {
                if (!board.isSquareAttacked(e8, WHITE) &&
                    !board.isSquareAttacked(d8, WHITE) &&
                    !board.isSquareAttacked(c8, WHITE)) {
                    push_move(Move(e8, c8, QUEEN_CASTLE));
                }
            }
        }
    }
}

void generateBishopMoves(const Board &board, Move *move_list, Colour colour, int *move_count) {
    
    u64 bishops = (colour == WHITE)?board.pieces[WB]:board.pieces[BB];
    u64 us = (colour == WHITE)?board.occupancies[WHITE]:board.occupancies[BLACK];
    u64 them = (colour == BLACK)?board.occupancies[WHITE]:board.occupancies[BLACK];
    while(bishops){
        int from = pop_lsb(bishops);
        u64 attacks = bishopAttacks(board.occupancies[BOTH],(Square)from);
        attacks &= ~us;
        while(attacks){
            int to = pop_lsb(attacks);
            MoveFlag flag = ((1ull << to) & them)?CAPTURE:QUIET;
            move_list[(*move_count)++] = Move((Square)from,(Square)to,flag);
        }
    }
}

void generateRookMoves(const Board &board, Move *move_list, Colour colour, int *move_count) {
    
    u64 rooks = (colour == WHITE)?board.pieces[WR]:board.pieces[BR];
    u64 us = (colour == WHITE)?board.occupancies[WHITE]:board.occupancies[BLACK];
    u64 them = (colour == BLACK)?board.occupancies[WHITE]:board.occupancies[BLACK];
    while(rooks){
        int from = pop_lsb(rooks);
        u64 attacks = rookAttacks(board.occupancies[BOTH],(Square)from);
        attacks &= ~us;
        while(attacks){
            int to = pop_lsb(attacks);
            MoveFlag flag = ((1ull << to) & them)?CAPTURE:QUIET;
            move_list[(*move_count)++] = Move((Square)from,(Square)to,flag);
        }
    }
}

void generateQueenMoves(const Board &board, Move *move_list, Colour colour, int *move_count) {
    
    u64 queens = (colour == WHITE)?board.pieces[WQ]:board.pieces[BQ];
    u64 us = (colour == WHITE)?board.occupancies[WHITE]:board.occupancies[BLACK];
    u64 them = (colour == BLACK)?board.occupancies[WHITE]:board.occupancies[BLACK];
    while(queens){
        int from = pop_lsb(queens);
        u64 attacks = bishopAttacks(board.occupancies[BOTH],(Square)from) | rookAttacks(board.occupancies[BOTH],(Square)from);
        attacks &= ~us;
        while(attacks){
            int to = pop_lsb(attacks);
            MoveFlag flag = ((1ull << to) & them)?CAPTURE:QUIET;
            move_list[(*move_count)++] = Move((Square)from,(Square)to,flag);
        }
    }
}

void generateLegalMoves(Board &board,Move *legal_move_list,Colour colour,int *legal_count)
{
    int movecount = 0;

    Move move_list[MAX_MOVES];

    Colour attacker =
        (colour == WHITE)
        ? BLACK
        : WHITE;

    generatePawnMoves(board, move_list, colour, &movecount);
    generateKnightMoves(board, move_list, colour, &movecount);
    generateBishopMoves(board, move_list, colour, &movecount);
    generateRookMoves(board, move_list, colour, &movecount);
    generateQueenMoves(board, move_list, colour, &movecount);
    generateKingMoves(board, move_list, colour, &movecount);

    *legal_count = 0;

    for (int i = 0; i < movecount; i++)
    {
        board.make_move(move_list[i]);

        if (!board.isSquareAttacked(board.king_square[colour],attacker))
        {
            legal_move_list[*legal_count] =
                move_list[i];

            (*legal_count)++;
        }

        board.undo_move();
    }
}