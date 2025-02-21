//
// Created by sophosmoros on 2/17/25.
//

#ifndef MOB_HPP
#define MOB_HPP
#include <godot_cpp/classes/rigid_body2d.hpp>

namespace godot {
class Mob final : RigidBody2D{
	GDCLASS(Mob, RigidBody2D);

	static constexpr int defaultMinimumSpeed = 150;
	static constexpr int defaultMaximumSpeed = 200;
	int minimumSpeed = defaultMinimumSpeed;
	int maximumSpeed = defaultMaximumSpeed;

	public:
	static void _bind_methods();

	void setMinimumSpeed(const int minimum_speed) { minimumSpeed = minimum_speed; }
	[[nodiscard]] int getMinimumSpeed() const { return minimumSpeed; }
	void setMaximumSpeed(const int maximum_speed) { maximumSpeed = maximum_speed; }
	[[nodiscard]] int getMaximumSpeed() const { return maximumSpeed; }


	Mob();
	~Mob() override;

	void _ready() override;
};

}
#endif //MOB_HPP
