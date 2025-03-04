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

template<typename... nodeChoices>
void Main::initChildren(nodeChoices... nodeEnums)
{
  (checkAndLoadNodes(nodeEnums), ... );
}

void Main::checkAndLoadNodes(const NodeEnums nodeChoice)
{
  switch (nodeChoice)
  {
    case ALL: initChildren(SCORE_TIMER,
                           MOB_TIMER,
                           PLAYER,
                           START_POSITION,
                           HEADS_UP_DISPLAY);
      break;
    case SCORE_TIMER:
      if (child.scoreTimer == nullptr)
      {
        const auto scoreTimerTmp = get_node<Timer>("ScoreTimer");
        if (scoreTimerTmp != nullptr) child.scoreTimer = scoreTimerTmp;
      }
      break;
    case MOB_TIMER:
      if (child.mobTimer == nullptr)
      {
        const auto mobTimerTmp = get_node<Timer>("MobTimer");
        if (mobTimerTmp != nullptr) child.mobTimer = mobTimerTmp;
      }
      break;
    case PLAYER:
      if (child.player == nullptr)
      {
        const auto playerTmp = get_node<Player>("Player");
        if (playerTmp != nullptr) child.player = playerTmp;
      }
      break;
    case START_POSITION:
      if (child.startPosition == nullptr)
      {
        const auto startPositionTmp = get_node<Marker2D>("StartPosition");
        if (startPositionTmp != nullptr) child.startPosition = startPositionTmp;
      }
      break;
    case HEADS_UP_DISPLAY: if (child.hud == nullptr)
      {
        const auto hudTmp = get_node<HUD>("HUD");
        if (hudTmp != nullptr) child.hud = hudTmp;
      }
      break;
    default: break;
  }
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

Main::Main() :
  rng(std::random_device()()),
  child{nullptr, nullptr, nullptr, nullptr, nullptr},
  score(0) {}

Main::~Main() = default;

void Main::onStartTimerTimeout() const
{
  child.scoreTimer->start();
  child.mobTimer->start();
}

void Main::onScoreTimerTimeout()
{
  ++score;
  child.hud->updateScore(score);
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
  dist.param(std::uniform_real_distribution<float>::param_type(mob->getMinimumSpeed(), mob->getMaximumSpeed()));
  // NOLINT(*-narrowing-conversions)
  mob->set_linear_velocity(Vector2(dist(rng), 0).rotated(direction));

  //Spawn the mob by adding it to the Main scene
  add_child(mobNode);
}

void Main::gameOver() const
{
  get_node<AudioStreamPlayer>("Music")->stop();
  get_node<AudioStreamPlayer>("DeathSound")->play();

  child.scoreTimer->stop();
  child.mobTimer->stop();
  child.hud->showGameOverMessage();

  get_tree()->call_group("mobs", "queue_free");
}

void Main::newGame()
{
  score = 0;
  get_node<AudioStreamPlayer>("Music")->play();
  initChildren(PLAYER, START_POSITION, HEADS_UP_DISPLAY);
  child.player->start(child.startPosition->get_position());
  get_node<Timer>("StartTimer")->start();
  child.hud->updateScore(score);
  child.hud->showMessage("Get Ready");
}

void Main::_ready()
{
  initChildren(ALL);
}