#pragma once
#include <cstdint>
#include <move/move.h>
#include <string>
#include <iostream>
using u64 = uint64_t;
constexpr int MATE_SCORE = 999000;
enum Colour : uint8_t{
    WHITE, BLACK, BOTH
};
enum Piece : uint8_t {
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK,
    NO_PIECE
};
enum Castling : uint8_t {
    WK_CASTLE = 1,
    WQ_CASTLE = 2,
    BK_CASTLE = 4,
    BQ_CASTLE = 8
};
struct Undo {

    Move move;

    uint8_t castling_rights;

    Square ep_square;

    uint16_t halfmove_clock;

    Piece captured_piece;
    
    uint64_t zobrist_hash;
};

class Board {

public:

    // Piece bitboards
    u64 pieces[12];

    //Piece On board
    Piece piece_on[64];

    // Occupancies
    u64 occupancies[3];

    // Side to move
    Colour side_to_move;

    //Zobrist Hash
    u64 zobrist_hash;

    // Castling rights
    uint8_t castling_rights;

    // En passant square
    Square ep_square;

    // Rule bookkeeping
    uint16_t halfmove_clock;
    uint16_t fullmove_number;

    // Cached king squares
    Square king_square[2];

    // Move history
    Undo history[1024];

    int ply = 0;

public:

    void make_move(Move move);

    void undo_move();

    void add_piece(Piece piece, Square sq);
    void remove_piece(Piece piece, Square sq);
    void move_piece(Piece piece, Square from, Square to);
    Piece promotion_piece(Colour side, MoveFlag flag) const;
    bool isSquareAttacked(Square square,Colour attacker_colour) const;
    void clearBoard();
    void Parse_FEN(const std::string& fen);
    Piece char_to_piece(char c);
    char piece_to_char(Square from);
    u64 computeHash() const;
    void print_board();
};
inline bool is_in_check(const Board &board){
    Colour us = board.side_to_move;
    Colour them = static_cast<Colour>(board.side_to_move^1);
    if(board.isSquareAttacked(board.king_square[us],them)) return true;
    return false;
}