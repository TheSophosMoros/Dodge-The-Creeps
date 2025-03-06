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

/**
 * @brief The Main Node for the DodgeTheCreeps Game
 */
class Main final : public Node {
  GDCLASS(Main, Node);
  Ref<PackedScene> mobScene;
  std::mt19937 rng;
  std::uniform_real_distribution<float> dist;
  int score;

	struct Child
  {
	  mutable Timer* startTimer;
		mutable Timer* scoreTimer;
		mutable Timer* mobTimer;
		mutable Player* player;
		mutable Marker2D* startPosition;
		mutable HUD* hud;
	  mutable AudioStreamPlayer* musicAudioStreamPlayer;
	  mutable AudioStreamPlayer* deathAudioStreamPlayer;
	  mutable PathFollow2D* mobSpawnPath;
	} child;

	enum NodeEnums
	{
		ALL,
	  START_TIMER,      ///< @link Child::startTimer @endlink
		SCORE_TIMER,      ///< @link Child::scoreTimer @endlink
		MOB_TIMER,        ///< @link Child::mobTimer @endlink
		PLAYER,           ///< @link Child::player @endlink
		START_POSITION,   ///< @link Child::startPosition @endlink
		HEADS_UP_DISPLAY, ///< @link Child::hud @endlink
	  MUSIC_AUDIO,      ///< @link Child::musicAudioStreamPlayer @endlink
	  DEATH_AUDIO,      ///< @link Child::deathAudioStreamPlayer @endlink
	  MOB_SPAWN_PATH    ///< @link Child::mobSpawnPath @endlink
	};

	template<typename... nodeChoices>
	void initChildren(nodeChoices... nodeEnums);
	void checkAndLoadNodes(NodeEnums nodeChoice);

protected:
  static void _bind_methods();

public:
  void setMobScene(const Ref<PackedScene> &mobScene);
  [[nodiscard]] Ref<PackedScene> getMobScene();

  Main();
  ~Main() override;

  void onStartTimerTimeout();
  void onScoreTimerTimeout();
  void onMobTimerTimeout();

  void gameOver();
  void newGame();

  void _ready() override;
};
}

#endif //MAIN_HPP
