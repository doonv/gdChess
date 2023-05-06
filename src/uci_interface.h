#ifndef UCI_INTERFACE_H
#define UCI_INTERFACE_H


#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "lib_communication.hpp"

namespace godot {

class UCIInterface : public Object, public Communication::Process {
    GDCLASS(UCIInterface, Object)

private:

protected:
    static void _bind_methods();

public:
	PackedStringArray read(String wait_for_word, int64_t timeout_threshold);
	/// @brief Send a messsage to the engine.
	void send(String message);
	static UCIInterface *open(String engine_path);

    UCIInterface();
    ~UCIInterface();

};


}

#endif
