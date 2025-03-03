//
// Created by sophosmoros on 2/17/25.
//

#ifndef HUD_HPP
#define HUD_HPP

#include <godot_cpp/classes/canvas_layer.hpp>

namespace godot
{
class HUD final : public CanvasLayer
{
  GDCLASS( HUD, CanvasLayer )

public:
  static void _bind_methods();

  HUD();

  ~HUD() override;

  void onStartButtonPressed();

  void onMessageTimerTimeout() const;

  void showMessage(const String &message) const;

  void showGameOverMessage();

  void onGameOverTimerTimeout();

  void onGameOverFinishRestart() const;

  void updateScore(int score) const;
};
}


#endif //HUD_HPP
