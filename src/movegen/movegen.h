#pragma once
#include "board\board.h"
#include "move\move.h"
void generatePawnMoves(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateKnightMoves(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateBishopMoves(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateQueenMoves(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateRookMoves(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateKingMoves(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateLegalMoves(Board &board,Move *legal_move_list,Colour colour,int *legal_count);
inline void generatePseudoLegalMoves(Board &board, Move *move_list, Colour colour, int *movecount){
    generatePawnMoves(board, move_list, colour, movecount);
    generateKnightMoves(board, move_list, colour, movecount);
    generateBishopMoves(board, move_list, colour, movecount);
    generateRookMoves(board, move_list, colour, movecount);
    generateQueenMoves(board, move_list, colour, movecount);
    generateKingMoves(board, move_list, colour, movecount);
}
//Captures
void generatePawnCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateKnightCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateBishopCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateQueenCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateRookCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateKingCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateCaptures(Board &board, Move *move_list, Colour colour, int *move_count);
inline void generatePseudoCaptures(Board &board, Move *move_list, Colour colour, int *movecount){
    generatePawnCaptures(board, move_list, colour, movecount);
    generateKnightCaptures(board, move_list, colour, movecount);
    generateBishopCaptures(board, move_list, colour, movecount);
    generateRookCaptures(board, move_list, colour, movecount);
    generateQueenCaptures(board, move_list, colour, movecount);
    generateKingCaptures(board, move_list, colour, movecount);
}