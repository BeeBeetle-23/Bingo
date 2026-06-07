#pragma once
#include <cstdint>
class Board;
extern const char* arr[64];
enum Square : uint8_t {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    NO_SQUARE
};

enum MoveFlag : uint8_t {
    QUIET,
    DOUBLE_PAWN_PUSH,
    KING_CASTLE,
    QUEEN_CASTLE,
    CAPTURE,
    EP_CAPTURE,

    KNIGHT_PROMOTION,
    BISHOP_PROMOTION,
    ROOK_PROMOTION,
    QUEEN_PROMOTION,

    KNIGHT_PROMO_CAPTURE,
    BISHOP_PROMO_CAPTURE,
    ROOK_PROMO_CAPTURE,
    QUEEN_PROMO_CAPTURE
};

struct Move {

    uint16_t data = 0;

    constexpr Move() = default;

    constexpr Move(
        Square from,
        Square to,
        MoveFlag flag = QUIET
    )
        : data(
            from |
            (to << 6) |
            (flag << 12)
        )
    {}

    constexpr Square from() const {
        return Square(data & 0x3F);
    }

    constexpr Square to() const {
        return Square((data >> 6) & 0x3F);
    }

    constexpr MoveFlag flag() const {
        return MoveFlag((data >> 12) & 0x0F);
    }
    void move_into_algebraic() const;
    constexpr bool operator==(const Move& other) const {
        return data == other.data;
    }

    constexpr bool operator!=(const Move& other) const {
        return data != other.data;
    }
};

struct ScoredMove{
    Move move[256];
    int score[256];
};