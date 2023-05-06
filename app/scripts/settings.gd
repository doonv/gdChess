extends Window

const themes_folder: String = "user://Themes/"

@onready var setting_theme: Control = $ScrollContainer/VBoxContainer/Theme
@onready var theme_input: OptionButton = setting_theme.get_node("SettingContainer/Input")
# Called when the node enters the scene tree for the first time.
func _ready():
	theme_input.clear()
	DirAccess.make_dir_absolute(themes_folder)
	DirAccess.copy_absolute("res://theme.tres", themes_folder.path_join("Default.tres"))
	var index: int = 0
	for filename in DirAccess.get_files_at(themes_folder):
		theme_input.add_item(filename, index)
		theme_input.set_item_tooltip(
			index,
			ProjectSettings.globalize_path(themes_folder.path_join(filename))
		)
		index += 1
	var description: RichTextLabel = setting_theme.get_node("Description")
	description.text = description.text.format([ProjectSettings.globalize_path(themes_folder)])




func _on_description_meta_clicked(meta) -> void:
	OS.shell_open(str(meta))


func _on_close_requested() -> void:
	hide()

func _on_theme_input_item_selected(index: int) -> void:
	var filename: String = theme_input.get_item_text(index)
	var filepath: String = themes_folder.path_join(filename)
	if ResourceLoader.exists(filepath, "Theme"):
		var main: Control = owner as Control
		main.theme = ResourceLoader.load(filepath, "Theme", ResourceLoader.CACHE_MODE_REUSE)
		main.queue_redraw()
	else:
		print("Couldn't find file!")
