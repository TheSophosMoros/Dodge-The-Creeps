//
// Created by sophosmoros on 2/17/25.
//

#ifndef MAIN_HPP
#define MAIN_HPP
#include <random>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {
class Main final : public Node {
  GDCLASS(Main, Node);
  Ref<PackedScene> mobScene;
  std::mt19937 rng;
  std::uniform_real_distribution<> dist;

  int score;

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

  void gameOver();
  void newGame();

  void _ready() override;
};
}

#endif //MAIN_HPP
