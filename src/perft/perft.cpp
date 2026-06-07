#include "perft/perft.h"
#include "board/board.h"
#include "movegen/movegen.h"
#include <cstdint>
using u64 = uint64_t;
u64 perft(Board& board, int depth)
{
    if (depth == 0)
        return 1ULL;

    Move move_list[256];
    int move_count = 0;

    generateLegalMoves(board,move_list,board.side_to_move,&move_count);

    u64 nodes = 0ULL;

    for (int i = 0; i < move_count; i++)
    {
        board.make_move(move_list[i]);

        nodes += perft(board, depth - 1);

        board.undo_move();
    }

    return nodes;
}
void perft_divide(Board& board, int depth) {
    if (depth == 0) return;
    int legal_count = 0;
    u64 total_nodes = 0; // Kept consistent with u64 from perft
    Move move_list[256];
    
    
    generateLegalMoves(board, move_list, board.side_to_move, &legal_count);

    std::cout << "\n--- DIVIDE DEPTH " << depth << " ---\n";

    uint64_t root_hash = board.zobrist_hash;

for (int i = 0; i < legal_count; i++) {

    board.make_move(move_list[i]);

    /*if (board.zobrist_hash != board.computeHash()) {
        printf("BROKEN AFTER MAKE\n");
        exit(1);
    }*/

    u64 nodes_this_move = perft(board, depth - 1);
    total_nodes += nodes_this_move;

    board.undo_move();

    move_list[i].move_into_algebraic(); std::cout<<": "<<nodes_this_move<<"\n";
    if (board.zobrist_hash != root_hash) {
        printf("BROKEN AFTER UNDO\n");
        exit(1);
    }
}

    std::cout << "\nTotal Nodes: " << total_nodes << "\n\n";
}