//
// Created by sophosmoros on 2/17/25.
//

#include "Hud.hpp"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>


using namespace godot;

/**
 * @breif Method to allow seamless call of multiple node pointers to be loaded quickly
 * @tparam nodeChoices Type allowing (type1, type2, ect)
 * @param nodeEnums
 */
template<typename... nodeChoices>
void HUD::initChildren(nodeChoices... nodeEnums) { (checkAndLoadNodes(nodeEnums), ... ); }

/**
 * @breif Method to check if a node pointer is loaded and if not attempt to populate the node pointer
 * @param nodeChoice The @link HUD#NodeEnums @endlink related to the node pointer to be validated
 */
void HUD::checkAndLoadNodes(const NodeEnums nodeChoice)
{
  switch (nodeChoice)
  {
    case ALL:
    {
      initChildren(START_BUTTON,
                   MESSAGE_LABEL,
                   MESSAGE_TIMER,
                   SCORE_LABEL);
      break;
    }
    case START_BUTTON:
    {
      if (child.startButton == nullptr)
      {
        const auto startButtonTmp = get_node<Button>("StartButton");
        if (startButtonTmp != nullptr) child.startButton = startButtonTmp;
      }
      break;
    }
    case MESSAGE_LABEL:
    {
      if (child.messageLabel == nullptr)
      {
        const auto messageLabelTmp = get_node<Label>("Message");
        if (messageLabelTmp != nullptr) child.messageLabel = messageLabelTmp;
      }
      break;
    }
    case MESSAGE_TIMER:
    {
      if (child.messageTimer == nullptr)
      {
        const auto messageTimerTmp = get_node<Timer>("MessageTimer");
        if (messageTimerTmp != nullptr) child.messageTimer = messageTimerTmp;
      }
      break;
    }
    case SCORE_LABEL:
    {
      if (child.scoreLabel == nullptr)
      {
        const auto scoreLabelTmp = get_node<Label>("ScoreLabel");
        if (scoreLabelTmp != nullptr) child.scoreLabel = scoreLabelTmp;
      }
      break;
    }
    default: break;
  }
}

/**
 * @breif Godot built in method to link cpp methods to godot engine. Any further methods that will attempt to link to your methods should use the p_name stated in your binds
 */
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

/**
 * @brief HUD Constructor
 */
HUD::HUD() : child{nullptr, nullptr, nullptr}
{}

/**
 * @brief HUD Deconstructor
 */
HUD::~HUD() = default;

/**
 * @brief When the Start Button Pressed Signal emitted this method should be called
 */
void HUD::onStartButtonPressed()
{
  initChildren(START_BUTTON);
  child.startButton->hide();
  emit_signal("start_game");
}

/**
 * @brief When the Message Timer Timeout emitted this method should be called
 */
void HUD::onMessageTimerTimeout()
{
  initChildren(MESSAGE_LABEL);
  child.messageLabel->hide();
}

/**
 * @brief The method called whenever a temporary message needs to be displayed. Length of the message's appearance is determined by the child.messageTimer
 * @param message the text to be set to child.messageLabel
 */
void HUD::showMessage(const String &message)
{
  initChildren(MESSAGE_LABEL, MESSAGE_TIMER);
  child.messageLabel->set_text(message);
  child.messageLabel->show();
  child.messageTimer->start();
}

/**
 * @brief The method called by Main during the gameOver Method. This is the First step of the three step game over hud process
 */
void HUD::showGameOverMessage()
{
  showMessage("Game Over");
  initChildren(MESSAGE_TIMER);
  child.messageTimer->connect("timeout",
                              Callable(this, "_on_game_over_timer_timeout"),
                              CONNECT_ONE_SHOT);
  child.messageTimer->start();
}

/**
 * @brief The Second step of the hud game over process
 */
void HUD::onGameOverTimerTimeout()
{
  initChildren(MESSAGE_LABEL);
  child.messageLabel->set_text("Dodge the Creeps!");
  child.messageLabel->show();
  get_tree()
  ->create_timer(1)
  ->connect("timeout",
            Callable(this, "_on_game_over_finish_restart"),
            CONNECT_ONE_SHOT);
}

/**
 * @brief The Third and final step of the hud game over process
 */
void HUD::onGameOverFinishRestart()
{
  initChildren(START_BUTTON);
  child.startButton->show();
}

/**
 * @brief The method called by Main during the onScoreTimerTimeout method. This method updates the text of the child.scoreLabel
 * @param score the integer from main that is the players current score
 */
void HUD::updateScore(const int score)
{
  initChildren(SCORE_LABEL);
  child.scoreLabel->set_text(String::num_int64(score));
}
