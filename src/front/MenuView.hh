#ifndef CPP4_3DVIEWER_SRC_MENU_H_
#define CPP4_3DVIEWER_SRC_MENU_H_

#include <QColorDialog>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "../back/Controller.hh"
#include "../back/Model.hh"
#include "../back/ObserverInterface.hh"
#include "GLView.hh"

namespace ViewerApp {

class LabelFilename : public QLabel, public IObserver {
  Q_OBJECT

 public:
  explicit LabelFilename(QWidget *parent = nullptr) {
    (void)parent;
    updateText();
  }

  void observerUpdate(Model::Property property) override {
    if (property == Model::OBJECT) {
      updateText();
    }
  }

 protected:
  void updateText() {
    if (Model::getInstance().getObjectFile().getFileName().empty())
      setText(QString::fromStdString("No file provided."));
    else
      setText(QString::fromStdString(
          Model::getInstance().getObjectFile().getFileName()));
  }
};

class LabelVertices : public QLabel, public IObserver {
  Q_OBJECT

 public:
  explicit LabelVertices(QWidget *parent = nullptr) {
    (void)parent;
    updateText();
  }

  void observerUpdate(Model::Property property) override {
    if (property == Model::OBJECT) {
      updateText();
    }
  }

 protected:
  void updateText() {
    setText(QString::fromStdString("Vertices: ") +
            QString::number(
                Model::getInstance().getObjectFile().getVertices().size() / 3));
  }
};

class LabelEdges : public QLabel, public IObserver {
  Q_OBJECT

 public:
  explicit LabelEdges(QWidget *parent = nullptr) {
    (void)parent;
    updateText();
  }

  void observerUpdate(Model::Property property) override {
    if (property == Model::OBJECT) {
      updateText();
    }
  }

 protected:
  void updateText() {
    setText(QString::fromStdString("Edges: ") +
            QString::number(
                Model::getInstance().getObjectFile().getIndices().size() / 2));
  }
};

/// @brief Menu view for options handling.
class MenuView : public QWidget {
  Q_OBJECT

 public:
  explicit MenuView(QWidget *parent, QPointer<GLView> gl_view);
  ~MenuView() = default;

 private:
  Controller &controller_;
  Model &model_;
  QPointer<GLView> gl_view_;

  QPointer<LabelFilename> l_filename_;
  QPointer<LabelVertices> l_vertices_;
  QPointer<LabelEdges> l_edges_;
  QPointer<QPushButton> open_file_b_;
  QPointer<QPushButton> bmp_b_;
  QPointer<QPushButton> jpg_b_;
  QPointer<QPushButton> gif_b_;
  QPointer<QLabel> x_mov_l_;
  QPointer<QSlider> x_mov_s_;
  QPointer<QLabel> y_mov_l_;
  QPointer<QSlider> y_mov_s_;
  QPointer<QLabel> z_mov_l_;
  QPointer<QSlider> z_mov_s_;
  QPointer<QLabel> x_rot_l_;
  QPointer<QSlider> x_rot_s_;
  QPointer<QLabel> y_rot_l_;
  QPointer<QSlider> y_rot_s_;
  QPointer<QLabel> z_rot_l_;
  QPointer<QSlider> z_rot_s_;
  QPointer<QLabel> scale_l_;
  QPointer<QSlider> scale_s_;
  QPointer<QRadioButton> proj_central_rb_;
  QPointer<QRadioButton> proj_parallel_rb_;
  QPointer<QRadioButton> line_solid_rb_;
  QPointer<QRadioButton> line_dashed_rb_;
  QPointer<QPushButton> line_color_b_;
  QPointer<QLabel> line_size_l_;
  QPointer<QSlider> line_size_s_;
  QPointer<QRadioButton> point_none_rb_;
  QPointer<QRadioButton> point_circle_rb_;
  QPointer<QRadioButton> point_square_rb_;
  QPointer<QPushButton> point_color_b_;
  QPointer<QLabel> point_size_l_;
  QPointer<QSlider> point_size_s_;
  QPointer<QPushButton> fill_color_b_;
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_SRC_MENU_H_