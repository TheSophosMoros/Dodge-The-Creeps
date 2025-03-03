//
// Created by sophosmoros on 2/17/25.
//

#include "Main.hpp"

#include <cmath>

#include "Mob.hpp"

#include <godot_cpp/classes/path2d.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>

using namespace godot;

void Main::linkReferences()
{
  print_line("Getting ScoreTimer");
  if (scoreTimer == nullptr)
  {
    scoreTimer = get_node<Timer>("ScoreTimer");
    scoreTimer != nullptr
      ? print_line("ScoreTimer Found")
      : print_line("ScoreTimer Not Found");
  } else print_line("ScoreTimer Already Linked");
  print_line("Getting MobTimer");
  if (mobTimer == nullptr)
  {
    mobTimer = get_node<Timer>("MobTimer");
    mobTimer != nullptr
      ? print_line("MobTimer Found")
      : print_line("MobTimer Not Found");
  } else print_line("MobTimer Already Linked");
  print_line("Getting Player");
  if (player == nullptr)
  {
    player = get_node<Player>("Player");
    player != nullptr
      ? print_line("Player Found")
      : print_line("Player Not Found");
  } else print_line("Player Already Linked");
  print_line("Getting StartPosition");
  if (startPosition == nullptr)
  {
    startPosition = get_node<Marker2D>("StartPosition");
    startPosition != nullptr
      ? print_line("StartPosition Found")
      : print_line("StartPosition Not Found");
  } else print_line("StartPosition Already Linked");
  print_line("Getting HUD");
  if (hud == nullptr)
  {
    hud = get_node<HUD>("HUD");
    hud != nullptr
      ? print_line("HUD Found")
      : print_line("HUD Not Found");
  } else print_line("HUD Already linked");
}

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
  scoreTimer->start();
  mobTimer->start();
}

void Main::onScoreTimerTimeout()
{
  ++score;
  hud->updateScore(score);
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
  float direction = mobSpawnLocation->get_rotation() + M_PI / 2; // NOLINT(*-narrowing-conversions)

  //Set the mob's position to a random location
  mob->set_position(mobSpawnLocation->get_position());

  //Add some randomness to the direction.
  dist.param(std::uniform_real_distribution<float>::param_type(-M_PI / 2, M_PI / 2));
  direction += dist(rng);
  mob->set_rotation(direction);

  //Choose a random velocity for the mob
  dist.param(std::uniform_real_distribution<float>::param_type(mob->getMinimumSpeed(), mob->getMaximumSpeed())); // NOLINT(*-narrowing-conversions)
  mob->set_linear_velocity(Vector2(dist(rng), 0).rotated(direction));

  //Spawn the mob by adding it to the Main scene
  add_child(mobNode);
}

void Main::gameOver() const
{
  get_node<AudioStreamPlayer>("Music")->stop();
  get_node<AudioStreamPlayer>("DeathSound")->play();

  scoreTimer->stop();
  mobTimer->stop();
  hud->showGameOverMessage();

  get_tree()->call_group("mobs", "queue_free");
}

void Main::newGame()
{
  score = 0;
  get_node<AudioStreamPlayer>("Music")->play();
  print_line("New Game Started");
  if (player != nullptr || startPosition != nullptr) linkReferences();
  print_line("Attempting to move player");
  player->start(startPosition->get_position());
  get_node<Timer>("StartTimer")->start();
  hud->updateScore(score);
  hud->showMessage("Get Ready");
}

void Main::_ready()
{
  linkReferences();
}
