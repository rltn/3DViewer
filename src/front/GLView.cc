#include "GLView.hh"

#include <cmath>

namespace ViewerApp {

GLView::GLView(QWidget *parent) : QOpenGLWidget(parent) {
  applyLine();
  applyPoint();
}

GLView::~GLView() {}

void GLView::initializeGL() {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

void GLView::resizeGL(int w, int h) {
  applyProjection(w, h);
  update();
}

void GLView::paintGL() {
  applyProjection(size().width(), size().height());

  Model &model = Model::getInstance();

  Color fill = model.getColorFill();
  glClearColor(fill.red, fill.green, fill.blue, fill.alpha);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!model.getObjectFile().getVertices().empty()) {
    if (!model.getObjectFile().getIndices().empty()) {
      Color line = model.getColorLine();
      glColor3f(line.red, line.green, line.blue);
      glLineWidth(static_cast<float>(model.getLineSize()));
      drawing_line_.get()->drawingStart();
      drawing_line_.get()->drawingDraw();
      drawing_line_.get()->drawingEnd();
    }
    Color point = model.getColorPoint();
    glColor3f(point.red, point.green, point.blue);
    glPointSize(static_cast<float>(model.getPointSize()));
    drawing_point_.get()->drawingStart();
    drawing_point_.get()->drawingDraw();
    drawing_point_.get()->drawingEnd();
  }

  glDisableClientState(GL_VERTEX_ARRAY);
}

void GLView::observerUpdate(Model::Property property) {
  switch (property) {
    case Model::LINE_TYPE:
      applyLine();
      break;
    case Model::POINT_TYPE:
      applyPoint();
      break;
    default:
      break;
  }
  update();
}

void GLView::applyLine() {
  Model &model = Model::getInstance();
  switch (model.getLineType()) {
    case Model::LINE_TYPE_NONE:
      drawing_line_ = std::move(std::make_unique<DrawingStrategyNone>());
      break;
    case Model::LINE_TYPE_SOLID:
      drawing_line_ = std::move(std::make_unique<DrawingStrategyLineSolid>());
      break;
    case Model::LINE_TYPE_DASHED:
      drawing_line_ = std::move(std::make_unique<DrawingStrategyLineDashed>());
      break;
    default:
      throw std::runtime_error("Unknown line type. Cannot apply.");
      break;
  }
}

void GLView::applyPoint() {
  Model &model = Model::getInstance();
  switch (model.getPointType()) {
    case Model::POINT_TYPE_NONE:
      drawing_point_ = std::move(std::make_unique<DrawingStrategyNone>());
      break;
    case Model::POINT_TYPE_CIRCLE:
      drawing_point_ =
          std::move(std::make_unique<DrawingStrategyPointCircle>());
      break;
    case Model::POINT_TYPE_SQUARE:
      drawing_point_ =
          std::move(std::make_unique<DrawingStrategyPointSquare>());
      break;
    default:
      throw std::runtime_error("Unknown point type. Cannot apply.");
      break;
  }
}

void GLView::applyProjection(int w, int h) {
  Model &model = Model::getInstance();

  glViewport(0, 0, w, h);

  float fov = 0.7853981633974483;  // radians(45.0)
  float near = 0.1;
  float far = 100.0;
  float aspect = static_cast<float>(w) / h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  switch (model.getProjectionType()) {
    case Model::PROJECTION_TYPE_CENTRAL: {
      float f = 1.0f / std::tan(fov / 2.0f);
      float rangeInv = 1.0f / (near - far);

      GLfloat matrix[16] = {
          f / aspect, 0.0f, 0.0f,
          0.0f,  // Column 1
          0.0f,       f,    0.0f,
          0.0f,  // Column 2
          0.0f,       0.0f, (near + far) * rangeInv,
          -1.0f,  // Column 3
          0.0f,       0.0f, near * far * rangeInv * 2.0f,
          0.0f  // Column 4
      };
      glLoadMatrixf(matrix);
      break;
    }
    case Model::PROJECTION_TYPE_PARALLEL: {
      float left = -aspect * 2.0;
      float right = aspect * 2.0;
      float bottom = -2.0;
      float top = 2.0;

      GLfloat matrix[16] = {
          2.0f / (right - left),
          0.0f,
          0.0f,
          0.0f,  // Column 1
          0.0f,
          2.0f / (top - bottom),
          0.0f,
          0.0f,  // Column 2
          0.0f,
          0.0f,
          -2.0f / (far - near),
          0.0f,  // Column 3
          -(right + left) / (right - left),
          -(top + bottom) / (top - bottom),
          -(far + near) / (far - near),
          1.0f  // Column 4
      };
      glLoadMatrixf(matrix);
      break;
    }
    default:
      throw std::runtime_error("Unknown projection type. Cannot apply.");
      break;
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  GLfloat base_matrix[16] = {
      1.0f, 0.0f, 0.0f,  0.0f,  // Column 1
      0.0f, 1.0f, 0.0f,  0.0f,  // Column 2
      0.0f, 0.0f, 1.0f,  0.0f,  // Column 3
      0.0f, 0.0f, -3.0f, 1.0f   // Column 4 (translation)
  };
  glLoadMatrixf(base_matrix);
}

}  // namespace ViewerApp