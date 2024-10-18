#include "MainWindow.hh"

#include "../back/AffineApplier.hh"
#include "../back/Controller.hh"
#include "../back/Model.hh"
#include "../back/ObjectFile.hh"
#include "../back/SettingsHandler.hh"

namespace ViewerApp {

MainWindow::MainWindow(QWidget *parent) : QWidget{parent} {
  Controller &controller = Controller::getInstance();
  controller.attach(&SettingsHandler::getInstance());
  controller.attach(&AffineApplier::getInstance());

  setWindowTitle("3DViewer_v2.0");
  setGeometry(700, 200, 1244, 768);
  setFixedSize(1244, 768);

  gl_view_ = new GLView(this);
  menu_view_ = new MenuView(this, gl_view_);
  controller.attach(gl_view_);

  menu_view_->setGeometry(0, 0, 200, 1000);
  gl_view_->setGeometry(220, 0, 1024, 768);

  scroller_ = new QScrollArea(this);
  scroller_->setGeometry(QRect(QPoint(0, 0), QSize(220, 768)));
  scroller_->setWidget(menu_view_);
}

}  // namespace ViewerApp