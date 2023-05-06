extends Control
class_name BoardNode

## The node which handles user interaction with the [Board].

## The texture for the pieces. Sliced via code.
@export var pieces_texture: Texture2D
## Precomputed value for the size of a texture slice of [member pieces_texture].
@onready var texture_slice_size: Vector2 = Vector2(
	pieces_texture.get_width() / 6.0,
	pieces_texture.get_height() / 2.0
)

## The [StyleBox] of the dark squares. Changed via the [Theme].
@onready var dark_square: StyleBox = get_theme_stylebox("dark_square", "BoardNode")
## The [StyleBox] of the light squares. Changed via the [Theme].
@onready var light_square: StyleBox = get_theme_stylebox("light_square", "BoardNode")

## The [Board] the user interacts with. 
var board: Board = Board.from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR")
## An [Array] of [int] positions. Those positions will be highlighted to the user.
## (Used to telegraph to the user to where its piece can move).
var highlight_positions: Array

## If true, the index position of every square will be rendered.
var draw_piece_positions: bool = false

# The renderer wont render the piece at this position
var _skip_rendering: int = -1

## A signal that fires when the [member board] changes.
signal board_changed

func _ready() -> void:
	print(board.fen())

## Returns the position of the tile that specified local position is on.
func local_pos_to_tile_pos(pos: Vector2) -> Vector2i:
	return (pos / (size / board.size_vector)) as Vector2i

## Returns the offset between [param pos] and the top left corner of the tile nearest to it.
func local_pos_mod_tile_pos(pos: Vector2) -> Vector2:
	return pos.posmodv(size / board.size_vector)

func _exit_tree() -> void:
	# Don't waste memory
	board.free()

func _get_drag_data(at_position: Vector2):
	var tile_pos: Vector2i = local_pos_to_tile_pos(at_position)
	if board.getv(tile_pos) == Board.Piece.Empty:
		return null
	var drag_data = {
		"tile" = tile_pos,
		"piece" = board.getv(tile_pos),
		"moves" = board.generate_moves_for_piece(tile_pos.x + (tile_pos.y * board.width))
	}
	# We create a parent for the preview so we can offset the preview by any value we want
	# This is because Godot automatically sets the position of the drag preview so we can't offset it
	var square_size: Vector2 = size / board.size_vector
	var preview_parent: Control = Control.new()
	var preview: TextureRect = TextureRect.new()
	var preview_texture: AtlasTexture = AtlasTexture.new()
	preview_texture.atlas = pieces_texture 
	preview_texture.region = get_piece_draw_region(drag_data["piece"])
	preview.texture = preview_texture
	preview.position = -local_pos_mod_tile_pos(at_position)
	preview.scale = square_size / 133
	preview_parent.add_child(preview)
	set_drag_preview(preview_parent)
	
	_skip_rendering = tile_pos.x + (tile_pos.y * board.width)
	
	highlight_positions = drag_data["moves"].map(func(move: BoardMove): return move.target_square)
	# Get rid of the original piece so it doesn't render
#	board.setv(tile_pos, Board.Piece.Empty)
	# Redraw the board
	queue_redraw()
	
	return drag_data

func _can_drop_data(_at_position: Vector2, _drag_data) -> bool:
	return true

func _drop_data(at_position: Vector2, drag_data) -> void:
	if drag_data != null:
		var a = local_pos_to_tile_pos(at_position)
		var b = drag_data["tile"]
		var c = BoardMove.make(
			b.x + (b.y * board.width),
			a.x + (a.y * board.width)
		)
		board.do_move(c)
		print(board.fen())
		board_changed.emit()
		print(board.moves)
		
		_skip_rendering = -1
		
		# Redraw the board
		queue_redraw()
		highlight_positions = []

## Returns a [Rect2] used to splice the [member pieces_texture].
func get_piece_draw_region(piece: Board.Piece) -> Rect2:
	if (piece & 0b111 == Board.Piece.Empty): return Rect2(0, 0, 0, 0)
	@warning_ignore("integer_division")
	return Rect2(
		((piece - 1) % 8) * texture_slice_size.x, ((piece - 1) / 8) * texture_slice_size.y,
		texture_slice_size.x, texture_slice_size.y
	)

func _unhandled_key_input(event: InputEvent) -> void:
	if (event.is_action_pressed("ui_undo") && board.moves.size() > 0):
		board.undo_move(board.moves.back())
		board_changed.emit()
		queue_redraw()

func _draw() -> void:
	var square_size: Vector2 = size / board.size_vector
	for index in board.area:
		var x: int = index % board.size.x
		@warning_ignore("integer_division") var y: int = index / board.size.x
		var rect: Rect2 = Rect2(x * square_size.x, y * square_size.y, square_size.x, square_size.y)
		var piece: Board.Piece = board.pieces[index]
		(light_square if (x+y) % 2 == 0 else dark_square).draw(get_canvas_item(), rect)
		if board.pieces.size() > index && _skip_rendering != index && piece != Board.Piece.Empty:
			draw_texture_rect_region(
				pieces_texture,
				rect,
				get_piece_draw_region(piece),
				Color.WHITE
			)
		if draw_piece_positions:
			draw_string(
				ThemeDB.fallback_font, Vector2(rect.position.x, rect.get_center().y + 16),
				String.num(index), HORIZONTAL_ALIGNMENT_CENTER, rect.size.x, 32, Color.WHITE
			)
			draw_string_outline(
				ThemeDB.fallback_font, Vector2(rect.position.x, rect.get_center().y + 16),
				String.num(index), HORIZONTAL_ALIGNMENT_CENTER, rect.size.x, 32, 2, Color.BLACK
			)
	for pos in highlight_positions:
		var x: int = pos % board.size.x
		@warning_ignore("integer_division") var y: int = pos / board.size.x
		draw_circle(
			Vector2((x + 0.5) * square_size.x, (y + 0.5) * square_size.y), 
			square_size[square_size.min_axis_index()] / 4.0,
			Color.from_hsv(0, 0, 0, 0.4)
		)


func _on_main_theme_changed() -> void:
	# We need to defer this call or it won't work for some reason.
	call_deferred("reload_theme")
## Reloads the theme properties of the [BoardNode].
func reload_theme() -> void:
	dark_square = get_theme_stylebox("dark_square", "BoardNode")
	light_square = get_theme_stylebox("light_square", "BoardNode")
	queue_redraw()
