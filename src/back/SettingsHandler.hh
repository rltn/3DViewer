#ifndef CPP4_3DVIEWER_CODE_BACK_SETTINGSHANDLER_HH_
#define CPP4_3DVIEWER_CODE_BACK_SETTINGSHANDLER_HH_

#include <QObject>
#include <QTimer>
#include <algorithm>
#include <list>
#include <memory>
#include <mutex>

#include "Model.hh"
#include "ObserverInterface.hh"

namespace ViewerApp {

/// @brief SettingsHandler instance class.
class SettingsHandler : public QObject, public IObserver {
  Q_OBJECT

 public:
  /// @brief Non cloneable.
  SettingsHandler(SettingsHandler& other) = delete;

  /// @brief Non assignable.
  void operator=(const SettingsHandler&) = delete;

  /// @brief Thread-safe instance accessor.
  static SettingsHandler& getInstance();

  void observerUpdate(Model::Property property) override;

 private:
  SettingsHandler();
  ~SettingsHandler();

  void saveSettings();

  static std::mutex mutex_;
  QTimer save_timer_;
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_CODE_BACK_SETTINGSHANDLER_HH_