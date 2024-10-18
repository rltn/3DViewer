#ifndef CPP4_3DVIEWER_SRC_CODE_BACK_AFFINEAPPLIER_H_
#define CPP4_3DVIEWER_SRC_CODE_BACK_AFFINEAPPLIER_H_

#include <mutex>

#include "Model.hh"
#include "ObserverInterface.hh"

namespace ViewerApp {

/// @brief Observer class which applies affine transformations on all vertices.
class AffineApplier : public IObserver {
 public:
  /// @brief Non cloneable.
  AffineApplier(AffineApplier& other) = delete;

  /// @brief Non assignable.
  void operator=(const AffineApplier&) = delete;

  /// @brief Thread-safe instance accessor.
  static AffineApplier& getInstance();

  void observerUpdate(Model::Property property) override;

 private:
  AffineApplier();
  ~AffineApplier();

  static std::mutex mutex_;

  void applyScale();
  void applyRotate();
  void applyMove();
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_SRC_CODE_BACK_AFFINEAPPLIER_H_