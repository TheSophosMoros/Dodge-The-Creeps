//
// Created by sophosmoros on 2/17/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

namespace godot {

class Player final : public Area2D{
	GDCLASS(Player, Area2D);

	int speed{};
	Vector2 screenSize;
	AnimatedSprite2D* animatedSprite;
	CollisionShape2D* collisionShape;

protected:
	static void _bind_methods();

public:
	enum ChildNodeEnum
	{
		ALL,
		ANIMATED_SPRITE,
		COLLISION_SHAPE
	};

	template<typename... ChildNodes>
	void loadNodes(ChildNodes... children);
	void processNode(ChildNodeEnum child);

	void setSpeed(const int new_speed) { speed = new_speed; }
	[[nodiscard]] int getSpeed() const { return speed; }

	Player();
	~Player() override;

	void _process(float delta);
	void _ready() override;

	void bodyCollision(Node *body);
	void start(Vector2 position);
};

}
#endif //PLAYER_HPP
