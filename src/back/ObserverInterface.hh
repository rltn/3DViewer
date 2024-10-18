#ifndef CPP4_3DVIEWER_CODE_BACK_OBSERVERINTERFACE_H_
#define CPP4_3DVIEWER_CODE_BACK_OBSERVERINTERFACE_H_

#include <string>

#include "Model.hh"

namespace ViewerApp {

/// @brief Observer interface.
class IObserver {
 public:
  virtual ~IObserver() = default;
  /// @brief Observer updation method.
  virtual void observerUpdate(Model::Property property) = 0;
};

/// @brief Notifier interface.
class INotifier {
 public:
  virtual ~INotifier() = default;
  /// @brief Attaches an observer to self.
  virtual void attach(IObserver* observer) = 0;
  /// @brief Detaches an observer to self.
  virtual void detach(IObserver* observer) = 0;
  /// @brief Notifies every attached observer by `observerUpdate` call.
  virtual void notify(Model::Property property) = 0;
};

}  // namespace ViewerApp

#endif