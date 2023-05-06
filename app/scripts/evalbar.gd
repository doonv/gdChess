extends ProgressBar

@onready var _board: Board = %Board.board
var engine: UCIInterface
var target_percent: float = 0
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	engine = UCIInterface.open("/usr/games/stockfish")
	engine.send("uci")
	engine.read("uciok", 0) # Wait until uciok is sent
	engine.send("ucinewgame")

func _exit_tree() -> void:
	engine.free()

func _on_board_changed() -> void:
	engine.send("position startpos moves %s" % " ".join(_board.moves.map(func(move):
		return _board.move_to_uci(move)
	)))
	engine.send("go depth 10")
	var result: PackedStringArray = engine.read("bestmove", 1000)
	target_percent = (result[result.size() - 2].split(" ")[9].to_int())
	if (_board.moves.size() % 2 == 0):
		target_percent = -target_percent
	print(target_percent)

func _process(delta: float) -> void:
	value = move_toward(value, target_percent, delta * 400)
