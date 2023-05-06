extends Label

@onready var board_node: BoardNode = %Board
@onready var orig_text: String = text
func _on_board_changed() -> void:
	if (board_node.board.moves.size() == 0):
		text = orig_text
		return
	var new_opening: String = board_node.board.get_opening_name()
	print(new_opening)
	if (new_opening.is_empty() == false):
		text = new_opening
