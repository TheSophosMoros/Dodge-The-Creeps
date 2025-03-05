//
// Created by sophosmoros on 2/17/25.
//

#include "Player.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

/**
 * @breif Method to allow seamless call of multiple node pointers to be loaded quickly
 * @tparam nodeChoices Type allowing (type1, type2, ect)
 * @param nodeEnums
 */
template<typename... nodeChoices>
void Player::initChildren(nodeChoices... nodeEnums)
{
  (checkAndLoadNodes(nodeEnums), ... );
}

/**
 * @breif Method to check if a node pointer is loaded and if not attempt to populate the node pointer
 * @param nodeChoice The @link Player#NodeEnums @endlink related to the node pointer to be validated
 */
void Player::checkAndLoadNodes(const NodeEnums nodeChoice)
{
  switch (nodeChoice)
  {
    case ALL:
    {
      initChildren(COLLISION_SHAPE, ANIMATED_SPRITE);
      break;
    }
    case COLLISION_SHAPE:
    {
      if (child.collisionShape2D == nullptr)
      {
        const auto collisionShapeTmp = get_node<CollisionShape2D>("CollisionShape2D");
        if (collisionShapeTmp != nullptr) child.collisionShape2D = collisionShapeTmp;
      }
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
 * @breif Godot built in method to link cpp methods to godot engine. Any further methods that will attempt to link to your methods should use the p_name stated in your binds
 */
void Player::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Player::setSpeed);
	ClassDB::bind_method(D_METHOD("get_speed"), &Player::getSpeed);
	ClassDB::bind_method(D_METHOD("_body_collision", "p_body"), &Player::bodyCollision);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "speed"), "set_speed", "get_speed");
	ADD_SIGNAL(MethodInfo("hit"));
}

void Player::setSpeed(const int new_speed) { speed = new_speed; }
int Player::getSpeed() const { return speed; }

/**
 * @brief Player Constructor
 */
Player::Player() : speed(400),
                   child{nullptr, nullptr}
{}

/**
 * @brief Player Deconstructor
 */
Player::~Player() = default;

/**
 * @brief Godot built in method that is called every frame and process the players action
 * @param delta represents the time elapsed since the last frame was processed, measured in seconds
 */
void Player::_process(const float delta)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	//Work Around for editing that stops the error of 'InputMap action "" doesn't exist'
	const auto inputPtr = Input::get_singleton();
	auto velocity = Vector2();

	if (inputPtr->is_action_pressed("move_left")) velocity.x -= 1;
	if (inputPtr->is_action_pressed("move_right")) velocity.x += 1;
	if (inputPtr->is_action_pressed("move_up")) velocity.y -= 1;
	if (inputPtr->is_action_pressed("move_down")) velocity.y += 1;

  initChildren(ANIMATED_SPRITE);
	velocity = velocity.normalized() * static_cast<float>(speed);
	velocity.length() > 0 ?	child.animatedSprite2D->play() : child.animatedSprite2D->stop();

	auto positionVector = get_position();
	positionVector += velocity * delta;
	positionVector.x = Math::clamp(positionVector.x, 0.f, screenSize.x);
	positionVector.y = Math::clamp(positionVector.y, 0.f, screenSize.y);
	this->set_position(positionVector);

	if (velocity.x != 0.f)
	{
		child.animatedSprite2D->set_animation("walk");
		child.animatedSprite2D->set_flip_v(false);
		child.animatedSprite2D->set_flip_h(velocity.x < 0.f);
	}
	else if (velocity.y != 0.f)
	{
		child.animatedSprite2D->set_animation("up");
		child.animatedSprite2D->set_flip_v(velocity.y > 0.f);
	}
}

/**
 * @brief Godot built in method that is called when a node is loaded
 */
void Player::_ready() {
	screenSize = get_viewport_rect().get_size();
	hide();
}

/**
 * @brief The Method determining the result of another node colliding with the player
 * @param body the @link Node @endlink of what collided with the player
 */
void Player::bodyCollision(Node *body) {
	hide();
	emit_signal("hit");
  initChildren(COLLISION_SHAPE);
	child.collisionShape2D->set_deferred("disabled", true);
}

/**
 * @brief The method to be called at the beginning of the game
 * @param position the location where the player will start
 */
void Player::start(const Vector2 position)
{
	set_position(position);
	show();
  initChildren(COLLISION_SHAPE);
  child.collisionShape2D->set_disabled(false);
}