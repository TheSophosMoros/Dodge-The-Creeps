//
// Created by sophosmoros on 2/17/25.
//

#include "Player.hpp"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

using namespace godot;

void Player::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Player::setSpeed);
	ClassDB::bind_method(D_METHOD("get_speed"), &Player::getSpeed);
	ClassDB::bind_method(D_METHOD("_body_collision", "p_body"), &Player::bodyCollision);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "speed"), "set_speed", "get_speed");
	ADD_SIGNAL(MethodInfo("hit"));
}

Player::Player()
{
	speed = 400;
}

Player::~Player() = default;

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

	const auto sprite2dPtr = this->get_node<AnimatedSprite2D>("AnimatedSprite2D");
	velocity = velocity.normalized() * static_cast<float>(speed);
	velocity.length() > 0 ?	sprite2dPtr->play() : sprite2dPtr->stop();

	auto position = get_position();
	position += velocity * delta;
	position.x = Math::clamp(position.x, 0.f, screenSize.x);
	position.y = Math::clamp(position.y, 0.f, screenSize.y);
	this->set_position(position);

	if (velocity.x != 0.f)
	{
		sprite2dPtr->set_animation("walk");
		sprite2dPtr->set_flip_v(false);
		sprite2dPtr->set_flip_h(velocity.x < 0.f);
	}
	else if (velocity.y != 0.f)
	{
		sprite2dPtr->set_animation("up");
		sprite2dPtr->set_flip_v(velocity.y > 0.f);
	}
}

void Player::_ready() {
	screenSize = get_viewport_rect().get_size();
	hide();
}

void Player::bodyCollision(Node *body) {
	hide();
	emit_signal("hit");
	get_node<CollisionShape2D>("CollisionShape2D")->set_deferred("disabled", true);
}

void Player::start(const Vector2 position)
{
	print_line("Setting player position");
	set_position(position);
	show();
	get_node<CollisionShape2D>("CollisionShape2D")->set_disabled(false);
}