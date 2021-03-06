/*
 * player_agent.cpp
 *  Created on: Sep 1, 2017
 *      Author: Nathaniel Rodriguez
 *
 * The PlayerAgent is an abstract class use to define the basic functionality
 * of an agent for the ALE atari games. It is used by the Controller to play
 * the games. Derived classes MUST define an Act() function that returns
 * an ALE action.
 *
 * This is a simplified and slightly modified version of the google Xitari
 * PlayerAgent class.
 */

#include "player_agent.hpp"
#include "../common/screen_logger.hpp"
#include <ale_interface.hpp>

namespace alectrnn {

PlayerAgent::PlayerAgent(ALEInterface* ale)
    : ale_(ale), frame_skip_(ale_->getInt("frame_skip")),
    frame_number_(0), episode_frame_number_(0), episode_number_(0),
    has_terminated_(false) {
  available_actions_ = ale->getMinimalActionSet();
  max_num_frames_ = ale_->getInt("max_num_frames");
  max_num_episodes_ = ale_->getInt("max_num_episodes");
  max_num_frames_per_episode_ = ale_->getInt("max_num_frames_per_episode");
}

PlayerAgent::~PlayerAgent() {
}

Action PlayerAgent::AgentStep() {
  if (max_num_frames_ > 0 && frame_number_ >= max_num_frames_) {
    EndGame();
  }

  if (max_num_frames_per_episode_ > 0 &&
      episode_frame_number_ >= max_num_frames_per_episode_) {
    return RESET; //Returns RESET, gets picked up by controller which implements
    // the appropriate type of reset (doesn't actually use console reset)
  }

  Action agent_action(Act());
  frame_number_ += frame_skip_;
  episode_frame_number_ += frame_skip_;

  return agent_action;
}

Action PlayerAgent::EpisodeStart() {
  episode_frame_number_ = 0;
  Action agent_action(Act());
  frame_number_ += frame_skip_;
  episode_frame_number_ += frame_skip_;
  return agent_action;
}

void PlayerAgent::EpisodeEnd() {
  episode_number_++;
  if (max_num_episodes_ > 0 && episode_number_ >= max_num_episodes_) {
    EndGame();
  }
}

bool PlayerAgent::HasTerminated() {
  return has_terminated_;
}

void PlayerAgent::Reset() {
  frame_number_ = 0;
  episode_number_ = 0;
  has_terminated_ = false;
  episode_frame_number_ = 0;
}

void PlayerAgent::EndGame() {
  has_terminated_ = true;
}

void PlayerAgent::RewardFeedback(const int reward)
{
}

}
