#include "Controller.hh"

namespace ViewerApp {

std::mutex Controller::mutex_;

Controller::Controller() {}

Controller::~Controller() {}

Controller &Controller::getInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  static Controller instance;
  return instance;
}

}  // namespace ViewerApp