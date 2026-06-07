#include <iostream>
#include "Board/board.h"
#include "Board/zobrist.h"
#include "movegen/attacks.h"
#include "perft/perft.h"
#include "move/move.h"
int main(){
    initAttackTables();
    initMagicTables();
    Zobrist::init();
    Board board;
    board.Parse_FEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    std::cout<<perft(board,5)<<std::endl;
}