//
// Created by sophosmoros on 2/17/25.
//

#include "Main.hpp"

#include <cmath>

#include "Mob.hpp"

#include <godot_cpp/classes/path2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

template<typename... ChildNodes>
void Main::loadNodes(ChildNodes... children)
{
  (processNode(children), ...);
}

void Main::processNode(const ChildNodeEnum child)
{
  switch (child)
  {
    case SCORE_TIMER:
      if (scoreTimer == nullptr)
            scoreTimer = get_node<Timer>("ScoreTimer");
      break;
    case MOB_TIMER:
      if (mobTimer == nullptr)
        mobTimer = get_node<Timer>("MobTimer");
      break;
    case START_TIMER:
      if (startTimer == nullptr)
        startTimer = get_node<Timer>("StartTimer");
      break;
    case PLAYER:
      if (player == nullptr)
        player = get_node<Player>("Player");
      break;
    case START_POSITION:
      if (startPosition == nullptr)
        startPosition = get_node<Marker2D>("StartPosition");
      break;
    case HEADS_UP_DISPLAY:
      if (hud == nullptr)
        hud = get_node<HUD>("HUD");
      break;
    case MUSIC_AUDIO_PLAYER:
      if (musicAudioPlayer == nullptr)
        musicAudioPlayer = get_node<AudioStreamPlayer>("Music");
      break;
    case DEATH_AUDIO_PLAYER:
      if (deathAudioPlayer == nullptr)
        deathAudioPlayer = get_node<AudioStreamPlayer>("DeathSound");
      break;
    case MOB_SPAWN_LOCAL:
      if (mobSpawnLocal == nullptr)
        mobSpawnLocal = get_node<PathFollow2D>("MobPath/MobSpawnLocation");
      break;
    case ALL:
      loadNodes(SCORE_TIMER,
                MOB_TIMER,
                START_TIMER,
                PLAYER,
                START_POSITION,
                HEADS_UP_DISPLAY,
                MUSIC_AUDIO_PLAYER,
                DEATH_AUDIO_PLAYER,
                MOB_SPAWN_LOCAL);
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

Main::Main()
{
  score = 0;
  rng.seed(std::random_device()());
  scoreTimer = nullptr;
  mobTimer = nullptr;
  startTimer = nullptr;
  player = nullptr;
  startPosition = nullptr;
  hud = nullptr;
  musicAudioPlayer = nullptr;
  deathAudioPlayer = nullptr;
  mobSpawnLocal = nullptr;
}

Main::~Main()
= default;

void Main::onStartTimerTimeout()
{
  loadNodes(SCORE_TIMER, MOB_TIMER);

  scoreTimer->start();
  mobTimer->start();
}

void Main::onScoreTimerTimeout()
{
  loadNodes(HEADS_UP_DISPLAY);

  ++score;
  hud->updateScore(score);
}

void Main::onMobTimerTimeout()
{
  loadNodes(MOB_SPAWN_LOCAL);

  //Create a new Instance of the Mob scene
  const auto nodePtrFromMobScene = mobScene->instantiate();
  const auto mobPtr = cast_to<Mob>(nodePtrFromMobScene);

  //Choose a random location on Path2D
  mobSpawnLocal->set_progress_ratio(dist(rng));

  //Set the mob's direction perpendicular to the path direction.
  float direction = mobSpawnLocal->get_rotation() + M_PI / 2; // NOLINT(*-narrowing-conversions)

  //Set the mob's position to a random location
  mobPtr->set_position(mobSpawnLocal->get_position());

  //Add some randomness to the direction.
  dist.param(std::uniform_real_distribution<float>::param_type(-M_PI / 2, M_PI / 2));
  direction += dist(rng);
  mobPtr->set_rotation(direction);

  //Choose a random velocity for the mob
  dist.param(std::uniform_real_distribution<float>::param_type(mobPtr->getMinimumSpeed(), mobPtr->getMaximumSpeed())); // NOLINT(*-narrowing-conversions)
  mobPtr->set_linear_velocity(Vector2(dist(rng), 0).rotated(direction));

  //Spawn the mob by adding it to the Main scene
  add_child(nodePtrFromMobScene);
}

void Main::gameOver()
{
  loadNodes(MUSIC_AUDIO_PLAYER,
            DEATH_AUDIO_PLAYER,
            SCORE_TIMER,
            MOB_TIMER,
            HEADS_UP_DISPLAY);

  musicAudioPlayer->stop();
  deathAudioPlayer->play();
  scoreTimer->stop();
  mobTimer->stop();
  hud->showGameOverMessage();

  get_tree()->call_group("mobs", "queue_free");
}

void Main::newGame()
{
  loadNodes(MUSIC_AUDIO_PLAYER,
            PLAYER,
            START_POSITION,
            START_TIMER,
            HEADS_UP_DISPLAY);
  print_line("new_game");
  score = 0;
  musicAudioPlayer->play();
  player->start(startPosition->get_position());
  startTimer->start();
  hud->updateScore(score);
  hud->showMessage("Get Ready");
}

void Main::_ready()
{
  loadNodes(ALL);
}
