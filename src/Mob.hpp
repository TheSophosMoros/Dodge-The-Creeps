//
// Created by sophosmoros on 2/17/25.
//

#ifndef MOB_HPP
#define MOB_HPP

#include <random>
#include <godot_cpp/classes/rigid_body2d.hpp>

namespace godot
{
class Mob final : public RigidBody2D
{
  GDCLASS( Mob, RigidBody2D );

  static constexpr int DEFAULT_MINIMUM_SPEED = 150;
  static constexpr int DEFAULT_MAXIMUM_SPEED = 200;
  int minimumSpeed;
  int maximumSpeed;
  std::mt19937 rng;
  std::uniform_real_distribution<> dist;

  void setRandomAnimation();

public:
  static void _bind_methods();

  void setMinimumSpeed(const int minimum_speed) { minimumSpeed = minimum_speed; }
  [[nodiscard]] int getMinimumSpeed() const { return minimumSpeed; }
  void setMaximumSpeed(const int maximum_speed) { maximumSpeed = maximum_speed; }
  [[nodiscard]] int getMaximumSpeed() const { return maximumSpeed; }

  void onVisibilityNotifier2DScreenExited();

  Mob();

  ~Mob() override;

  void _ready() override;
};
}
#endif //MOB_HPP
