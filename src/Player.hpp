//
// Created by sophosmoros on 2/17/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <godot_cpp/classes/area2d.hpp>

namespace godot {

class Player final : public Area2D{
	GDCLASS(Player, Area2D);

	int speed{};
	Vector2 screenSize;

protected:
	static void _bind_methods();

public:
	static void _register_methods();

	void setSpeed(int new_speed);
	[[nodiscard]] int getSpeed() const;

	Player();
	~Player() override;

	void _process(float delta);
	void _ready() override;

	void bodyEntered(Node *body);
	void start(Vector2 position);
};

}
#endif //PLAYER_HPP
