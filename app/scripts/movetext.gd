extends RichTextLabel

@onready var _board: Board = %Board.board

func _on_board_changed() -> void:
	var new_text: String = ""
	for i in _board.moves.size():
		print(i)
		if (i % 2 == 0):
			new_text += "[/color]\n%d. [color=ffffff99]" % ((i/2)+1)
		new_text += "%s " % _board.moves[i]
	text = "[center]%s" % new_text.trim_prefix("[/color]\n")
