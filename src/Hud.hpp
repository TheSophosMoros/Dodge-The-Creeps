//
// Created by sophosmoros on 2/17/25.
//

#ifndef HUD_HPP
#define HUD_HPP
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/button.hpp>

namespace godot {

class HUD final : public CanvasLayer {
  GDCLASS(HUD, CanvasLayer)
  Button* startButton;
  Label* messageLabel;
  Timer* messageTimer;
  Label* scoreLabel;

protected:
  static void _bind_methods();

public:
  enum ChildNodeEnum
  {
    ALL,
    START_BUTTON,
    MESSAGE_LABEL,
    MESSAGE_TIMER,
    SCORE_LABEL
  };

  template<typename... ChildNodes>
  void loadNodes(ChildNodes... children);
  void processNode(ChildNodeEnum child);

  HUD();
  ~HUD() override;

  void onStartButtonPressed();
  void onMessageTimerTimeout();

  void showMessage(const String &message);
  void showGameOverMessage();
  void onGameOverTimerTimeout();
  void onGameOverFinishRestart();

  void updateScore(int score);
};
}


#endif //HUD_HPP
