#include "piece.h"
#include "board.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <algorithm>
#include <sstream>
#include <godot_cpp/classes/file_access.hpp>

using namespace godot;

String Board::move_to_uci(const BoardMove *move) const {
    const char str[4] = {
       'a' + (move->start_square % size.x), '0' + (size.x - (move->start_square / size.x)),
       'a' + (move->target_square % size.x), '0' + (size.x - (move->target_square / size.x)),
    };
    return String(str);
}

Board::Board() {
    // Initialize any variables here.
    this->size = Vector2(8, 8);
    this->pieces = new Piece[get_area()];
    this->moves = TypedArray<BoardMove>();
    std::fill_n(pieces, get_area(), Piece::Empty);
}
// Board::Board(Vector2 size) {
//     // Initialize any variables here.
//     this->size = size;
//     this->pieces = new Piece[get_area()];
//     std::fill_n(pieces, get_area(), Piece::Empty);
// }
// Board::Board(Vector2 size, Piece pieces[]) {
//     // Initialize any variables here.
//     this->size = size;
//     this->pieces = pieces;
// }

Board::~Board() {
    // Add your cleanup here.
    std::free(pieces);
}

// void Board::set_pieces(const TypedArray<Piece> new_pieces) {
//     pieces = new Piece[new_pieces.size()];
//     for (size_t i = 0; i < new_pieces.size(); i++)
//     {
//         pieces[i] = *cast_to<Piece>(new_pieces[i]);
//     }    
//     pieces = pieces;
// }



Array Board::get_pieces() const {
    Array variants = Array();
    for (size_t i = 0; i < get_area(); i++)
    {
        variants.append(Variant(pieces[i]));
    }
    return variants;
}

TypedArray<BoardMove> Board::get_moves() const {
    return moves;
}

void Board::set_size(const Vector2i p_size) { size = p_size; }
Vector2i Board::get_size() const { return size; }
Vector2 Board::get_size_vector() const { return Vector2(size); }
int Board::get_width() const { return size.x; }
void Board::set_width(const int p_width) { size.x = p_width; }
int Board::get_height() const { return size.y; }
void Board::set_height(const int p_height) { size.y = p_height; }
int Board::get_area() const { return size.x * size.y; }

/// @brief Sets the piece in a square located at the position specified.
/// @param position The position of the square.
/// @param value The piece to set.
void Board::setv(const Vector2i position, const Piece value) {
    pieces[position.x + (position.y * size.x)] = value;
}
/// @brief Gets the piece in a square located at the position specified.
/// @param position The position of the square.
/// @return The piece of that square.
Piece Board::getv(const Vector2i position) const {
    return pieces[position.x + (position.y * size.x)];
}


TypedArray<BoardMove> Board::generate_moves() const {
    TypedArray<BoardMove> generated_moves = TypedArray<BoardMove>();

    for (size_t i = 0; i < get_area() * sizeof(Piece); i++)
    {
        switch (pieces[i] & ~Piece::Black) // Get the piece without its color.
        {
            case Pawn:
                generated_moves.push_back(Variant(BoardMove::make(i, i - size.y)));
                break;
        }
    }

    return generated_moves;
}

TypedArray<BoardMove> Board::generate_moves_for_piece(const int piece_pos) const {
    TypedArray<BoardMove> generated_moves = TypedArray<BoardMove>();
    /// @brief This is a number converted to a boolean.
    /// If the piece is white, its zero so it gets converted to `false`. Opposite with black.
    const Piece isBlack = (Piece)(pieces[piece_pos] & Piece::Black);
    // This macro adds a move to the list if:
    //  1. The target is in bounds.
    //  2. The target piece's color is opposite of origin piece's color.
    // TODO: Make this method faster or someth
    #define TRY_ADD(target) \
        if ((0 < target && target < get_area()) && (((pieces[target] & 0b111) == Piece::Empty) | ((pieces[target] & Piece::Black) != isBlack))) \
            generated_moves.push_back(Variant(BoardMove::make(piece_pos, target))); 
    // 0b00000
    // 0b01000
    // 0b01001
    switch (pieces[piece_pos] & ~Piece::Black) // Get the piece without its color.
    {
        case Pawn:
            if (isBlack) {
                generated_moves.push_back(Variant(BoardMove::make(piece_pos, piece_pos + size.y)));
            } else {
                generated_moves.push_back(Variant(BoardMove::make(piece_pos, piece_pos - size.y)));
            }
            break;
        case King:
            TRY_ADD(piece_pos - 1);
            TRY_ADD(piece_pos + 1);
            TRY_ADD(piece_pos - size.x - 1);
            TRY_ADD(piece_pos - size.x);
            TRY_ADD(piece_pos - size.x + 1);
            TRY_ADD(piece_pos + size.x - 1);
            TRY_ADD(piece_pos + size.x);
            TRY_ADD(piece_pos + size.x + 1);
            break;
        case Knight:
            TRY_ADD(piece_pos - (size.x * 2) - 1);
            TRY_ADD(piece_pos - (size.x * 2) + 1);
            TRY_ADD(piece_pos + (size.x * 2) - 1);
            TRY_ADD(piece_pos + (size.x * 2) + 1);
            TRY_ADD(piece_pos + (size.x * 1) - 2);
            TRY_ADD(piece_pos - (size.x * 1) - 2);
            TRY_ADD(piece_pos + (size.x * 1) + 2);
            TRY_ADD(piece_pos - (size.x * 1) + 2);
            break;
    }

    return generated_moves;
}
// TypedArray<Board::BoardMove> Board::generate_moves_gd() {
//     TypedArray<BoardMove> moves = TypedArray<BoardMove>(Variant()));
//     return moves;
// }
// Board::BoardMove *Board::generate_moves_for_piece(Piece piece, BoardMove *moves = nullptr) {
//     return moves;
// }
void Board::do_move(BoardMove *move) {
    if (move->start_square == move->target_square) return;
    // Save the captured piece
    move->captured_piece = pieces[move->target_square];
    // Do the move.
    pieces[move->target_square] = pieces[move->start_square];
    pieces[move->start_square] = Piece::Empty;

    // Add the move to the list.
    moves.push_back(Variant(move));
}

void Board::undo_move(const BoardMove *move) {
    if (move->start_square == move->target_square) return;
    // Undo the move.
    pieces[move->start_square] = pieces[move->target_square];
    pieces[move->target_square] = move->captured_piece;
    // Remove the move from the list.
    moves.erase(Variant(move));
}
Board *Board::from_fen(const String fen) {  
    constexpr std::string_view PieceToChar(" KQBNRP  kqbnrp");
    unsigned char token;
    size_t idx;
    int sq = 0;
    std::stringstream ss(fen.utf8().get_data());
    
    // Determine width
    int width = 8;
    const int length = width * 8;
    Piece *pieces = new Piece[length];
    std::fill_n(pieces, length, Piece::Empty);
    while ((ss >> token) && !isspace(token))
    {
        if (isdigit(token))
            sq += (token - '0'); // Advance the given number of files
        else if (token == '/')
            continue;
        else if ((idx = PieceToChar.find(token)) != std::string::npos) {
            pieces[sq] = (Piece)idx;
            ++sq;
        }
    }
    Board *board = memnew(Board());
    board->pieces = pieces;
    board->size = Vector2(width, length / width);
    return board;
}

String Board::pos_to_str(const int pos) const {
    const char format_string[] = {
        'a' + (pos % size.x),
        '0' + (size.y - (pos / size.x)),
        '\0'
    };
    return String(format_string);
}

String Board::get_opening_name() const {
    if (moves.size() < 1) return String("");
    // // Our opening database (lichess-org/chess-openings) splits up chess openings into
    // // ECO classifications (Go google what "ECO chess" means).
    // // The following code identifies which classification the following move fits in to.
    // // (We don't combine the classifications because 1. I'm lazy and 2. Smaller files are (probably) faster.)
    // const BoardMove *move = cast_to<BoardMove>(moves[0]);
    // char opening_classification = 'p';
    // if (55 >= move->start_square && move->start_square >= 48) {
    //     // Pawn move
    //     switch (move->target_square) {
    //         case 34: // c4 English Opening
    //         case 41: // b3 Larsen's Opening
    //         case 46: // g3 King's Fianchetto Opening, also known as Benko's Opening
    //         case 40: // a3 Anderssen's Opening
    //         case 32: // a4 Ware Opening
    //         case 33: // b4 Sokolsky Opening, also known as the Polish Opening or the Orangutan Opening
    //         case 42: // c3 Saragossa Opening
    //         case 45: // f3 Barnes Opening, also known as Gedult's Opening
    //         case 38: // g4 Grob's Attack
    //         case 47: // h3 Clemenz Opening, or Basman's Attack
    //         case 39: // h4 Desprez Opening, or Kadas Opening
    //             // Flank opening (ECO A)
    //             opening_classification = 'A';
    //         case 36: // e4 The King's Pawn opening
    //             // This case is special because we need to check if the next move
    //             // is followed up by e5 by black.
    //             if (moves.size() >= 2 && cast_to<BoardMove>(moves[1])->target_square == 28) {
    //                 // There at at least 2 half-moves played and the second half-move was pawn to e5.
    //                 opening_classification = 'C'; // Open Game
    //             } else {
    //                 // Either the second half-move wasnt played or it wasn't pawn to e5
    //                 opening_classification = 'B'; // Semi Open Game
    //             }
    //         case 35: // d4 The Queen's pawn opening
    //             // Similar as d4 above but for the closed game / indian defenses.
    //             if (moves.size() >= 2 && cast_to<BoardMove>(moves[1])->target_square == 21) {
    //                 // There at at least 2 half-moves played and the second half-move was knight to f6.
    //                 opening_classification = 'E'; // Indian defense
    //             } else {
    //                 // Either the second half-move wasnt played or it wasn't knight to f6
    //                 opening_classification = 'D'; // Closed Game
    //             }
    //     }
    // } else {
    //     // Other move (Horsie move)
    //     switch (move->target_square) {
    //         case 45: // Nf3 Zukertort Opening
    //         case 40: // Na3 Durkin Opening, also known as Durkin's Attack or the Sodium Attack
    //         case 42: // Nc3 Dunst Opening
    //         case 47: // Nh3 Amar Opening, also known as Paris Opening
    //             // Flank opening (ECO A)
    //             opening_classification = 'A';
    //     }
    // }
    // Read the right file
    // char file_name[] = "res://openings/x.tsv";
    // file_name[15] = opening_classification;

    // UtilityFunctions::print(file_name);

    // if (FileAccess::file_exists(file_name) == false)
    //     return String("Opening database \"{0}\" does not exist.").format(Array::make(file_name));
    Ref<FileAccess> file = FileAccess::open("res://openings/openings.tsv", FileAccess::READ);
    String opening = String("");
    while (file->get_position() < file->get_length()) {
        PackedStringArray line = file->get_line().split("\t");
        PackedStringArray opening_moves = line[1].split(" ");
        int matching_moves_count = 0;
        for (size_t i = 0; i < UtilityFunctions::mini(opening_moves.size(), moves.size()); i++)
        {
            if (move_to_uci(cast_to<BoardMove>(moves[i])) == opening_moves[i]) {
                matching_moves_count++;
            } else {
                break;
            }
        }
        // UtilityFunctions::print(opening_moves, " --- ", opening_moves.size(), " = ", moves.size(), " = ", matching_moves_count);
        // This is a mess because A==B==C doesnt work how I thought it would.
        if (
            matching_moves_count == opening_moves.size() &&
            opening_moves.size() == moves.size() &&
            moves.size() == matching_moves_count
        ) {
            opening = line[0];
            break;
        }
    }
    file->close();


    return opening;
}

String Board::move_to_pgn(const BoardMove *move) const {
    char piece_type = '?';
    switch (pieces[move->start_square])
    {
        case Piece::Bishop: piece_type = 'b'; break;
        case Piece::Queen: piece_type = 'q'; break;
        case Piece::Pawn: piece_type = 'p'; break;
        case Piece::King: piece_type = 'k'; break;
        case Piece::Knight: piece_type = 'n'; break;
        case Piece::Rook: piece_type = 'r'; break;
    }
    const char move_str[4] = {
        piece_type,
        'a' + (move->target_square % size.x), '0' + (size.y - (move->target_square / size.x)),
        '\0'
    };
    return String(move_str);
}

String Board::fen() const {
    char text[get_area() + size.y + 1];
    size_t text_index = 0;
    char empty_spaces_count = 0;
    for (int i = 0; i < get_area(); i++)
    {
        bool end_of_row = i % size.x == size.x - 1;
        
        if (empty_spaces_count > 0 && ((pieces[i] & 0b111) != Piece::Empty)) {
            text[text_index] = '0' + empty_spaces_count;
            text_index++;
            empty_spaces_count = 0;
        }
        switch (pieces[i] & ~Piece::Black)
        {
            case Piece::Bishop: text[text_index] = 'b'; break;
            case Piece::Queen: text[text_index] = 'q'; break;
            case Piece::Pawn: text[text_index] = 'p'; break;
            case Piece::King: text[text_index] = 'k'; break;
            case Piece::Knight: text[text_index] = 'n'; break;
            case Piece::Rook: text[text_index] = 'r'; break;
            default:
                empty_spaces_count++;
                text_index--;
                break;
        }
        if (end_of_row) {
            if (empty_spaces_count > 0) {
                text_index++;
                text[text_index] = '0' + empty_spaces_count;
                empty_spaces_count = 0;
            }
            text_index++;
            text[text_index] = '/';
        }
        text_index++;
    }
    text[text_index-1] = '\0';
    return String(text);
}


void Board::_bind_methods() {
    // Size
    ClassDB::bind_method(D_METHOD("get_size"), &Board::get_size);
    ClassDB::bind_method(D_METHOD("set_size", "p_size"), &Board::set_size);
    ClassDB::add_property("Board", PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
    
    ClassDB::bind_method(D_METHOD("get_size_vector"), &Board::get_size_vector);
    ClassDB::add_property("Board", PropertyInfo(Variant::VECTOR2, "size_vector"), "", "get_size_vector");
    
    ClassDB::bind_method(D_METHOD("get_width"), &Board::get_width);
    ClassDB::bind_method(D_METHOD("set_width", "width"), &Board::set_width);
    ClassDB::add_property("Board", PropertyInfo(Variant::INT, "width"), "set_width", "get_width");
    
    ClassDB::bind_method(D_METHOD("get_height"), &Board::get_height);
    ClassDB::bind_method(D_METHOD("set_height", "height"), &Board::set_height);
    ClassDB::add_property("Board", PropertyInfo(Variant::INT, "height"), "set_height", "get_height");
    
    ClassDB::bind_method(D_METHOD("get_area"), &Board::get_area);
    ClassDB::add_property("Board", PropertyInfo(Variant::INT, "area"), "", "get_area");

    // Pieces
    ClassDB::bind_method(D_METHOD("get_pieces"), &Board::get_pieces);
    ClassDB::add_property("Board", PropertyInfo(Variant::ARRAY, "pieces"), "", "get_pieces");

    ClassDB::bind_method(D_METHOD("get_moves"), &Board::get_moves);
    ClassDB::add_property("Board", PropertyInfo(Variant::ARRAY, "moves"), "", "get_moves");

    // Methods
    ClassDB::bind_method(D_METHOD("setv", "position", "value"), &Board::setv);
    ClassDB::bind_method(D_METHOD("getv", "position"), &Board::getv);

    ClassDB::bind_method(D_METHOD("do_move", "move"), &Board::do_move);
    ClassDB::bind_method(D_METHOD("undo_move", "move"), &Board::undo_move);

    ClassDB::bind_method(D_METHOD("get_opening_name"), &Board::get_opening_name);
    ClassDB::bind_method(D_METHOD("pos_to_str", "pos"), &Board::pos_to_str);
    ClassDB::bind_method(D_METHOD("move_to_pgn", "move"), &Board::move_to_pgn);
    ClassDB::bind_method(D_METHOD("move_to_uci", "move"), &Board::move_to_uci);
    ClassDB::bind_method(D_METHOD("fen"), &Board::fen);

    ClassDB::bind_method(D_METHOD("generate_moves"), &Board::generate_moves);
    ClassDB::bind_method(D_METHOD("generate_moves_for_piece", "piece_pos"), &Board::generate_moves_for_piece);


    // Static methods
    ClassDB::bind_static_method("Board", D_METHOD("from_fen", "fen"), &Board::from_fen);

    BIND_ENUM_CONSTANT(Empty);
    BIND_ENUM_CONSTANT(King);
    BIND_ENUM_CONSTANT(Queen);
    BIND_ENUM_CONSTANT(Bishop);
    BIND_ENUM_CONSTANT(Knight);
    BIND_ENUM_CONSTANT(Rook);
    BIND_ENUM_CONSTANT(Pawn);

    BIND_ENUM_CONSTANT(White);
	BIND_ENUM_CONSTANT(Black);
}
