#include "Board\board.h"
#include "evaluation/evaluation.h"
#include "bitboard/bitboard.h"
const int piece_value[6] = {100,300,315,550,925,2500};
int evaluate(Board &board){
    int score = 0;
    for(int i = 0; i<6; i++){
        u64 piece = board.pieces[i];
        int number = std::popcount(piece);
        score += number*piece_value[i];
    }
    for(int i = 6; i<12; i++){
        u64 piece = board.pieces[i];
        int number = std::popcount(piece);
        score -= number*piece_value[i];
    }
    int multiplier = (board.side_to_move == WHITE)?1:-1;
    return multiplier*score;
}