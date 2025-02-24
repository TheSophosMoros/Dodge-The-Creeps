//
// Created by sophosmoros on 2/17/25.
//

#include "Main.hpp"

#include <cmath>

#include "Hud.hpp"
#include "Mob.hpp"

#include <godot_cpp/classes/path2d.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/marker2d.hpp>

#include "Player.hpp"

using namespace godot;

void Main::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("game_over"), &Main::gameOver);
  ClassDB::bind_method(D_METHOD("new_game"), &Main::newGame);

  ClassDB::bind_method(D_METHOD("_on_score_timer_timeout"), &Main::onScoreTimerTimeout);
  ClassDB::bind_method(D_METHOD("_on_mob_timer_timeout"), &Main::onMobTimerTimeout);
  ClassDB::bind_method(D_METHOD("_on_start_timer_timeout"), &Main::onStartTimerTimeout);

  ClassDB::bind_method(D_METHOD("set_mob_scene", "p_mob_scene"), &Main::setMobScene);
  ClassDB::bind_method(D_METHOD("get_mob_scene"), &Main::getMobScene);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mob_scene"), "set_mob_scene", "get_mob_scene");
}

Main::Main()
{
  score = 0;
  rng.seed(std::random_device()());
}

Main::~Main()
{
}

void Main::onStartTimerTimeout() const
{
  get_node<Timer>("ScoreTimer")->start();
  get_node<Timer>("MobTimer")->start();
}

void Main::onScoreTimerTimeout()
{
  ++score;
  get_node<HUD>("HUD")->updateScore(score);
}

void Main::onMobTimerTimeout()
{
  //Create a new Instance of the Mob scene
  const auto mobNode = mobScene->instantiate();
  const auto mob = cast_to<Mob>(mobNode);

  //Choose a random location on Path2D
  const auto mobSpawnLocation = get_node<PathFollow2D>("MobPath/MobSpawnLocation");
  mobSpawnLocation->set_progress_ratio(dist(rng));

  //Set the mob's direction perpendicular to the path direction.
  double direction = mobSpawnLocation->get_rotation() + M_PI / 2;

  //Set the mob's position to a random location
  mob->set_position(mobSpawnLocation->get_position());

  //Add some randomness to the direction.
  dist.param(std::uniform_real_distribution<>::param_type(-M_PI / 2, M_PI / 2));
  direction += dist(rng);
  mob->set_rotation(direction);

  //Choose a random velocity for the mob
  dist.param(std::uniform_real_distribution<>::param_type(mob->getMinimumSpeed(), mob->getMaximumSpeed()));
  mob->set_linear_velocity(Vector2(dist(rng), 0).rotated(direction));

  //Spawn the mob by adding it to the Main scene
  add_child(mobNode);
}

void Main::gameOver()
{
  get_node<Timer>("ScoreTimer")->stop();
  get_node<Timer>("MobTimer")->stop();
  get_node<HUD>("HUD")->showGameOverMessage();

  get_tree()->call_group("mobs", "queue_free");
}

void Main::newGame()
{
  score = 0;
  get_node<Player>("Player")
  ->start(get_node<Marker2D>("StartPosition")
          ->get_position());
}

void Main::_ready()
{
  newGame();
}
