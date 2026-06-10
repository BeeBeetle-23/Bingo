#include <iostream>
#include "Board/board.h"
#include "Board/zobrist.h"
#include "movegen/attacks.h"
#include "perft/perft.h"
#include "search/search.h"
#include "uci/uci.h"
#include "constants.h"
#include "move/move.h"
#include "time/time.h"
int main(){
    initAttackTables();
    initMagicTables();
    Zobrist::init();
    TimeManager::init_infinite();
    Board board;int depth = 10;
    board.Parse_FEN("8/8/pp2pk2/1p1p2p1/1P1P2Pp/P3P2P/3K4/8 w - - 0 1");
    findBestMove(board,depth).move_into_algebraic();
    printf("Nodes: %llu\n", TimeManager::nodes);
    printf("Aborted: %d\n", TimeManager::search_aborted);
    printf("Probes: %d\nHits: %d\nCuttoffs: %d\nUpper: %d\nLower: %d\n",probes,hits,cutoffs,upper,lower);
    float hitrate = (100.0*hits)/probes; float cuttoffrate = (100.0*cutoffs)/probes;
    printf("HitRate: %.2f\nCuttoffRate: %.2f\n",hitrate,cuttoffrate);
    
}