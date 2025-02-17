//
// Created by sophosmoros on 2/17/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <godot_cpp/classes/area2d.hpp>

namespace godot {

class Player final : public Area2D{
	GDCLASS(Player, Area2D);

protected:
	static void _bind_methods();

public:
	static void _register_methods();

	Player();
	~Player() override;

	void _process(double delta) override;
};

}
#endif //PLAYER_HPP
