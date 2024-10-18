#ifndef CPP4_3DVIEWER_SRC_GLVIEWER_H_
#define CPP4_3DVIEWER_SRC_GLVIEWER_H_

#define GL_SILENCE_DEPRECATION

#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QWidget>
#include <QtOpenGLWidgets>
#include <iostream>

#include "../back/Model.hh"
#include "../back/ObserverInterface.hh"

namespace ViewerApp {

/// @brief Drawing strategies interface.
class IDrawingStrategy {
 public:
  virtual ~IDrawingStrategy() = default;
  virtual void drawingStart() = 0;
  virtual void drawingDraw() = 0;
  virtual void drawingEnd() = 0;
};

class DrawingStrategyNone : public IDrawingStrategy {
  void drawingStart() override {}
  void drawingDraw() override {}
  void drawingEnd() override {}
};

class DrawingStrategyPointSquare : public DrawingStrategyNone {
  void drawingDraw() override {
    Model &model = Model::getInstance();
    auto &vertices = model.getObjectFile().getVertices();
    glBegin(GL_POINTS);
    for (std::size_t i = 0; i < vertices.size(); i += 3) {
      glVertex3d(vertices[i], vertices[i + 1], vertices[i + 2]);
    }
    glEnd();
  }
};

class DrawingStrategyPointCircle : public DrawingStrategyNone {
  void drawingStart() override { glEnable(GL_POINT_SMOOTH); }
  void drawingEnd() override { glDisable(GL_POINT_SMOOTH); }
};

class DrawingStrategyLineSolid : public DrawingStrategyNone {
  void drawingDraw() override {
    Model &model = Model::getInstance();
    glBegin(GL_LINES);
    auto &vertices = model.getObjectFile().getVertices();
    for (auto index : model.getObjectFile().getIndices()) {
      glVertex3d(vertices[index * 3], vertices[index * 3 + 1],
                 vertices[index * 3 + 2]);
    }
    glEnd();
  }
};

class DrawingStrategyLineDashed : public DrawingStrategyLineSolid {
  void drawingStart() override {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  }
  void drawingEnd() override { glDisable(GL_LINE_STIPPLE); }
};

/// @brief GLView widget class which working as an observer.
class GLView : public QOpenGLWidget, public IObserver {
  Q_OBJECT

 public:
  GLView(QWidget *parent = nullptr);
  ~GLView();

 private:
  std::unique_ptr<IDrawingStrategy> drawing_point_, drawing_line_;
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void observerUpdate(Model::Property property) override;
  void applyLine();
  void applyPoint();
  void applyProjection(int w, int h);
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_SRC_GLVIEWER_H_