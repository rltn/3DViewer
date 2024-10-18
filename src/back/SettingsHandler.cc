#include "SettingsHandler.hh"

#include <QCoreApplication>
#include <QSettings>
#include <QString>

#include "Controller.hh"
#include "Model.hh"
#include "ObserverInterface.hh"

namespace ViewerApp {

std::mutex SettingsHandler::mutex_;

SettingsHandler::SettingsHandler() {
  Model& model = Model::getInstance();
  QSettings set_f(QCoreApplication::applicationDirPath() + "/settings.ini",
                  QSettings::IniFormat);
  set_f.beginGroup("Settings");
  model.setProperty(set_f.value("move_x", 0.0).toFloat(), Model::MOVE_X);
  model.setProperty(set_f.value("move_y", 0.0).toFloat(), Model::MOVE_Y);
  model.setProperty(set_f.value("move_z", 0.0).toFloat(), Model::MOVE_Z);
  model.setProperty(set_f.value("rotate_x", 0.0).toFloat(), Model::ROTATE_X);
  model.setProperty(set_f.value("rotate_y", 0.0).toFloat(), Model::ROTATE_Y);
  model.setProperty(set_f.value("rotate_z", 0.0).toFloat(), Model::ROTATE_Z);
  model.setProperty(set_f.value("scale", 1.0).toFloat(), Model::SCALE);
  model.setProperty(set_f.value("point_size", 1).toInt(), Model::POINT_SIZE);
  model.setProperty(set_f.value("line_size", 1).toInt(), Model::LINE_SIZE);
  model.setProperty(set_f.value("object_path", "").toString().toStdString(),
                    Model::OBJECT);
  model.setProperty(Color(set_f.value("color_point_r", 1.0).toFloat(),
                          set_f.value("color_point_g", 0.0).toFloat(),
                          set_f.value("color_point_b", 0.0).toFloat()),
                    Model::COLOR_POINT);
  model.setProperty(Color(set_f.value("color_line_r", 0.0).toFloat(),
                          set_f.value("color_line_g", 1.0).toFloat(),
                          set_f.value("color_line_b", 0.0).toFloat()),
                    Model::COLOR_LINE);
  model.setProperty(Color(set_f.value("color_fill_r", 0.0).toFloat(),
                          set_f.value("color_fill_g", 0.0).toFloat(),
                          set_f.value("color_fill_b", 0.0).toFloat()),
                    Model::COLOR_FILL);
  model.setProperty(
      static_cast<Model::PointType>(set_f.value("point_type", 0).toInt()),
      Model::POINT_TYPE);
  model.setProperty(
      static_cast<Model::LineType>(set_f.value("line_type", 1).toInt()),
      Model::LINE_TYPE);
  model.setProperty(static_cast<Model::ProjectionType>(
                        set_f.value("projection_type", 1).toInt()),
                    Model::PROJECTION_TYPE);
  set_f.endGroup();

  connect(&save_timer_, &QTimer::timeout, this, &SettingsHandler::saveSettings);
  save_timer_.setSingleShot(true);
}

SettingsHandler::~SettingsHandler() {}

SettingsHandler& SettingsHandler::getInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  static SettingsHandler instance;
  return instance;
}

void SettingsHandler::observerUpdate(Model::Property property) {
  (void)property;

  save_timer_.start(2000);
}

void SettingsHandler::saveSettings() {
  Model& model = Model::getInstance();
  QSettings set_f(QCoreApplication::applicationDirPath() + "/settings.ini",
                  QSettings::IniFormat);
  set_f.beginGroup("Settings");
  set_f.setValue("move_x", model.getMoveX());
  set_f.setValue("move_y", model.getMoveY());
  set_f.setValue("move_z", model.getMoveZ());
  set_f.setValue("rotate_x", model.getRotateX());
  set_f.setValue("rotate_y", model.getRotateY());
  set_f.setValue("rotate_z", model.getRotateZ());
  set_f.setValue("scale", model.getScale());
  set_f.setValue("point_size", model.getPointSize());
  set_f.setValue("line_size", model.getLineSize());
  set_f.setValue("object_path",
                 QString::fromStdString(model.getObjectFile().getFilePath()));
  set_f.setValue("color_point_r", model.getColorPoint().red);
  set_f.setValue("color_point_g", model.getColorPoint().green);
  set_f.setValue("color_point_b", model.getColorPoint().blue);
  set_f.setValue("color_line_r", model.getColorLine().red);
  set_f.setValue("color_line_g", model.getColorLine().green);
  set_f.setValue("color_line_b", model.getColorLine().blue);
  set_f.setValue("color_fill_r", model.getColorFill().red);
  set_f.setValue("color_fill_g", model.getColorFill().green);
  set_f.setValue("color_fill_b", model.getColorFill().blue);
  set_f.setValue("point_type", static_cast<int>(model.getPointType()));
  set_f.setValue("line_type", static_cast<int>(model.getLineType()));
  set_f.setValue("projection_type",
                 static_cast<int>(model.getProjectionType()));
  set_f.endGroup();
}

}  // namespace ViewerApp