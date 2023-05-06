extends Object
class_name BoardOlde

## A class that represents the pieces on the board
##
## Insert description here

## The
## [url=https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation][b]FEN[/b] (Forsyth–Edwards Notation)[/url]
## string for the starting position of a regular chess game.
const start_pos: StringName = &"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

var size: Vector2i = Vector2i(8, 8)
var size_vec: Vector2:
	get: return size as Vector2
	set(value): size = value as Vector2i
var width: int:
	get: return size.x
	set(value): size.x = value
var height: int:
	get: return size.y
	set(value): size.y = value 
var area: int:
	get: return width * height
	set(value): Vector2i(sqrt(value) as int, sqrt(value) as int)


## A 1D [Array] of [enum Piece]s that represent a chess board.
## [br]This array is 1 dimensional because GDScript doesn't support 2D typed arrays.
var pieces: Array[Piece]

## Indicates if it's blacks turn or not.
var blacks_turn: bool = false

class MoveOld extends Object:
	var start_pos: Vector2i
	var end_pos: Vector2i
	
	func _init(start_position: Vector2i, end_position: Vector2i) -> void:
		start_pos = start_position
		end_pos = end_position
	
	func _to_string() -> String:
		return "(start_pos: %s, end_pos: %s)" % [start_pos, end_pos]

## Represents a piece on the board, along with its [Color].
enum Piece {
	Empty = -1, ## Represents an empty tile with no piece.
	King = 0,
	Queen = 1,
	Bishop = 2,
	Knight = 3,
	Rook = 4,
	Pawn = 5,
	
	White = 0,
	Black = 8
}

## Creates an [Array] of [enum Piece]s from a 
## [url=https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation][b]FEN[/b] (Forsyth–Edwards Notation)[/url]
## string.
## [br]If you are trying to create an instance of [BoardOld], please use [method from_fen] instead.
static func get_fen(fen: String) -> Array[Piece]:
	var new_pieces: Array[Piece] = []
	for c in fen:
		if c.is_valid_int():
			for i in range(c.to_int()):
				new_pieces.push_back(Piece.Empty)
			continue
		# White pieces use UPPERCASE
		# Black pieces use lowercase
		var piece_color: Piece = Piece.Black if c == c.to_lower() else Piece.White
		
		match c.to_lower():
			'p':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Pawn | piece_color)
			'n':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Knight | piece_color)
			'b':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Bishop | piece_color)
			'r':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Rook | piece_color)
			'k':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.King | piece_color)
			'q':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Queen | piece_color)
			'/': continue # TODO: Add the proper functionality
	return new_pieces

## Creates a new instance [BoardOld] from a 
## [url=https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation][b]FEN[/b] (Forsyth–Edwards Notation)[/url]
## string.
static func from_fen(fen: String) -> BoardOlde:
	var new_pieces: Array[Piece] = []
#	var max_width: int = 0
#	var current_width: int = 0
	for c in fen:
		if c.is_valid_int():
			for i in range(c.to_int()):
				new_pieces.push_back(Piece.Empty)
			continue
		# White pieces use UPPERCASE
		# Black pieces use lowercase
		var piece_color: Piece = Piece.Black if c == c.to_lower() else Piece.White
		
		match c.to_lower():
			'p':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Pawn | piece_color)
			'n':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Knight | piece_color)
			'b':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Bishop | piece_color)
			'r':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Rook | piece_color)
			'k':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.King | piece_color)
			'q':
				@warning_ignore("int_as_enum_without_cast") 
				new_pieces.push_back(Piece.Queen | piece_color)
			'/':
#				if (max_width < current_width):
#					max_width = current_width
#				else:
#					# Add missing empty spots
#					for i in range(max_width - current_width):
#						new_pieces.push_back(Piece.Empty)
#				current_width = 0
				
				continue
#		current_width += 1
	@warning_ignore("integer_division")
#	return Board.new(new_pieces, Vector2i(max_width, new_pieces.size() / max_width))
	return BoardOlde.new(new_pieces)

## Get the [enum Piece] at the coordinates of the [Vector2i].
func getv(vector: Vector2i) -> Piece:
	return pieces[vector.x + (vector.y * width)]
## Set the [enum Piece] at the coordinates of the [Vector2i].
func setv(vector: Vector2i, value: Piece) -> void:
	pieces[vector.x + (vector.y * width)] = value

# Creates a brand new [BoardOld]
func _init(board_pieces: Array[Piece], board_size: Vector2i = Vector2i(8, 8)) -> void:
	size = board_size
	pieces = board_pieces
	pieces.resize(area) # Resize the array so we don't get out of range errors.

func generate_moves() -> Array[MoveOld]:
	var moves: Array[MoveOld] = []
	for y in range(height):
		for x in range(width):
			var pos: Vector2i = Vector2i(x, y)
			moves.append_array(get_moves_for_piece(pos))
	
	return moves

func get_moves_for_piece(piece_pos: Vector2i) -> Array[MoveOld]:
	var moves: Array[MoveOld] = []
	var piece: Piece = (getv(piece_pos) & 0b0111) as Piece
	match piece:
		Piece.Pawn:
			moves.push_back(MoveOld.new(piece_pos, piece_pos + Vector2i(0, 1)))
	return moves
