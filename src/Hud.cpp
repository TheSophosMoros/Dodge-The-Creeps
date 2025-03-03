//
// Created by sophosmoros on 2/17/25.
//

#include "Hud.hpp"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>


using namespace godot;

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
}

HUD::~HUD()
{
}

void HUD::onStartButtonPressed()
{
  get_node<Button>("StartButton")->hide();
  emit_signal("start_game");
}

void HUD::onMessageTimerTimeout() const
{
  get_node<Label>("Message")->hide();
}

void HUD::showMessage(const String &message) const
{
  auto messageLabel = get_node<Label>("Message");
  messageLabel->set_text(message);
  messageLabel->show();
  get_node<Timer>("MessageTimer")->start();
}

void HUD::showGameOverMessage()
{
  showMessage("Game Over");
  const auto messageTimer = get_node<Timer>("MessageTimer");
  messageTimer->connect("timeout", Callable(this, "_on_game_over_timer_timeout"), CONNECT_ONE_SHOT);
  messageTimer->start();
}

void HUD::onGameOverTimerTimeout()
{
  print_line("Restarting game");
  auto messageLabel = get_node<Label>("Message");
  messageLabel->set_text("Dodge the Creeps!");
  messageLabel->show();

  get_tree()
  ->create_timer(1)
  ->connect("timeout", Callable(this, "_on_game_over_finish_restart"), CONNECT_ONE_SHOT);
}

void HUD::onGameOverFinishRestart() const
{
  get_node<Button>("StartButton")->show();
}

void HUD::updateScore(const int score) const
{
  get_node<Label>("ScoreLabel")->set_text(String::num_int64(score));
}
