//
// Created by sophosmoros on 2/17/25.
//

#include "Hud.hpp"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>


using namespace godot;

template<typename ... ChildNodes>
void HUD::loadNodes(ChildNodes... children)
{
  (processNode(children), ...);
}

inline void HUD::processNode(const ChildNodeEnum child)
{
  switch (child)
  {
    case ALL:
      loadNodes(START_BUTTON,
                MESSAGE_LABEL,
                MESSAGE_TIMER,
                SCORE_LABEL);
      break;
    case START_BUTTON:
      if (startButton == nullptr)
        startButton = get_node<Button>("StartButton");
      break;
    case MESSAGE_LABEL:
      if (messageLabel == nullptr)
        messageLabel = get_node<Label>("MessageLabel");
      break;
    case MESSAGE_TIMER:
      if (messageTimer == nullptr)
        messageTimer = get_node<Timer>("MessageTimer");
      break;
    case SCORE_LABEL:
      if (scoreLabel == nullptr)
        scoreLabel = get_node<Label>("ScoreLabel");
      break;
    default: break;
  }
}

void HUD::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_on_start_button_pressed"), &HUD::onStartButtonPressed);
  ClassDB::bind_method(D_METHOD("_on_message_timer_timeout"), &HUD::onMessageTimerTimeout);
  ClassDB::bind_method(D_METHOD("show_message", "p_message"), &HUD::showMessage);
  ClassDB::bind_method(D_METHOD("show_game_over_message"), &HUD::showGameOverMessage);
  ClassDB::bind_method(D_METHOD("_on_game_over_timer_timeout"), &HUD::onGameOverTimerTimeout);
  ClassDB::bind_method(D_METHOD("_on_game_over_finish_restart"), &HUD::onGameOverFinishRestart);

  ADD_SIGNAL(MethodInfo("start_game"));
}

HUD::HUD()
{
  startButton = nullptr;
  messageLabel = nullptr;
  messageTimer = nullptr;
  scoreLabel = nullptr;
}

HUD::~HUD()
= default;

void HUD::onStartButtonPressed()
{
  loadNodes(ALL);
  startButton->hide();
  emit_signal("start_game");
}

void HUD::onMessageTimerTimeout()
{
  loadNodes(MESSAGE_LABEL);
  messageLabel->hide();
}

void HUD::showMessage(const String &message)
{
  loadNodes(MESSAGE_LABEL, MESSAGE_TIMER);
  messageLabel->set_text(message);
  messageLabel->show();
  messageTimer->start();
}

void HUD::showGameOverMessage()
{
  showMessage("Game Over");
  loadNodes(MESSAGE_TIMER);
  messageTimer->connect("timeout", Callable(this, "_on_game_over_timer_timeout"), CONNECT_ONE_SHOT);
  messageTimer->start();
}

void HUD::onGameOverTimerTimeout()
{
  loadNodes(MESSAGE_LABEL);
  messageLabel->set_text("Dodge the Creeps!");
  messageLabel->show();

  get_tree()
  ->create_timer(1)
  ->connect("timeout", Callable(this, "_on_game_over_finish_restart"), CONNECT_ONE_SHOT);
}

void HUD::onGameOverFinishRestart()
{
  loadNodes(START_BUTTON);
  startButton->show();
}

void HUD::updateScore(const int score)
{
  loadNodes(SCORE_LABEL);
  scoreLabel->set_text(String::num_int64(score));
}
