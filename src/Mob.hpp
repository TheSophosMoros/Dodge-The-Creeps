//
// Created by sophosmoros on 2/17/25.
//

#ifndef MOB_HPP
#define MOB_HPP

#include <random>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>

namespace godot {

/**
 * @brief The Mob Node for the Godot DodgeTheCreeps Game
 */
class Mob final : public RigidBody2D{
	GDCLASS(Mob, RigidBody2D);

	static constexpr int DEFAULT_MINIMUM_SPEED = 150;
	static constexpr int DEFAULT_MAXIMUM_SPEED = 200;
	int minimumSpeed;
	int maximumSpeed;
	std::mt19937 rng;
	std::uniform_real_distribution<> dist;

  struct Child
  {
    mutable AnimatedSprite2D* animatedSprite2D;
  } child;

  enum NodeEnums
  {
    ALL,
    ANIMATED_SPRITE ///< @link Child::animatedSprite2D @endlink
  };

  template<typename... nodeChoices>
  void initChildren(nodeChoices... nodeEnums);
  void checkAndLoadNodes(NodeEnums nodeChoice);

	void setRandomAnimation();

protected:
	static void _bind_methods();

public:
	void setMinimumSpeed(int minimum_speed);
  [[nodiscard]] int getMinimumSpeed() const;
  void setMaximumSpeed(int maximum_speed);
  [[nodiscard]] int getMaximumSpeed() const;

  void onVisibilityNotifier2DScreenExited();

	Mob();
	~Mob() override;

	void _ready() override;
};

}
#endif //MOB_HPP
