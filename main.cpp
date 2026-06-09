#include <iostream>
#include "Board/board.h"
#include "Board/zobrist.h"
#include "movegen/attacks.h"
#include "perft/perft.h"
#include "search/search.h"
#include "uci/uci.h"
#include "move/move.h"
int main(){
    initAttackTables();
    initMagicTables();
    Zobrist::init();
    Board board;
    //board.Parse_FEN("3Q4/5pk1/6p1/6P1/2N2r2/1P2R3/6B1/6K1 b - - 0 48");
    //findBestMove(board,5).move_into_algebraic();
    uci_loop(board);
    //std::cout<<perft(board,depth)<<std::endl;
    //perft_divide(board,5);
}