#ifndef PIECE_H
#define PIECE_H

#include <godot_cpp/core/binder_common.hpp>
#include <stdint.h>

namespace godot {

/// @brief  5-bit enum that represents a chess piece.
/// Bits 1-3 represent the type of piece, bit 4 represents the color,
/// Bit 5 represents whether it's a ghost piece or not (See Undo move for info) 
enum Piece : uint8_t {
    Empty = 0,
    King = 1,
    Queen = 2,
    Bishop = 3,
    Knight = 4,
    Rook = 5,
    Pawn = 6,

    White = 0b00000,
    Black = 0b01000
};

}

VARIANT_ENUM_CAST(Piece);

#endif