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
    board.Parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    findBestMove(board,7).move_into_algebraic();
    //std::cout<<perft(board,4)<<std::endl;
    //perft_divide(board,depth);
}