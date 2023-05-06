extends MenuBar

@onready var _board: BoardNode = %Board

#@onready var _file: PopupMenu = $File
@onready var _debug: PopupMenu = $Debug

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

func _on_file_id_pressed(id: int) -> void:
	match id:
		0: $"File/QuitConfirm".popup_centered()
		1: $"File/LoadFEN".popup_centered()
		2: $"File/Settings".popup_centered()
func _on_debug_id_pressed(id: int) -> void:
	match id:
		0:
			var checked: bool = _debug.is_item_checked(0)
			_debug.set_item_checked(0, !checked)
			_board.draw_piece_positions = !checked
			_board.queue_redraw()
		2:
			_debug.set_item_checked(2, true)
			_debug.set_item_checked(3, false)
			_debug.set_item_checked(4, false)
		3: 
			_debug.set_item_checked(2, false)
			_debug.set_item_checked(3, true)
			_debug.set_item_checked(4, false)
		4: 
			_debug.set_item_checked(2, false)
			_debug.set_item_checked(3, false)
			_debug.set_item_checked(4, true)
func _on_quit_confirm_confirmed() -> void:
	get_tree().quit(0)

func _on_load_fen_confirmed() -> void:
	_board.board = Board.from_fen($"File/LoadFEN/VBoxContainer/FENInput".text)
	_board.queue_redraw() # Redraw the board with the new changes
	$"File/LoadFEN".hide()



