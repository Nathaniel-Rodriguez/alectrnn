/*
 * internal_controller.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: nathaniel
 */

#include <ale_interface.hpp>
#include "../agents/player_agent.h"
#include "controller.h"

namespace alectrnn {

Controller::Controller(ALEInterface* ale, PlayerAgent* agent) : ale_(ale),
    agent_(agent), episode_score_(0), episode_number_(0), cumulative_score_(0) {
  max_num_frames_ = ale_->getInt("max_num_frames");
  max_num_episodes_ = ale_->getInt("max_num_episodes");
  ale_->environment->reset();
}

Controller::~Controller() {
}

void Controller::Run() {
  Action agent_action;
  bool first_step = true;
  ale_->environment->reset();

  while (!IsDone()) {
    // Start a new episode: Check for terminal state
    if (ale_->environment->isTerminal()) {
      EpisodeEnd();
      first_step = true;
    }
    else {
      if (first_step) {
        // Start a new episode; obtain actions
        EpisodeStart(agent_action);
        first_step = false;
      }
      else {
        // Poll agents for actions
        EpisodeStep(agent_action);
      }

      // Apply said actions
      ApplyActions(agent_action);
      episode_score_ += ale_->romSettings->getReward();
      cumulative_score_ += ale_->romSettings->getReward();
    }
  }
}

int Controller::getCumulativeScore() const {
  return cumulative_score_;
}

void Controller::EpisodeStart(Action& action) {
  // Poll the agent for first action
  action = agent_->EpisodeStart();
  episode_score_ = 0;
  episode_number_++;
}

void Controller::EpisodeStep(Action& action) {
  action = agent_->AgentStep();
}

void Controller::EpisodeEnd() {
  agent_->EpisodeEnd();
  ale_->environment->reset();
}

void Controller::ApplyActions(Action& action) {
  // Perform different operations based on the first player's action
  switch (action) {
    case LOAD_STATE: // Load system state
      // Note - this does not reset the game screen;
      // so that the subsequent screen
      // is incorrect (in fact, two screens, due to colour averaging)
      ale_->environment->load();
      break;
    case SAVE_STATE: // Save system state
      ale_->environment->save();
      break;
    case SYSTEM_RESET:
      ale_->environment->reset();
      break;
    default:
      // Pass action to emulator!
      ale_->environment->minimalAct(action, PLAYER_B_NOOP);
      break;
  }
}

bool Controller::IsDone() const {
  return (agent_->HasTerminated() ||
      (max_num_episodes_ > 0 && episode_number_ > max_num_episodes_) ||
      (max_num_frames_ > 0 &&
          ale_->environment->getFrameNumber() >= max_num_frames_));
}

}
