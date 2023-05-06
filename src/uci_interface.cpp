#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "uci_interface.h"

using namespace godot;

UCIInterface::UCIInterface() {
}

UCIInterface::~UCIInterface() {

}
PackedStringArray UCIInterface::read(String wait_for_word, int64_t timeout_threshold) {
   	// TODO: This is not an efficent way to do this but whatever.
	std::vector<std::string> vec = readProcess(
		std::string_view(wait_for_word.utf8().get_data()),
		timeout_threshold
	);
	PackedStringArray arr = PackedStringArray();
	for (const std::string &line : vec) {
        arr.append(String(line.c_str()));
    }
	return arr;
}

void UCIInterface::send(String message) {
	writeProcess(std::string(message.utf8().get_data()) + "\n");
}

UCIInterface *UCIInterface::open(String engine_path) {
	UCIInterface *interface = memnew(UCIInterface());
	interface->initProcess(engine_path.utf8().get_data());
	return interface;
}

void UCIInterface::_bind_methods() {
    ClassDB::bind_method(D_METHOD("read", "wait_for_word", "timeout_threshold"), &UCIInterface::read);
    ClassDB::bind_method(D_METHOD("send", "message"), &UCIInterface::send);
    ClassDB::bind_static_method("UCIInterface", D_METHOD("open", "engine_path"), &UCIInterface::open);
}
