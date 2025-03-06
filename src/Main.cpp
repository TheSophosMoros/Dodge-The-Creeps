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

/**
 * @breif Method to allow seamless call of multiple node pointers to be loaded quickly
 * @tparam nodeChoices Type allowing (type1, type2, ect)
 * @param nodeEnums
 */
template<typename... nodeChoices>
void Main::initChildren(nodeChoices... nodeEnums) { (checkAndLoadNodes(nodeEnums), ... ); }

/**
 * @breif Method to check if a node pointer is loaded and if not attempt to populate the node pointer
 * @param nodeChoice The @link Main#NodeEnums @endlink related to the node pointer to be validated
 */
void Main::checkAndLoadNodes(const NodeEnums nodeChoice)
{
  switch (nodeChoice)
  {
    case ALL:
    {
      initChildren(START_TIMER,
                   SCORE_TIMER,
                   MOB_TIMER,
                   PLAYER,
                   START_POSITION,
                   HEADS_UP_DISPLAY,
                   MUSIC_AUDIO,
                   DEATH_AUDIO,
                   MOB_SPAWN_PATH);
      break;
    }
    case START_TIMER:
    {
      if (child.startTimer == nullptr)
      {
        const auto startTimerTmp = get_node<Timer>("StartTimer");
        if (startTimerTmp != nullptr) child.startTimer = startTimerTmp;
      }
      break;
    }
    case SCORE_TIMER:
    {
      if (child.scoreTimer == nullptr)
      {
        const auto scoreTimerTmp = get_node<Timer>("ScoreTimer");
        if (scoreTimerTmp != nullptr) child.scoreTimer = scoreTimerTmp;
      }
      break;
    }
    case MOB_TIMER:
    {
      if (child.mobTimer == nullptr)
      {
        const auto mobTimerTmp = get_node<Timer>("MobTimer");
        if (mobTimerTmp != nullptr) child.mobTimer = mobTimerTmp;
      }
      break;
    }
    case PLAYER:
    {
      if (child.player == nullptr)
      {
        const auto playerTmp = get_node<Player>("Player");
        if (playerTmp != nullptr) child.player = playerTmp;
      }
      break;
    }
    case START_POSITION:
    {
      if (child.startPosition == nullptr)
      {
        const auto startPositionTmp = get_node<Marker2D>("StartPosition");
        if (startPositionTmp != nullptr) child.startPosition = startPositionTmp;
      }
      break;
    }
    case HEADS_UP_DISPLAY:
    {
      if (child.hud == nullptr)
      {
        const auto hudTmp = get_node<HUD>("HUD");
        if (hudTmp != nullptr) child.hud = hudTmp;
      }
      break;
    }
    case MUSIC_AUDIO:
    {
      if (child.musicAudioStreamPlayer == nullptr)
      {
        const auto musicAudioTmp = get_node<AudioStreamPlayer>("Music");
        if (musicAudioTmp != nullptr) child.musicAudioStreamPlayer = musicAudioTmp;
      }
      break;
    }
    case DEATH_AUDIO:
    {
      if (child.deathAudioStreamPlayer == nullptr)
      {
        const auto deathAudioTmp = get_node<AudioStreamPlayer>("DeathSound");
        if (deathAudioTmp != nullptr) child.deathAudioStreamPlayer = deathAudioTmp;
      }
      break;
    }
    case MOB_SPAWN_PATH:
    {
      if (child.mobSpawnPath == nullptr)
      {
        const auto mobSpawnPathTmp = get_node<PathFollow2D>("MobPath/MobSpawnLocation");
        if (mobSpawnPathTmp != nullptr) child.mobSpawnPath = mobSpawnPathTmp;
      }
      break;
    }
    default: break;
  }
}

/**
 * @breif Godot built in method to link cpp methods to godot engine. Any further methods that will attempt to link to your methods should use the p_name stated in your binds
 */
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

/**
 * @breif Main Constructor
 */
Main::Main() : rng(std::random_device()()),
               score(0),
               child{nullptr,
                     nullptr,
                     nullptr,
                     nullptr,
                     nullptr}
{}

/**
 * @breif Main Deconstructor
 */
Main::~Main() = default;

/**
 * @breif When the Start Timer Timeout Signal emitted this method should be called
 */
void Main::onStartTimerTimeout()
{
  initChildren(SCORE_TIMER, MOB_TIMER);
  child.scoreTimer->start();
  child.mobTimer->start();
}

/**
 * @breif When the Score Timer Timeout Signal emitted this method should be called
 */
void Main::onScoreTimerTimeout()
{
  ++score;
  child.hud->updateScore(score);
}

/**
 * @breif When the Mob Timer Timeout Signal emitted this method should be called
 */
void Main::onMobTimerTimeout()
{
  //Create a new Instance of the Mob scene
  const auto mobNode = mobScene->instantiate();
  const auto mob = cast_to<Mob>(mobNode);

  //Choose a random location on Path2D
  initChildren(MOB_SPAWN_PATH);
  child.mobSpawnPath->set_progress_ratio(dist(rng));

  //Set the mob's direction perpendicular to the path direction.
  float direction = child.mobSpawnPath->get_rotation() + M_PI / 2; // NOLINT(*-narrowing-conversions)

  //Set the mob's position to a random location
  mob->set_position(child.mobSpawnPath->get_position());

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

void Main::setMobScene(const Ref<PackedScene> &mobScene) {this->mobScene = mobScene;}
Ref<PackedScene> Main::getMobScene() {return mobScene;}

/**
 * @breif When the hit Signal emitted from player, this method should be called
 */
void Main::gameOver()
{
  initChildren(MUSIC_AUDIO,
               DEATH_AUDIO,
               SCORE_TIMER,
               MOB_TIMER,
               HEADS_UP_DISPLAY);
  child.musicAudioStreamPlayer->stop();
  child.deathAudioStreamPlayer->play();
  child.scoreTimer->stop();
  child.mobTimer->stop();
  child.hud->showGameOverMessage();

  get_tree()->call_group("mobs", "queue_free");
}

/**
 * @breif When the Start_Game Signal emitted from HUD, this method should be called
 */
void Main::newGame()
{
  score = 0;

  initChildren(MUSIC_AUDIO,
               PLAYER,
               START_TIMER,
               START_POSITION,
               HEADS_UP_DISPLAY);
  child.musicAudioStreamPlayer->play();
  child.player->start(child.startPosition->get_position());
  child.startTimer->start();
  child.hud->updateScore(score);
  child.hud->showMessage("Get Ready");
}

/**
 * @breif Godot built in method that is called when a node is loaded
 */
void Main::_ready() { initChildren(ALL); }
