#ifndef MOVE_H
#define MOVE_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/variant/string.hpp>

#include "piece.h"

namespace godot {
/// @brief A class to represent a move on the board.
class BoardMove : public Object {
    GDCLASS(BoardMove, Object)
private:

protected:
    static void _bind_methods();
public:
    int start_square;
    void set_start_square(const int start_square);
    int get_start_square() const;
    int target_square;
    void set_target_square(const int target_square);
    int get_target_square() const;
    Piece captured_piece;

    static BoardMove *make(const int start_square, const int target_square);
    BoardMove();
    ~BoardMove();

    String _to_string() const;
};

}

#endif