#include <cstring>
#include "search/search.h"
#include "Board/board.h"
#include "evaluation/evaluation.h"
#include "move/move.h"
#include "movegen/movegen.h"
#define INF 90000
#define MAX_MOVES 256
#define MAX_DEPTH 64
Move pv[MAX_DEPTH][MAX_DEPTH];
int pv_length[MAX_DEPTH];
int negamax(Board &board, int depth, int alpha, int beta,int ply){
    if(depth == 0) return evaluate(board);
    int best_score = -INF;
    Move move_list[MAX_MOVES];
    int legalCount = 0;
    generateLegalMoves(board,move_list,board.side_to_move,&legalCount);
    for(int i = 0; i<legalCount; i++){
        board.make_move(move_list[i]);
        int score = -negamax(board,depth-1,-beta,-alpha,ply + 1);
        board.undo_move();
        if(score > best_score){
            best_score = score;
            if(score>alpha){
                alpha = score;
                pv[ply][ply] = move_list[i];
                for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
                    pv[ply][next_ply] = pv[ply + 1][next_ply];
                }
                pv_length[ply] = pv_length[ply + 1];
            }
        }
        if(score>=beta){
            return best_score;
        }
    }
    return best_score;
}
Move findBestMove(Board &board, int depth){
    Move best_move;
    for(int i = 1; i<depth; i++){
        std::memset(pv_length, 0, sizeof(pv_length));
        negamax(board,i,-INF,INF,0);
        best_move = pv[0][0];
    }
    return best_move;
}