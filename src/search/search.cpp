#include <cstring>
#include "search/search.h"
#include "Board/board.h"
#include "evaluation/evaluation.h"
#include "move/move.h"
#include "movegen/movegen.h"
#include "evaluation/quiescence.h"
#include "Board/zobrist.h"
#include "constants.h"
#include "time/time.h"
// Time control globals
extern EvalInfo pv_leaf_breakdown;

Move pv[MAX_DEPTH][MAX_DEPTH];
int pv_length[MAX_DEPTH];

int negamax(Board &board, int depth, int alpha, int beta, int ply, bool is_pv) {
    TimeManager::nodes++;
    TimeManager::check_time();

    if (TimeManager::search_aborted)
        return 0;

    if (ply >= MAX_PLY)
        return Quiesce(board, alpha, beta, is_pv);

    pv_length[ply] = ply;

    int original_alpha = alpha;

    Move tt_move = Move();
    Move best_move_this_node = Move();

    TTEntry *tt = probe(board.zobrist_hash);

    if (tt) {
        if (tt->best_move != Move())
            tt_move = tt->best_move;

        // Do not allow TT cutoffs at root. Root move must come from generated legal moves.
        if (ply > 0 && tt->depth >= depth) {
            int tt_score = score_from_tt(tt->score, ply);

            switch (tt->flag) {
                case EXACT:
                    return tt_score;

                case LOWER_BOUND:
                    alpha = std::max(alpha, tt_score);
                    break;

                case UPPER_BOUND:
                    beta = std::min(beta, tt_score);
                    break;
            }

            if (alpha >= beta)
                return tt_score;
        }
    }

    if (depth == 0)
        return Quiesce(board, alpha, beta, is_pv);

    ScoredMove move_list;
    int legalCount = 0;

    generateLegalMoves(board, move_list.move, board.side_to_move, &legalCount);

    if (legalCount == 0)
        return is_in_check(board) ? -MATE + ply : 0;

    // Avoid using stale pv[ply][ply]. TT move is only used for ordering.
    scoreMoves(board, move_list, legalCount, Move(), tt_move);

    int best_score = -INF;
    bool is_first_move = true;

    for (int i = 0; i < legalCount; i++) {
        int best = i;

        for (int j = i + 1; j < legalCount; j++) {
            if (move_list.score[j] > move_list.score[best])
                best = j;
        }

        std::swap(move_list.move[i], move_list.move[best]);
        std::swap(move_list.score[i], move_list.score[best]);

        Move move = move_list.move[i];

        board.make_move(move);

        bool child_is_pv = is_pv && is_first_move;
        int score = -negamax(board, depth - 1, -beta, -alpha, ply + 1, child_is_pv);

        board.undo_move();

        if (TimeManager::search_aborted)
            return 0;

        if (score > best_score) {
            best_score = score;
            best_move_this_node = move;
        }

        if (score >= beta) {
            store(board.zobrist_hash, score, depth, LOWER_BOUND, move, ply);
            return score;
        }

        if (score > alpha) {
            alpha = score;

            pv[ply][ply] = move;

            if (ply + 1 < MAX_PLY) {
                for (int j = ply + 1; j < pv_length[ply + 1]; j++)
                    pv[ply][j] = pv[ply + 1][j];

                pv_length[ply] = pv_length[ply + 1];
            } else {
                pv_length[ply] = ply + 1;
            }
        }

        is_first_move = false;
    }

    if (TimeManager::search_aborted)
        return 0;

    TTflag flag;

    if (best_score <= original_alpha)
        flag = UPPER_BOUND;
    else
        flag = EXACT;

    store(board.zobrist_hash, best_score, depth, flag, best_move_this_node, ply);

    return best_score;
}

Move findBestMove(Board &board, int max_depth) {
    ScoredMove root_moves;
    int root_count = 0;

    generateLegalMoves(board, root_moves.move, board.side_to_move, &root_count);

    if (root_count == 0) {
        printf("NO LEGAL ROOT MOVES: in_check=%d hash=%llu side=%d\n",
            is_in_check(board),
            (unsigned long long)board.zobrist_hash,
            board.side_to_move);
        return Move();
    }

    // Legal fallback, so timeout before depth 1 cannot return a1a1.
    Move best_move = root_moves.move[0];
    int score = 0;

    max_depth = std::min(max_depth, MAX_DEPTH);

    for (int depth = 1; depth <= max_depth; depth++) {
        std::memset(pv_length, 0, sizeof(pv_length));

        score = negamax(board, depth, -INF, INF, 0, true);

        if (TimeManager::search_aborted)
            break;

        // pv[0][0] can only be set by searching generated legal root moves now.
        if (pv_length[0] > 0 && pv[0][0] != Move())
            best_move = pv[0][0];

        /*
        printf("Depth: %d | Total Score: %d cp\n", depth, score);
        */
    }

    return best_move;
}