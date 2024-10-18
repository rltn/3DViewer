#include "Model.hh"

#include <stdexcept>

namespace ViewerApp {

std::mutex Model::mutex_;

Model::Model() {
  move_x_ = move_y_ = move_z_ = 0;
  rotate_x_ = rotate_y_ = rotate_z_ = 0;
  scale_ = 1;
  line_size_ = point_size_ = 1;
  object_ = ObjectFile();
  line_type_ = LINE_TYPE_SOLID;
  point_type_ = POINT_TYPE_NONE;
  projection_type_ = PROJECTION_TYPE_CENTRAL;
}

Model::~Model() {}

Model& Model::getInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  static Model instance;
  return instance;
}

void Model::setProperty(float value, Property property) {
  switch (property) {
    case MOVE_X:
      move_x_ = value;
      break;
    case MOVE_Y:
      move_y_ = value;
      break;
    case MOVE_Z:
      move_z_ = value;
      break;
    case ROTATE_X:
      rotate_x_ = value;
      break;
    case ROTATE_Y:
      rotate_y_ = value;
      break;
    case ROTATE_Z:
      rotate_z_ = value;
      break;
    case SCALE:
      scale_ = value;
      break;
    default:
      throw std::runtime_error("Incorrect property type error.");
      break;
  }
}

void Model::setProperty(int value, Property property) {
  switch (property) {
    case LINE_SIZE:
      line_size_ = value;
      break;
    case POINT_SIZE:
      point_size_ = value;
      break;
    default:
      throw std::runtime_error("Incorrect property type error.");
      break;
  }
}

void Model::setProperty(const std::string& value, Property property) {
  (void)value;
  switch (property) {
    case OBJECT:
      object_ = ObjectFile(value);
      break;
    default:
      throw std::runtime_error("Incorrect property type error.");
      break;
  }
}

void Model::setProperty(Color value, Property property) {
  switch (property) {
    case COLOR_POINT:
      color_point_ = value;
      break;
    case COLOR_LINE:
      color_line_ = value;
      break;
    case COLOR_FILL:
      color_fill_ = value;
      break;
    default:
      throw std::runtime_error("Incorrect property type error.");
      break;
  }
}

void Model::setProperty(LineType value, Property property) {
  switch (property) {
    case LINE_TYPE:
      line_type_ = value;
      break;
    default:
      throw std::runtime_error("Incorrect property type error.");
      break;
  }
}

void Model::setProperty(PointType value, Property property) {
  switch (property) {
    case POINT_TYPE:
      point_type_ = value;
      break;
    default:
      throw std::runtime_error("Incorrect property type error.");
      break;
  }
}

void Model::setProperty(ProjectionType value, Property property) {
  switch (property) {
    case PROJECTION_TYPE:
      projection_type_ = value;
      break;
    default:
      throw std::runtime_error("Incorrect property type error.");
      break;
  }
}

}  // namespace ViewerApp