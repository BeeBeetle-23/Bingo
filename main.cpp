#include <iostream>
#include "Board/board.h"
#include "Board/zobrist.h"
#include "movegen/attacks.h"
#include "perft/perft.h"
#include "search/search.h"
#include "move/move.h"
int main(){
    int depth = 5;
    initAttackTables();
    initMagicTables();
    Zobrist::init();
    Board board;
    board.Parse_FEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    findBestMove(board,7).move_into_algebraic();
    //std::cout<<perft(board,4)<<std::endl;
    //perft_divide(board,depth);
}