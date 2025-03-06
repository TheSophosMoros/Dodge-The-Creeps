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

/**
 * @brief The HUD Node for the Godot DodgeTheCreeps Game
 */
class HUD final : public CanvasLayer {
  GDCLASS(HUD, CanvasLayer)

  struct Child
  {
    mutable Button* startButton;
    mutable Label* messageLabel;
    mutable Timer* messageTimer;
    mutable Label* scoreLabel;
  } child;

  enum NodeEnums
  {
    ALL,
    START_BUTTON,  ///< @link Child::startButton @endlink
    MESSAGE_LABEL, ///< @link Child::messageLabel @endlink
    MESSAGE_TIMER, ///< @link Child::messageTimer @endlink
    SCORE_LABEL    ///< @link Child::scoreLabel @endlink
  };

  template<typename... nodeChoices>
  void initChildren(nodeChoices... nodeEnums);
  void checkAndLoadNodes(NodeEnums nodeChoice);

protected:
  static void _bind_methods();

public:
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
