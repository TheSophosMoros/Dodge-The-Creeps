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

#include "Hud.hpp"
#include "Player.hpp"

namespace godot {
class Main final : public Node {
  GDCLASS(Main, Node);
  Ref<PackedScene> mobScene;
  std::mt19937 rng;
  std::uniform_real_distribution<float> dist;
  Timer* scoreTimer{};
  Timer* mobTimer{};
  Player* player{};
  Marker2D* startPosition{};
  HUD* hud{};

	struct {
		mutable Timer* scoreTimer;
		mutable Timer* mobTimer;
		mutable Player* player;
		mutable Marker2D* startPosition;
		mutable HUD* hud;
	} child;

	enum NodeEnums
	{
		ALL,
		SCORE_TIMER,
		MOB_TIMER,
		PLAYER,
		START_POSITION,
		HEADS_UP_DISPLAY
	};

  int score;

  void linkReferences();

	template<typename... nodeChoices>
	void initChildren(nodeChoices... nodeEnums);

	void checkAndLoadNodes(NodeEnums nodeChoice);

protected:
  static void _bind_methods();

public:
  Main();
  ~Main() override;

  void onStartTimerTimeout() const;
  void onScoreTimerTimeout();
  void onMobTimerTimeout();

  void setMobScene(const Ref<PackedScene> &mobScene) {this->mobScene = mobScene;};
  [[nodiscard]] Ref<PackedScene> getMobScene() {return mobScene;};

  void gameOver() const;
  void newGame();

  void _ready() override;
};
}

#endif //MAIN_HPP
