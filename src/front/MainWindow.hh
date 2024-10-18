#ifndef CPP4_3DVIEWER_SRC_MAINWINDOW_H_
#define CPP4_3DVIEWER_SRC_MAINWINDOW_H_

#include <QImage>
#include <QScrollArea>
#include <QTime>
#include <QWidget>
#include <iostream>

#include "GLView.hh"
#include "MenuView.hh"

namespace ViewerApp {

/// @brief Main window class.
class MainWindow : public QWidget {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);

  GLView *gl_view_;
  QScrollArea *scroller_;
  MenuView *menu_view_;
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_SRC_MAINWINDOW_H_