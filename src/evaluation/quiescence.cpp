#include "board/board.h"
#include "evaluation/evaluation.h"
#include "evaluation/quiescence.h"
#include "movegen/movegen.h"
#define INF 90000
EvalInfo pv_leaf_breakdown;
const int MVV_LVA[6][6] = {
// attacker: P  N  B  R  Q  K
    {15,14,13,12,11,10}, // victim pawn
    {25,24,23,22,21,20}, // victim knight
    {35,34,33,32,31,30}, // victim bishop
    {45,44,43,42,41,40}, // victim rook
    {55,54,53,52,51,50}, // victim queen
    {0,0,0,0,0,0}
};
const int piece_value[12] = {
    100, 320, 335, 500, 900, 20000, // White: P, N, B, R, Q, K
    100, 320, 335, 500, 900, 20000  // Black: p, n, b, r, q, k
};
int getMVVLVA(const Board& board, Move move) {
    // Get attacker type (0-5 regardless of colour)
    int attacker = board.piece_on[move.from()] % 6;
    int victim   = board.piece_on[move.to()]   % 6;
    return MVV_LVA[victim][attacker];
}
int getCapturedPieceValue(Board &board, Move move){
    if(move.flag() != CAPTURE) return 0;
    int piece = board.piece_on[move.to()];
    return piece_value[piece];
}
int Quiesce(Board &board, int alpha, int beta,bool is_pv) {
    bool isincheck = is_in_check(board);
    ScoredMove move_list;
    int move_count = 0;
    int static_eval = evaluate(board, is_pv ? &pv_leaf_breakdown : nullptr);
    int best_value = static_eval;

    if (!isincheck) {
        // Stand pat
        if (best_value >= beta)
            return best_value;
        if (best_value > alpha)
            alpha = best_value;

        const int DELTA = 900;
        if (static_eval + DELTA < alpha) return alpha;

        generatePseudoCaptures(board, move_list.move, board.side_to_move, &move_count);
    } 
    else {
        // If in check, we must generate all pseudolegal moves to escape check
        generatePseudoLegalMoves(board, move_list.move, board.side_to_move, &move_count);
        best_value = -INF; 
    }

    // Assign MVV-LVA to everything
    for (int i = 0; i < move_count; i++) {
        move_list.score[i] = getMVVLVA(board, move_list.move[i]); 
    }

    // Main search loop
    for (int i = 0; i < move_count; i++) {
        // Selection sort step
        int best = i;
        for (int j = i + 1; j < move_count; j++) {
            if (move_list.score[j] > move_list.score[best]) best = j;
        }
        std::swap(move_list.move[i],  move_list.move[best]);
        std::swap(move_list.score[i], move_list.score[best]);

        // Delta pruning for captures (only relevant if not in check)
        if (!isincheck) {
            int captured_value = getCapturedPieceValue(board, move_list.move[i]);
            if (static_eval + captured_value + 200 < alpha) continue;
        }

        
        Colour us = board.side_to_move;
        board.make_move(move_list.move[i]);
        if (board.isSquareAttacked(board.king_square[us], board.side_to_move)) {
            board.undo_move();
            continue;
        }
        int score = -Quiesce(board, -beta, -alpha,is_pv);
        board.undo_move();

        if (score >= beta)
            return score;
        if (score > best_value)
            best_value = score;
        if (score > alpha)
            alpha = score;
    }

    return best_value;
}