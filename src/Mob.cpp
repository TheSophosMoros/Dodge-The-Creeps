//
// Created by sophosmoros on 2/17/25.
//

#include "Mob.hpp"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>

using namespace godot;

void Mob::setRandomAnimation()
{
  const auto animatedSprite = this->get_node<AnimatedSprite2D>("AnimatedSprite2D");
  if (!animatedSprite) return;
  const auto animatedTypesArray = animatedSprite->get_sprite_frames()->get_animation_names();
  if (!animatedTypesArray) return;

  dist.param(std::uniform_real_distribution<>::param_type(0, animatedTypesArray.size()-1));

  animatedSprite->set_animation(animatedTypesArray[dist(rng)]);
  animatedSprite->play();
}


void Mob::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("set_minimum_speed", "p_speed"), &Mob::setMinimumSpeed);
  ClassDB::bind_method(D_METHOD("set_maximum_speed", "p_speed"), &Mob::setMaximumSpeed);
  ClassDB::bind_method(D_METHOD("get_minimum_speed"), &Mob::getMinimumSpeed);
  ClassDB::bind_method(D_METHOD("get_maximum_speed"), &Mob::getMaximumSpeed);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "minimum_speed"), "set_minimum_speed", "get_minimum_speed");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "maximum_speed"), "set_maximum_speed", "get_maximum_speed");

  ClassDB::bind_method(D_METHOD("_on_VisibilityNotifier2D_screen_exited"), &Mob::onVisibilityNotifier2DScreenExited);
}

void Mob::onVisibilityNotifier2DScreenExited()
{
  queue_free();
}

Mob::Mob()
{
  minimumSpeed = DEFAULT_MINIMUM_SPEED;
  maximumSpeed = DEFAULT_MAXIMUM_SPEED;
  rng.seed(std::random_device()());
}

Mob::~Mob() = default;

void Mob::_ready()
{
  setRandomAnimation();
}



