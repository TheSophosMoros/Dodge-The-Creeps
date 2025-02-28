//
// Created by sophosmoros on 2/17/25.
//

#ifndef MAIN_HPP
#define MAIN_HPP
#include <random>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/marker2d.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>

#include "Hud.hpp"
#include "Player.hpp"

namespace godot {
class Main final : public Node {
  GDCLASS(Main, Node);
  Ref<PackedScene> mobScene;
  std::mt19937 rng;
  std::uniform_real_distribution<float> dist;

  Timer* scoreTimer;
  Timer* mobTimer;
  Timer* startTimer;
  Player* player;
  Marker2D* startPosition;
  HUD* hud;
  AudioStreamPlayer* musicAudioPlayer;
  AudioStreamPlayer* deathAudioPlayer;
  PathFollow2D* mobSpawnLocal;


  int score;

protected:
  static void _bind_methods();

public:
  enum ChildNodeEnum
  {
    ALL,
    SCORE_TIMER,
    MOB_TIMER,
    START_TIMER,
    PLAYER,
    START_POSITION,
    HEADS_UP_DISPLAY,
    MUSIC_AUDIO_PLAYER,
    DEATH_AUDIO_PLAYER,
    MOB_SPAWN_LOCAL
  };

  template<typename... ChildNodes>
  void loadNodes(ChildNodes... children);
  void processNode(ChildNodeEnum child);

  Main();
  ~Main() override;

  void onStartTimerTimeout();
  void onScoreTimerTimeout();
  void onMobTimerTimeout();

  void setMobScene(const Ref<PackedScene> &mobScene) {this->mobScene = mobScene;};
  [[nodiscard]] Ref<PackedScene> getMobScene() {return mobScene;};

  void gameOver();
  void newGame();

  void _ready() override;
};
}

#endif //MAIN_HPP
