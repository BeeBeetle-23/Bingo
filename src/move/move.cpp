#include "move/move.h"
#include <iostream>
const char* arr[64] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};

void Move::move_into_algebraic() const {
    std::cout<<arr[from()]<<arr[to()];
    MoveFlag f = flag();
    if(!(f == QUIET || f== DOUBLE_PAWN_PUSH || f == CAPTURE || f == KING_CASTLE ||
        f == QUEEN_CASTLE || f == EP_CAPTURE)){
            if(f == KNIGHT_PROMO_CAPTURE || f == KNIGHT_PROMOTION)std::cout<<"n";
            else if(f== QUEEN_PROMO_CAPTURE || f == QUEEN_PROMOTION)std::cout<<"q";
            else if(f== ROOK_PROMO_CAPTURE || f == ROOK_PROMOTION)std::cout<<"r";
            else if(f== BISHOP_PROMO_CAPTURE || f == BISHOP_PROMOTION)std::cout<<"b";
            

        }
}