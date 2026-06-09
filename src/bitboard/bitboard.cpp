#include "bitboard/bitboard.h"
#include "move/move.h"
#include "movegen/movegen.h"
#include <board/board.h>
#include <string>
Move algebraic_into_move(Board &board, std::string &uci_move){
    std::string from = uci_move.substr(0,2);
    std::string to = uci_move.substr(2,2);
    char promo = (uci_move.length() == 5)?uci_move[4]:' ';
    Move move_list[256]; int legalCount = 0;
    generateLegalMoves(board, move_list, board.side_to_move, &legalCount);
    for(int i = 0; i<legalCount; i++){
        std::string legal_from = arr[move_list[i].from()];
        std::string legal_to = arr[move_list[i].to()];
        if(legal_from == from && legal_to == to){
            MoveFlag flag = move_list[i].flag();
            if(promo != ' '){
                if(promo == 'q' && (flag == QUEEN_PROMOTION || flag == QUEEN_PROMO_CAPTURE)) return move_list[i];
                if(promo == 'r' && (flag == ROOK_PROMOTION || flag == ROOK_PROMO_CAPTURE)) return move_list[i];
                if(promo == 'b' && (flag == BISHOP_PROMOTION || flag == BISHOP_PROMO_CAPTURE)) return move_list[i];
                if(promo == 'n' && (flag == KNIGHT_PROMOTION || flag == KNIGHT_PROMO_CAPTURE)) return move_list[i];
            }
            return move_list[i];
        }

    }
    return Move();

}