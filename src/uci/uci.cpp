#include <iostream>
#include <string>
#include "uci/uci.h"
#include "movegen/movegen.h"
#include <sstream>
#include "board/board.h"
#include "search/search.h"
#include "move/move.h"
#include "bitboard/bitboard.h"
#include "time/time.h"

void uci_loop(Board& board) {
    // Disable output buffering so commands print instantly to Cutechess
    std::setbuf(stdout, NULL); 
    
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "uci") {
            std::cout << "id name ChessEngineV3\n";
            std::cout << "id author Abhi\n";
            std::cout << "uciok\n";
        }
        else if (line == "isready") {
            std::cout << "readyok\n";
        }
        else if (line == "ucinewgame") {
            // Clean up hash tables / reset engine state if needed
        }
        else if (line.rfind("position", 0) == 0) {
            std::istringstream stream(line);
            std::string token;
            stream >> token; // skip "position"
            stream >> token; // get "startpos" or "fen"

            if (token == "startpos") {
                board.Parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                stream >> token;
            } 
            else if (token == "fen") {
                std::string fen_str = "";
                for (int i = 0; i < 6; i++) {
                    stream >> token;
                    fen_str += token + " ";
                }
                board.Parse_FEN(fen_str);
                stream >> token;
            }
            if (token == "moves") {
                std::string move_str;
                while (stream >> move_str) {
                    Move move = algebraic_into_move(board, move_str);
                    if (move == Move()) {
                        printf("Empty move");
                    }
                    else {    
                        board.make_move(move);
                    }
                }
            }
        }
        else if (line.rfind("go", 0) == 0) {
            std::istringstream stream(line);
            std::string token;
            
            // Initialize time tracking variables
            int wtime = -1, btime = -1, winc = 0, binc = 0;
            int depth = -1;

            // Look through the entire "go" command line string
            while (stream >> token) {
                if (token == "depth")      stream >> depth;
                else if (token == "wtime") stream >> wtime;
                else if (token == "btime") stream >> btime;
                else if (token == "winc")  stream >> winc;
                else if (token == "binc")  stream >> binc;
            }

            // Determine our engine's clock based on the side to move
            int my_time = (board.side_to_move == WHITE) ? wtime : btime;
            int my_inc  = (board.side_to_move == WHITE) ? winc  : binc;

            if (my_time != -1) {
                // Time control mode: initialize the countdown stopwatch
                TimeManager::init(my_time, my_inc);
                
                // If Cutechess didn't specify an explicit depth limit,
                // set it to max depth so it relies purely on the timer allocation
                if (depth == -1) {
                    depth = 64; 
                }
            } 
            else {
                // Fixed depth/infinite mode: give it a massive budget so it never aborts
                if (depth == -1) depth = 4; // Fallback default
                TimeManager::init(2000000000, 0); 
            }

            // Run search!
            Move best = findBestMove(board, depth);
            
            std::cout << "bestmove ";
            best.move_into_algebraic();
            std::cout << "\n";
        }
        else if (line == "quit") {
            break;
        }
    }
}