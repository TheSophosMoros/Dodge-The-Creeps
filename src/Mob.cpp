//
// Created by sophosmoros on 2/17/25.
//

#include "Mob.hpp"

#include <godot_cpp/classes/sprite_frames.hpp>

using namespace godot;

/**
 * @breif Method to allow seamless call of multiple node pointers to be loaded quickly
 * @tparam nodeChoices Type allowing (type1, type2, ect)
 * @param nodeEnums
 */
template<typename... nodeChoices>
void Mob::initChildren(nodeChoices... nodeEnums) { (checkAndLoadNodes(nodeEnums), ... ); }

/**
 * @breif Method to check if a node pointer is loaded and if not attempt to populate the node pointer
 * @param nodeChoice The @link Mob#NodeEnums @endlink related to the node pointer to be validated
 */
void Mob::checkAndLoadNodes(const NodeEnums nodeChoice)
{
  switch (nodeChoice)
  {
    case ALL:
    {
      initChildren(ANIMATED_SPRITE);
      break;
    }
    case ANIMATED_SPRITE:
    {
      if (child.animatedSprite2D == nullptr)
      {
        const auto animatedSpriteTmp = get_node<AnimatedSprite2D>("AnimatedSprite2D");
        if (animatedSpriteTmp != nullptr) child.animatedSprite2D = animatedSpriteTmp;
      }
      break;
    }
    default: break;
  }
}

/**
 * @brief Method to randomly choose what Mob will be displayed upon creation
 */
void Mob::setRandomAnimation()
{
  initChildren(ANIMATED_SPRITE);
  const auto animatedTypesArray = child.animatedSprite2D->get_sprite_frames()->get_animation_names();

  dist.param(std::uniform_real_distribution<>::param_type(0, animatedTypesArray.size()-1));

  child.animatedSprite2D->set_animation(animatedTypesArray[dist(rng)]);
  child.animatedSprite2D->play();
}

/**
 * @breif Godot built in method to link cpp methods to godot engine. Any further methods that will attempt to link to your methods should use the p_name stated in your binds
 */
void Mob::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("set_minimum_speed", "p_speed"), &Mob::setMinimumSpeed);
  ClassDB::bind_method(D_METHOD("set_maximum_speed", "p_speed"), &Mob::setMaximumSpeed);
  ClassDB::bind_method(D_METHOD("get_minimum_speed"), &Mob::getMinimumSpeed);
  ClassDB::bind_method(D_METHOD("get_maximum_speed"), &Mob::getMaximumSpeed);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "minimum_speed"),
               "set_minimum_speed",
               "get_minimum_speed");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "maximum_speed"),
               "set_maximum_speed",
               "get_maximum_speed");

  ClassDB::bind_method(D_METHOD("_on_VisibilityNotifier2D_screen_exited"), &Mob::onVisibilityNotifier2DScreenExited);
}

void Mob::setMinimumSpeed(const int minimum_speed) { minimumSpeed = minimum_speed; }
int Mob::getMinimumSpeed() const { return minimumSpeed; }
void Mob::setMaximumSpeed(const int maximum_speed) { maximumSpeed = maximum_speed; }
int Mob::getMaximumSpeed() const { return maximumSpeed; }

/**
 * @brief this method overloads the "_on_VisibilityNotifier2D_screen_exited" aka VisibilityNotifier2D._screen_exited signal such that if the mob leaves the screen then this method gets called
 */
void Mob::onVisibilityNotifier2DScreenExited() { queue_free(); }

/**
 * @brief Mob Constructor
 */
Mob::Mob() : minimumSpeed(DEFAULT_MINIMUM_SPEED),
             maximumSpeed(DEFAULT_MAXIMUM_SPEED),
             rng(std::random_device()()),
             child{nullptr}
{}

/**
 * @brief Mob Deconstructor
 */
Mob::~Mob() = default;

/**
 * @brief Godot built in method that is called when a node is loaded
 */
void Mob::_ready() { setRandomAnimation(); }



