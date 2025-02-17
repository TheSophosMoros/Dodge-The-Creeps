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
	ClassDB::bind_method(D_METHOD("_ready"), &Player::_ready);
	ClassDB::bind_method(D_METHOD("bodyEntered"), &Player::bodyEntered);
	ClassDB::bind_method(D_METHOD("setSpeed", "p_speed"), &Player::setSpeed);
	ClassDB::bind_method(D_METHOD("getSpeed"), &Player::getSpeed);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "speed"),"setSpeed", "getSpeed");
}

void Player::_register_methods()
{

}

void Player::setSpeed(const int new_speed) {
	speed = new_speed;
}

int Player::getSpeed() const {
	return speed;
}

Player::Player()
{
	speed = 400;
}

Player::~Player() = default;

void Player::_process(const float delta)
{
	if (Engine::get_singleton()->is_editor_hint()) return; //Work Around for editing that stops the error of InputMap action "" doesn't exist
	const Input *input = Input::get_singleton();
	auto velocity = Vector2();

	if (input->is_action_pressed("move_left")) velocity.x -= 1;
	if (input->is_action_pressed("move_right")) velocity.x += 1;
	if (input->is_action_pressed("move_up")) velocity.y -= 1;
	if (input->is_action_pressed("move_down")) velocity.y += 1;

	auto* sprite = this->get_node<AnimatedSprite2D>("AnimatedSprite2D");
	velocity = velocity.normalized() * static_cast<float>(speed);
	velocity.length() > 0 ?	sprite->play() : sprite->stop();

	auto position = get_position();
	position += velocity * delta;
	position.x = Math::clamp(position.x, 0.f, screenSize.x);
	position.y = Math::clamp(position.y, 0.f, screenSize.y);
	this->set_position(position);

	if (velocity.x != 0.f)
	{
		sprite->set_animation("walk");
		sprite->set_flip_v(false);
		sprite->set_flip_h(velocity.x < 0.f);
	}
	else if (velocity.y != 0.f)
	{
		sprite->set_animation("up");
		sprite->set_flip_v(velocity.y > 0.f);
	}
}

void Player::_ready() {
	screenSize = get_viewport_rect().get_size();
	hide();
}

void Player::bodyEntered(Node *body)
{
	hide();
	emit_signal("hit");
	get_node<CollisionShape2D>("CollisionShape2D")->set_disabled(true);
}

void Player::start(const Vector2 position)
{
	set_position(position);
	show();
	get_node<CollisionShape2D>("CollisionShape2D")->set_disabled(false);
}