#include "AffineApplier.hh"

#include <cmath>

namespace ViewerApp {

std::mutex AffineApplier::mutex_;

AffineApplier::AffineApplier() {
  Model::getInstance().getObjectFile().resetVertices();
  applyScale();
  applyRotate();
  applyMove();
}

AffineApplier::~AffineApplier() {}

AffineApplier &AffineApplier::getInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  static AffineApplier instance;
  return instance;
}

void AffineApplier::observerUpdate(Model::Property property) {
  switch (property) {
    case Model::LINE_TYPE:
    case Model::POINT_TYPE:
    case Model::LINE_SIZE:
    case Model::POINT_SIZE:
    case Model::PROJECTION_TYPE:
      break;
    default:
      Model::getInstance().getObjectFile().resetVertices();
      applyScale();
      applyRotate();
      applyMove();
      break;
  }
}

void AffineApplier::applyScale() {
  Model &model = Model::getInstance();
  std::vector<double> &vertices = model.getObjectFile().getVertices();
  if (vertices.empty()) return;
  double scale = model.getScale();
  for (unsigned int i = 0; i < vertices.size(); i++) {
    vertices[i] *= scale;
  }
}

void AffineApplier::applyRotate() {
  Model &model = Model::getInstance();
  std::vector<double> &vertices = model.getObjectFile().getVertices();
  if (vertices.empty()) return;
  double rx = model.getRotateX() * M_PI / 180;
  double ry = model.getRotateY() * M_PI / 180;
  double rz = model.getRotateZ() * M_PI / 180;
  double xv, yv, zv;
  for (unsigned int i = 0; i < vertices.size(); i += 3) {
    yv = vertices[i + 1], zv = vertices[i + 2];
    vertices[i + 1] = std::cos(rx) * yv - std::sin(rx) * zv;
    vertices[i + 2] = std::sin(rx) * yv + std::cos(rx) * zv;
    xv = vertices[i], zv = vertices[i + 2];
    vertices[i] = std::cos(ry) * xv + std::sin(ry) * zv;
    vertices[i + 2] = -std::sin(ry) * xv + std::cos(ry) * zv;
    xv = vertices[i], yv = vertices[i + 1];
    vertices[i] = std::cos(rz) * xv - std::sin(rz) * yv;
    vertices[i + 1] = std::sin(rz) * xv + std::cos(rz) * yv;
  }
}

void AffineApplier::applyMove() {
  Model &model = Model::getInstance();
  std::vector<double> &vertices = model.getObjectFile().getVertices();
  if (vertices.empty()) return;
  double x_move = model.getMoveX();
  double y_move = model.getMoveY();
  double z_move = model.getMoveZ();
  for (unsigned int i = 0; i < vertices.size(); i += 3) {
    vertices[i] += x_move;
    vertices[i + 1] += y_move;
    vertices[i + 2] += z_move;
  }
}

}  // namespace ViewerApp