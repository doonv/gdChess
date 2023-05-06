#ifndef BOARD_H
#define BOARD_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "piece.h"
#include "move.h"

namespace godot {

class Board : public Object {
    GDCLASS(Board, Object)

private:

protected:
    static void _bind_methods();

public:
    /// @brief A 1 dimensional array representing all the pieces on the board.
    /// This is a 1 dimensional array because gdscript doesn't support 2D typed arrays.
    /// This is a pointer because it can have a dynamic length. (board size can change)
    Piece *pieces;
    // void set_pieces(const TypedArray<Piece> pieces);
    // We use `Array` instead of `TypedArray` because `TypedArray` gives us a weird error
    // TODO: Fix that error
    Array get_pieces() const;
    /// @brief A dynamic array representing the moves on the board.
    TypedArray<BoardMove> moves;
    TypedArray<BoardMove> get_moves() const;

    /// @brief The dimensions of the board.
    Vector2i size;
    void set_size(const Vector2i p_size);
    Vector2i get_size() const;
    Vector2 get_size_vector() const;
    int get_width() const;
    void set_width(const int width);
    int get_height() const;
    void set_height(const int height);
    int get_area() const;

    String pos_to_str(const int pos) const;
    String get_opening_name() const;

    void setv(const Vector2i position, const Piece value);
    Piece getv(const Vector2i position) const;

    /// @brief Generates all the moves in the current position.
    /// @return An array containing all the possible moves in the position.
    TypedArray<BoardMove> generate_moves() const;
    /// @brief Generates all the moves in the current position.
    /// @return An array containing all the possible moves in the position.
    TypedArray<BoardMove> generate_moves_for_piece(const int piece_pos) const;
    // TypedArray<BoardMove> generate_moves_gd();
    // BoardMove *generate_moves_for_piece(Piece piece, BoardMove *moves = nullptr);
    /// @brief Performs a move on the board.
    void do_move(BoardMove *move);
    /// @brief Reverts a move.
    /// Capture moves are handled by making all moves not replace, but swap the squares in the move.
    /// The only difference is the piece that got captured is marked as a "ghost".
    /// And then when the move is undone, the ghost gets revived.
    void undo_move(const BoardMove *move);

    String move_to_uci(const BoardMove *move) const;
    String move_to_pgn(const BoardMove *move) const;

    String fen() const;
    
    static Board *from_fen(const String fen);

    Board();
    // Board(Vector2 size);
    // Board(Vector2 size, Piece pieces[]);
    ~Board();
};


}




#endif