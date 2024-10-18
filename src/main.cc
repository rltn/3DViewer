#include <QApplication>

#include "front/MainWindow.hh"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ViewerApp::MainWindow w;
  w.show();
  return a.exec();
}