#include "MenuView.hh"

#include <iostream>

#include "../../libs/QtGifImage/gifimage/qgifimage.h"

namespace ViewerApp {

MenuView::MenuView(QWidget *parent, QPointer<GLView> gl_view)
    : QWidget{parent},
      controller_(Controller::getInstance()),
      model_(Model::getInstance()),
      gl_view_(gl_view) {
  open_file_b_ = new QPushButton("Open file", this);
  connect(open_file_b_, &QPushButton::clicked, this, [this]() {
    const std::string file_path =
        QFileDialog::getOpenFileName(this, "", "", "Object Files (*.obj)")
            .toLocal8Bit()
            .constData();
    controller_.setContext(file_path, Model::OBJECT);
  });

  l_filename_ = new LabelFilename(this);
  controller_.attach(l_filename_);

  l_vertices_ = new LabelVertices(this);
  controller_.attach(l_vertices_);

  l_edges_ = new LabelEdges(this);
  controller_.attach(l_edges_);

  bmp_b_ = new QPushButton("Generate an output.bmp", this);
  connect(bmp_b_, &QPushButton::clicked, this, [this]() {
    gl_view_->grabFramebuffer().save("output.bmp", "BMP");
    std::cout << "output.bmp is ready." << std::endl;
  });

  jpg_b_ = new QPushButton("Generate an output.jpg", this);
  connect(jpg_b_, &QPushButton::clicked, this, [this]() {
    gl_view_->grabFramebuffer().save("output.jpg", "JPG", 12);
    std::cout << "output.jpg is ready." << std::endl;
  });

  gif_b_ = new QPushButton("Generate an output.gif", this);
  connect(gif_b_, &QPushButton::clicked, this, [this]() {
    gif_b_->setEnabled(false);
    std::cout << "output.gif is creating." << std::endl;

    QGifImage gif_image;
    gif_image.setDefaultDelay(10);

    for (int i = 0; i < 50; ++i) {
      QImage screen = gl_view_->grabFramebuffer();
      gif_image.addFrame(screen);
      QTime time = QTime::currentTime().addMSecs(100);
      while (QTime::currentTime() < time)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }

    gif_image.save("output.gif");
    std::cout << "output.gif is ready." << std::endl;
    gif_b_->setEnabled(true);
  });

  x_mov_l_ = new QLabel("Move along the axis X", this);
  x_mov_s_ = new QSlider(Qt::Horizontal, this);
  x_mov_s_->setRange(-200, 200);
  x_mov_s_->setValue(static_cast<int>(model_.getMoveX() * 100));
  connect(x_mov_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(static_cast<float>(value) * 0.01F, Model::MOVE_X);
  });
  connect(x_mov_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(static_cast<float>(x_mov_s_->value()) * 0.01F,
                           Model::MOVE_X);
  });

  y_mov_l_ = new QLabel("Move along the axis Y", this);
  y_mov_s_ = new QSlider(Qt::Horizontal, this);
  y_mov_s_->setRange(-200, 200);
  y_mov_s_->setValue(static_cast<int>(model_.getMoveY() * 100));
  connect(y_mov_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(static_cast<float>(value) * 0.01F, Model::MOVE_Y);
  });
  connect(y_mov_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(static_cast<float>(y_mov_s_->value()) * 0.01F,
                           Model::MOVE_Y);
  });

  z_mov_l_ = new QLabel("Move along the axis Z", this);
  z_mov_s_ = new QSlider(Qt::Horizontal, this);
  z_mov_s_->setRange(-200, 200);
  z_mov_s_->setValue(static_cast<int>(model_.getMoveZ() * 100));
  connect(z_mov_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(static_cast<float>(value) * 0.01F, Model::MOVE_Z);
  });
  connect(z_mov_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(static_cast<float>(z_mov_s_->value()) * 0.01F,
                           Model::MOVE_Z);
  });

  x_rot_l_ = new QLabel("Rotate around the axis X", this);
  x_rot_s_ = new QSlider(Qt::Horizontal, this);
  x_rot_s_->setRange(0, 360);
  x_rot_s_->setValue(static_cast<int>(model_.getRotateX()));
  connect(x_rot_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(static_cast<float>(value), Model::ROTATE_X);
  });
  connect(x_rot_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(static_cast<float>(x_rot_s_->value()),
                           Model::ROTATE_X);
  });

  y_rot_l_ = new QLabel("Rotate around the axis Y", this);
  y_rot_s_ = new QSlider(Qt::Horizontal, this);
  y_rot_s_->setRange(0, 360);
  y_rot_s_->setValue(static_cast<int>(model_.getRotateY()));
  connect(y_rot_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(static_cast<float>(value), Model::ROTATE_Y);
  });
  connect(y_rot_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(static_cast<float>(y_rot_s_->value()),
                           Model::ROTATE_Y);
  });

  z_rot_l_ = new QLabel("Rotate around the axis Z", this);
  z_rot_s_ = new QSlider(Qt::Horizontal, this);
  z_rot_s_->setRange(0, 360);
  z_rot_s_->setValue(static_cast<int>(model_.getRotateZ()));
  connect(z_rot_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(static_cast<float>(value), Model::ROTATE_Z);
  });
  connect(z_rot_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(static_cast<float>(z_rot_s_->value()),
                           Model::ROTATE_Z);
  });

  scale_l_ = new QLabel("Scale", this);
  scale_s_ = new QSlider(Qt::Horizontal, this);
  scale_s_->setRange(10, 200);
  scale_s_->setValue(static_cast<int>(model_.getScale() * 100));
  connect(scale_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(static_cast<float>(value) * 0.01F, Model::SCALE);
  });
  connect(scale_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(static_cast<float>(scale_s_->value()) * 0.01F,
                           Model::SCALE);
  });

  auto projection_layout = new QVBoxLayout(this);
  proj_central_rb_ = new QRadioButton("Central", this);
  proj_central_rb_->setChecked(model_.getProjectionType() ==
                               Model::PROJECTION_TYPE_CENTRAL);
  proj_parallel_rb_ = new QRadioButton("Parallel", this);
  proj_central_rb_->setChecked(model_.getProjectionType() ==
                               Model::PROJECTION_TYPE_PARALLEL);
  projection_layout->addWidget(proj_central_rb_);
  projection_layout->addWidget(proj_parallel_rb_);
  QGroupBox *projection_box = new QGroupBox("Projection type", this);
  projection_box->setLayout(projection_layout);
  connect(proj_central_rb_, &QRadioButton::clicked, this, [this]() {
    controller_.setContext(Model::PROJECTION_TYPE_CENTRAL,
                           Model::PROJECTION_TYPE);
  });
  connect(proj_parallel_rb_, &QRadioButton::clicked, [this]() {
    controller_.setContext(Model::PROJECTION_TYPE_PARALLEL,
                           Model::PROJECTION_TYPE);
  });

  auto line_type_layout = new QVBoxLayout(this);
  line_solid_rb_ = new QRadioButton("Solid", this);
  line_solid_rb_->setChecked(model_.getLineType() == Model::LINE_TYPE_SOLID);
  line_dashed_rb_ = new QRadioButton("Dashed", this);
  line_dashed_rb_->setChecked(model_.getLineType() == Model::LINE_TYPE_DASHED);
  line_type_layout->addWidget(line_solid_rb_);
  line_type_layout->addWidget(line_dashed_rb_);
  QGroupBox *line_type_box = new QGroupBox("Type of lines", this);
  line_type_box->setLayout(line_type_layout);
  connect(line_solid_rb_, &QRadioButton::clicked, this, [this]() {
    controller_.setContext(Model::LINE_TYPE_SOLID, Model::LINE_TYPE);
  });
  connect(line_dashed_rb_, &QRadioButton::clicked, this, [this]() {
    controller_.setContext(Model::LINE_TYPE_DASHED, Model::LINE_TYPE);
  });

  line_color_b_ = new QPushButton("Set lines color", this);
  connect(line_color_b_, &QPushButton::clicked, this, [this]() {
    QColor color =
        QColorDialog::getColor(Qt::white, this, "Select lines color");
    if (color.isValid()) {
      int red, green, blue;
      color.getRgb(&red, &green, &blue);
      controller_.setContext(Color(red / 255., green / 255., blue / 255.),
                             Model::COLOR_LINE);
    }
  });

  line_size_l_ = new QLabel("Size of lines", this);
  line_size_s_ = new QSlider(Qt::Horizontal, this);
  line_size_s_->setRange(1, 20);
  line_size_s_->setValue(model_.getLineSize());
  connect(line_size_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(value, Model::LINE_SIZE);
  });
  connect(line_size_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(line_size_s_->value(), Model::LINE_SIZE);
  });

  auto point_type_layout = new QVBoxLayout(this);
  point_none_rb_ = new QRadioButton("None", this);
  point_none_rb_->setChecked(model_.getPointType() == Model::POINT_TYPE_NONE);
  point_circle_rb_ = new QRadioButton("Circle", this);
  point_circle_rb_->setChecked(model_.getPointType() ==
                               Model::POINT_TYPE_CIRCLE);
  point_square_rb_ = new QRadioButton("Square", this);
  point_square_rb_->setChecked(model_.getPointType() ==
                               Model::POINT_TYPE_SQUARE);
  point_type_layout->addWidget(point_none_rb_);
  point_type_layout->addWidget(point_circle_rb_);
  point_type_layout->addWidget(point_square_rb_);
  QGroupBox *point_type_box = new QGroupBox("Type of points", this);
  point_type_box->setLayout(point_type_layout);
  connect(point_none_rb_, &QRadioButton::clicked, this, [this]() {
    controller_.setContext(Model::POINT_TYPE_NONE, Model::POINT_TYPE);
  });
  connect(point_circle_rb_, &QRadioButton::clicked, this, [this]() {
    controller_.setContext(Model::POINT_TYPE_CIRCLE, Model::POINT_TYPE);
  });
  connect(point_square_rb_, &QRadioButton::clicked, this, [this]() {
    controller_.setContext(Model::POINT_TYPE_SQUARE, Model::POINT_TYPE);
  });

  point_color_b_ = new QPushButton("Set points color", this);
  connect(point_color_b_, &QPushButton::clicked, this, [this]() {
    QColor color =
        QColorDialog::getColor(Qt::white, this, "Select points color");
    if (color.isValid()) {
      int red, green, blue;
      color.getRgb(&red, &green, &blue);
      controller_.setContext(Color(red / 255., green / 255., blue / 255.),
                             Model::COLOR_POINT);
    }
  });

  point_size_l_ = new QLabel("Size of points", this);
  point_size_s_ = new QSlider(Qt::Horizontal, this);
  point_size_s_->setRange(1, 20);
  point_size_s_->setValue(model_.getPointSize());
  connect(point_size_s_, &QSlider::sliderMoved, this, [this](int value) {
    controller_.setContext(value, Model::POINT_SIZE);
  });
  connect(point_size_s_, &QSlider::sliderPressed, this, [this]() {
    controller_.setContext(point_size_s_->value(), Model::POINT_SIZE);
  });

  fill_color_b_ = new QPushButton("Set fill color", this);
  connect(fill_color_b_, &QPushButton::clicked, this, [this]() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Select fill color");
    if (color.isValid()) {
      int red, green, blue;
      color.getRgb(&red, &green, &blue);
      controller_.setContext(Color(red / 255., green / 255., blue / 255.),
                             Model::COLOR_FILL);
    }
  });

  auto menu_layout = new QVBoxLayout(this);
  menu_layout->addWidget(open_file_b_);
  menu_layout->addWidget(l_filename_);
  menu_layout->addWidget(l_vertices_);
  menu_layout->addWidget(l_edges_);
  menu_layout->addWidget(projection_box);
  menu_layout->addWidget(line_type_box);
  menu_layout->addWidget(line_color_b_);
  menu_layout->addWidget(line_size_l_);
  menu_layout->addWidget(line_size_s_);
  menu_layout->addWidget(point_type_box);
  menu_layout->addWidget(point_color_b_);
  menu_layout->addWidget(point_size_l_);
  menu_layout->addWidget(point_size_s_);
  menu_layout->addWidget(fill_color_b_);
  menu_layout->addWidget(x_mov_l_);
  menu_layout->addWidget(x_mov_s_);
  menu_layout->addWidget(y_mov_l_);
  menu_layout->addWidget(y_mov_s_);
  menu_layout->addWidget(z_mov_l_);
  menu_layout->addWidget(z_mov_s_);
  menu_layout->addWidget(x_rot_l_);
  menu_layout->addWidget(x_rot_s_);
  menu_layout->addWidget(y_rot_l_);
  menu_layout->addWidget(y_rot_s_);
  menu_layout->addWidget(z_rot_l_);
  menu_layout->addWidget(z_rot_s_);
  menu_layout->addWidget(scale_l_);
  menu_layout->addWidget(scale_s_);
  menu_layout->addWidget(bmp_b_);
  menu_layout->addWidget(jpg_b_);
  menu_layout->addWidget(gif_b_);
  setLayout(menu_layout);
}

}  // namespace ViewerApp