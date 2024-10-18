#ifndef CPP4_3DVIEWER_CODE_BACK_MODEL_H_
#define CPP4_3DVIEWER_CODE_BACK_MODEL_H_

#include <mutex>

#include "ObjectFile.hh"

namespace ViewerApp {

/// @brief Color structure which can handle an RGBA pallette.
struct Color {
  double red, green, blue, alpha;
  Color(double r, double g, double b, double a)
      : red(r), green(g), blue(b), alpha(a) {}
  Color(double r, double g, double b) : red(r), green(g), blue(b), alpha(1) {}
  Color() : red(0), green(0), blue(0), alpha(1) {}
};

/// @brief Model instance class for storing and accessing data
class Model {
 public:
  /// @brief Avaliable properties enum.
  enum Property {
    INVALID_PROPERTY,
    MOVE_X,
    MOVE_Y,
    MOVE_Z,
    ROTATE_X,
    ROTATE_Y,
    ROTATE_Z,
    SCALE,
    COLOR_POINT,
    COLOR_LINE,
    COLOR_FILL,
    LINE_TYPE,
    POINT_TYPE,
    LINE_SIZE,
    POINT_SIZE,
    PROJECTION_TYPE,
    OBJECT
  };

  /// @brief Avaliable line types enum.
  enum LineType { LINE_TYPE_NONE, LINE_TYPE_SOLID, LINE_TYPE_DASHED };

  /// @brief Avaliable point types enum.
  enum PointType { POINT_TYPE_NONE, POINT_TYPE_CIRCLE, POINT_TYPE_SQUARE };

  /// @brief Avaliable projection types enum.
  enum ProjectionType {
    PROJECTION_TYPE_NONE,
    PROJECTION_TYPE_CENTRAL,
    PROJECTION_TYPE_PARALLEL
  };

  /// @brief Non cloneable.
  Model(Model &other) = delete;

  /// @brief Non assignable.
  void operator=(const Model &) = delete;

  /// @brief Thread-safe instance accessor.
  static Model &getInstance();

  /// @brief Assignes a value to the specified property. Throws
  /// std::runtime_error on incorrect property type.
  void setProperty(float value, Property property);
  void setProperty(int value, Property property);
  void setProperty(const std::string &value, Property property);
  void setProperty(Color value, Property property);
  void setProperty(LineType value, Property property);
  void setProperty(PointType value, Property property);
  void setProperty(ProjectionType value, Property property);

  /// @brief Data accessors.
  float getMoveX() { return move_x_; }
  float getMoveY() { return move_y_; }
  float getMoveZ() { return move_z_; }
  float getRotateX() { return rotate_x_; }
  float getRotateY() { return rotate_y_; }
  float getRotateZ() { return rotate_z_; }
  float getScale() { return scale_; }
  int getLineSize() { return line_size_; }
  int getPointSize() { return point_size_; }
  Color getColorPoint() { return color_point_; }
  Color getColorLine() { return color_line_; }
  Color getColorFill() { return color_fill_; }
  LineType getLineType() { return line_type_; }
  PointType getPointType() { return point_type_; }
  ProjectionType getProjectionType() { return projection_type_; }
  ObjectFile &getObjectFile() { return object_; }

 private:
  Model();
  ~Model();

  float move_x_, move_y_, move_z_;
  float rotate_x_, rotate_y_, rotate_z_;
  float scale_;

  int line_size_, point_size_;

  Color color_point_, color_line_, color_fill_;

  LineType line_type_;
  PointType point_type_;
  ProjectionType projection_type_;

  ObjectFile object_;

  static std::mutex mutex_;
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_CODE_BACK_MODEL_H_