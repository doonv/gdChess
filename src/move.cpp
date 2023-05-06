#include "move.h"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;


BoardMove::BoardMove() {

}
BoardMove::~BoardMove() {

}

void BoardMove::set_start_square(const int start_square) {
    this->start_square = start_square;
}
int BoardMove::get_start_square() const {
    return start_square;
}

void BoardMove::set_target_square(const int target_square) {
    this->target_square = target_square;
}
int BoardMove::get_target_square() const {
    return target_square;
}

BoardMove *BoardMove::make(const int start_square, const int target_square) {
    BoardMove *move = memnew(BoardMove());
    move->start_square = start_square;
    move->target_square = target_square;
    return move;
}

void BoardMove::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_start_square"), &BoardMove::get_start_square);
    ClassDB::bind_method(D_METHOD("set_start_square", "start_square"), &BoardMove::set_start_square);
    ClassDB::add_property("BoardMove", PropertyInfo(Variant::INT, "start_square"),
            "set_start_square", "get_start_square");
    
    ClassDB::bind_method(D_METHOD("get_target_square"), &BoardMove::get_target_square);
    ClassDB::bind_method(D_METHOD("set_target_square", "target_square"), &BoardMove::set_target_square);
    ClassDB::add_property("BoardMove", PropertyInfo(Variant::INT, "target_square"),
            "set_target_square", "get_target_square");

    ClassDB::bind_static_method("BoardMove", D_METHOD("make", "start_square", "target_square"), &BoardMove::make);
}

String BoardMove::_to_string() const {
    return String("(Start: {0}, End: {1})").format(Array::make(start_square, target_square));
}

