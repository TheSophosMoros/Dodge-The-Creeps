//
// Created by sophosmoros on 2/17/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

namespace godot {

/**
 * @brief The Player Node for the Godot DodgeTheCreeps Game
 */
class Player final : public Area2D{
	GDCLASS( Player, Area2D );

	int speed;
	Vector2 screenSize;

  struct Child
  {
    mutable AnimatedSprite2D* animatedSprite2D;
    mutable CollisionShape2D* collisionShape2D;
  } child;

  enum NodeEnums
  {
    ALL,
    ANIMATED_SPRITE, ///< @link Child::animatedSprite2D @endlink
    COLLISION_SHAPE  ///< @link Child::collisionShape2D @endlink
  };

  template<typename... nodeChoices>
  void initChildren( nodeChoices... nodeEnums );
  void checkAndLoadNodes( NodeEnums nodeChoice );

protected:
	static void _bind_methods();

public:
	void setSpeed( int new_speed );
  [[nodiscard]] int getSpeed() const;

  Player();
	~Player() override;

	void _process( float delta );
	void _ready() override;

	void bodyCollision( Node *body );
	void start( Vector2 position );
};

}
#endif //PLAYER_HPP
