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
//Captures
void generatePawnCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateKnightCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateQueenCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateRookCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateKingCaptures(const Board &board, Move *move_list, Colour colour, int *move_count);
void generateCaptures(Board &board, Move *move_list, Colour colour, int *move_count);
