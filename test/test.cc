#include <gtest/gtest.h>

#include "../src/back/AffineApplier.hh"
#include "../src/back/Controller.hh"
#include "../src/back/Model.hh"
#include "../src/back/ObjectFile.hh"

namespace s21 {

class ObjectLoadingTest : public ::testing::Test {};

TEST_F(ObjectLoadingTest, None) {
  ObjectFile object;
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

TEST_F(ObjectLoadingTest, Unexisting) {
  ObjectFile object("");
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

TEST_F(ObjectLoadingTest, Empty) {
  ObjectFile object("test/models/empty");
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

TEST_F(ObjectLoadingTest, LoadS0) {
  ObjectFile object("test/models/load_0");
  EXPECT_FALSE(object.getIndices().empty());
  EXPECT_FALSE(object.getVertices().empty());
  object.getIndices();
  EXPECT_EQ(object.getIndices().size(), 16);
  EXPECT_EQ(object.getVertices().size(), 15);
}

TEST_F(ObjectLoadingTest, LoadF1) {
  ObjectFile object("test/models/load_1");
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

TEST_F(ObjectLoadingTest, LoadF2) {
  ObjectFile object("test/models/load_2");
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

TEST_F(ObjectLoadingTest, LoadF3) {
  ObjectFile object("test/models/load_3");
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

TEST_F(ObjectLoadingTest, LoadF4) {
  ObjectFile object("test/models/load_4");
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

TEST_F(ObjectLoadingTest, LoadF5) {
  ObjectFile object("test/models/load_5");
  EXPECT_TRUE(object.getIndices().empty());
  EXPECT_TRUE(object.getVertices().empty());
}

class ModelControllerAffineTest : public ::testing::Test {
 public:
  ModelControllerAffineTest()
      : controller(Controller::getInstance()), model(Model::getInstance()) {
    controller.attach(&AffineApplier::getInstance());
  }

 protected:
  Controller &controller;
  Model &model;
};

TEST_F(ModelControllerAffineTest, StartState) {
  EXPECT_EQ(model.getMoveX(), 0.0F);
  EXPECT_EQ(model.getMoveY(), 0.0F);
  EXPECT_EQ(model.getMoveZ(), 0.0F);
  EXPECT_EQ(model.getRotateX(), 0.0F);
  EXPECT_EQ(model.getRotateY(), 0.0F);
  EXPECT_EQ(model.getRotateZ(), 0.0F);
  EXPECT_EQ(model.getScale(), 1.0F);
  EXPECT_EQ(model.getLineSize(), 1);
  EXPECT_EQ(model.getPointSize(), 1);
  EXPECT_EQ(model.getColorPoint().red, 0.0);
  EXPECT_EQ(model.getColorPoint().green, 0.0);
  EXPECT_EQ(model.getColorPoint().blue, 0.0);
  EXPECT_EQ(model.getColorLine().red, 0.0);
  EXPECT_EQ(model.getColorLine().green, 0.0);
  EXPECT_EQ(model.getColorLine().blue, 0.0);
  EXPECT_EQ(model.getColorFill().red, 0.0);
  EXPECT_EQ(model.getColorFill().green, 0.0);
  EXPECT_EQ(model.getColorFill().blue, 0.0);
  EXPECT_EQ(model.getLineType(), Model::LINE_TYPE_SOLID);
  EXPECT_EQ(model.getPointType(), Model::POINT_TYPE_NONE);
  EXPECT_EQ(model.getProjectionType(), Model::PROJECTION_TYPE_CENTRAL);
  EXPECT_TRUE(model.getObjectFile().getVertices().empty());
  EXPECT_TRUE(model.getObjectFile().getIndices().empty());
}

TEST_F(ModelControllerAffineTest, SetPropertyValid) {
  controller.setContext(0.0F, Model::MOVE_X);
  controller.setContext(0.0F, Model::MOVE_Y);
  controller.setContext(0.0F, Model::MOVE_Z);
  controller.setContext(0.0F, Model::ROTATE_X);
  controller.setContext(0.0F, Model::ROTATE_Y);
  controller.setContext(0.0F, Model::ROTATE_Z);
  controller.setContext(2.0F, Model::SCALE);
  controller.setContext(Color(), Model::COLOR_POINT);
  controller.setContext(Color(), Model::COLOR_LINE);
  controller.setContext(Color(), Model::COLOR_FILL);
  controller.setContext(Model::LINE_TYPE_SOLID, Model::LINE_TYPE);
  controller.setContext(Model::POINT_TYPE_NONE, Model::POINT_TYPE);
  controller.setContext(1, Model::LINE_SIZE);
  controller.setContext(1, Model::POINT_SIZE);
  controller.setContext(Model::PROJECTION_TYPE_CENTRAL, Model::PROJECTION_TYPE);
  controller.setContext("test/models/point", Model::OBJECT);
}

TEST_F(ModelControllerAffineTest, SetPropertyInvalid) {
  EXPECT_THROW(controller.setContext(0.0F, Model::INVALID_PROPERTY),
               std::runtime_error);
  EXPECT_THROW(controller.setContext(0, Model::INVALID_PROPERTY),
               std::runtime_error);
  EXPECT_THROW(controller.setContext("", Model::INVALID_PROPERTY),
               std::runtime_error);
  EXPECT_THROW(controller.setContext(Color(), Model::INVALID_PROPERTY),
               std::runtime_error);
  EXPECT_THROW(
      controller.setContext(Model::LINE_TYPE_NONE, Model::INVALID_PROPERTY),
      std::runtime_error);
  EXPECT_THROW(
      controller.setContext(Model::POINT_TYPE_NONE, Model::INVALID_PROPERTY),
      std::runtime_error);
  EXPECT_THROW(controller.setContext(Model::PROJECTION_TYPE_NONE,
                                     Model::INVALID_PROPERTY),
               std::runtime_error);
}

TEST_F(ModelControllerAffineTest, AffineScale) {
  controller.setContext(4.0F, Model::SCALE);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[0], 2.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[1], 2.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[2], 2.0);
  controller.setContext(2.0F, Model::SCALE);
}

TEST_F(ModelControllerAffineTest, AffineRotateX) {
  controller.setContext(180.0F, Model::ROTATE_X);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[0], 1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[1], -1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[2], -1.0);
  controller.setContext(0.0F, Model::ROTATE_X);
}

TEST_F(ModelControllerAffineTest, AffineRotateY) {
  controller.setContext(180.0F, Model::ROTATE_Y);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[0], -1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[1], 1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[2], -1.0);
  controller.setContext(0.0F, Model::ROTATE_Y);
}

TEST_F(ModelControllerAffineTest, AffineRotateZ) {
  controller.setContext(180.0F, Model::ROTATE_Z);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[0], -1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[1], -1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[2], 1.0);
  controller.setContext(0.0F, Model::ROTATE_Z);
}

TEST_F(ModelControllerAffineTest, AffineMoveX) {
  controller.setContext(1.0F, Model::MOVE_X);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[0], 2.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[1], 1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[2], 1.0);
  controller.setContext(0.0F, Model::MOVE_X);
}

TEST_F(ModelControllerAffineTest, AffineMoveY) {
  controller.setContext(1.0F, Model::MOVE_Y);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[0], 1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[1], 2.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[2], 1.0);
  controller.setContext(0.0F, Model::MOVE_Y);
}

TEST_F(ModelControllerAffineTest, AffineMoveZ) {
  controller.setContext(1.0F, Model::MOVE_Z);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[0], 1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[1], 1.0);
  EXPECT_DOUBLE_EQ(model.getObjectFile().getVertices()[2], 2.0);
  controller.setContext(0.0F, Model::MOVE_Z);
}

TEST_F(ModelControllerAffineTest, ControllerDetach) {
  controller.detach(&AffineApplier::getInstance());
}

}  // namespace s21

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}