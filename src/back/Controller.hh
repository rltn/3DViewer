#ifndef CPP4_3DVIEWER_CODE_BACK_CONTROLLERSINGLETON_H_
#define CPP4_3DVIEWER_CODE_BACK_CONTROLLERSINGLETON_H_

#include <algorithm>
#include <list>
#include <memory>
#include <mutex>

#include "Model.hh"
#include "ObserverInterface.hh"

namespace ViewerApp {

/// @brief Controller instance class to notify observers and store model data.
class Controller : public INotifier {
 public:
  /// @brief Non cloneable.
  Controller(Controller& other) = delete;

  /// @brief Non assignable.
  void operator=(const Controller&) = delete;

  /// @brief Thread-safe instance accessor.
  static Controller& getInstance();

  void attach(IObserver* observer) override { observers_.push_back(observer); }

  void detach(IObserver* observer) override {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end());
  }

  void notify(Model::Property property) override {
    for (auto& observer : observers_) {
      observer->observerUpdate(property);
    }
  }

  template <typename T>
  void setContext(const T& value, Model::Property property) {
    Model::getInstance().setProperty(value, property);
    notify(property);
  }

 private:
  Controller();
  ~Controller();

  static std::mutex mutex_;

  std::list<IObserver*> observers_;
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_CODE_BACK_CONTROLLERSINGLETON_H_