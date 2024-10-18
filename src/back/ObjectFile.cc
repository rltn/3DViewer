#include "ObjectFile.hh"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>

namespace ViewerApp {

class FileOpenException : public std::runtime_error {
 public:
  FileOpenException(const std::string& filepath)
      : std::runtime_error("Failed to open file: " + filepath) {}
};

class VertexParseException : public std::runtime_error {
 public:
  VertexParseException() : std::runtime_error("Error parsing vertex.") {}
};

class FaceParseException : public std::runtime_error {
 public:
  FaceParseException() : std::runtime_error("Error parsing face.") {}
};

class IndexOutOfRangeException : public std::runtime_error {
 public:
  IndexOutOfRangeException()
      : std::runtime_error("Object index out of range.") {}
};

ObjectFile::ObjectFile() { dropObject(); }

ObjectFile::ObjectFile(const std::string& filepath) {
  try {
    parseFile(filepath);
  } catch (const std::exception& e) {
    // std::cerr << "Exception: " << e.what() << std::endl;
    dropObject();
  }
}

void ObjectFile::resetVertices() { vertices_ = vertices_src_; }

std::vector<double>& ObjectFile::getVertices() { return vertices_; }

std::vector<unsigned int>& ObjectFile::getIndices() { return indices_; }

void ObjectFile::parseFile(const std::string& filepath) {
  std::unique_ptr<std::ifstream, void (*)(std::ifstream*)> file(
      new std::ifstream(filepath), [](std::ifstream* f) {
        if (f->is_open()) f->close();
        delete f;
      });
  std::vector<std::vector<unsigned int>> faces;
  file_path_ = filepath;
  file_name_ = std::filesystem::path(filepath).filename().string();

  if (!file->is_open()) throw FileOpenException(filepath);

  std::string line;
  while (std::getline(*file, line)) {
    std::istringstream iss(line);
    char buf[2];
    iss.read(buf, 2);
    if (iss && buf[1] == ' ') {
      if (buf[0] == 'v')
        parseVertex(iss, vertices_src_);
      else if (buf[0] == 'f')
        parseFace(iss, faces);
    }
  }

  std::size_t vertex_size = vertices_src_.size() / 3;
  for (auto& face : faces)
    for (auto index : face)
      if (index > vertex_size) throw IndexOutOfRangeException();

  buildIndices(faces);
  normalizeVertices();
  resetVertices();
}

void ObjectFile::parseVertex(std::istringstream& iss,
                             std::vector<double>& vertices) {
  double x, y, z;
  if (!(iss >> x)) throw VertexParseException();
  if (iss.peek() == '/') {
    iss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
  }
  if (!(iss >> y)) throw VertexParseException();
  if (iss.peek() == '/') {
    iss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
  }
  if (!(iss >> z)) throw VertexParseException();
  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(z);
}

void ObjectFile::parseFace(std::istringstream& iss,
                           std::vector<std::vector<unsigned int>>& faces) {
  std::vector<unsigned int> face;
  unsigned int index;
  while (iss >> index) {
    face.push_back(index);
    if (iss.peek() == '/') {
      iss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    }
  }
  if (face.size() < 3) throw FaceParseException();
  faces.push_back(face);
}

void ObjectFile::buildIndices(std::vector<std::vector<unsigned int>>& faces) {
  std::vector<std::vector<unsigned int>> graph(vertices_src_.size() / 3);

  for (auto& face : faces) {
    for (std::size_t i = 0; i < face.size(); ++i) {
      unsigned int val_from = face[i] - 1;
      unsigned int val_to = face[i == 0 ? face.size() - 1 : i - 1] - 1;
      if (val_from == val_to) continue;
      if (val_from > val_to) std::swap(val_from, val_to);
      if (std::find_if(graph[val_from].begin(), graph[val_from].end(),
                       [val_to](unsigned int val) { return val == val_to; }) ==
          graph[val_from].end())
        graph[val_from].push_back(val_to);
    }
  }

  for (unsigned int node_i = 0; node_i < graph.size(); ++node_i) {
    for (unsigned int index : graph[node_i]) {
      indices_.push_back(node_i);
      indices_.push_back(index);
    }
  }
}

void ObjectFile::normalizeVertices() {
  if (vertices_src_.size() < 3) return;

  double x_min, y_min, z_min, x_max, y_max, z_max;
  x_min = x_max = vertices_src_[0];
  y_min = y_max = vertices_src_[1];
  z_min = z_max = vertices_src_[2];
  for (std::size_t i = 3; i < vertices_src_.size(); i += 3) {
    x_min = std::min(x_min, vertices_src_[i]);
    x_max = std::max(x_max, vertices_src_[i]);
    y_min = std::min(y_min, vertices_src_[i + 1]);
    y_max = std::max(y_max, vertices_src_[i + 1]);
    z_min = std::min(z_min, vertices_src_[i + 2]);
    z_max = std::max(z_max, vertices_src_[i + 2]);
  }
  float delta = std::max(std::max(x_max - x_min, y_max - y_min), z_max - z_min);

  for (std::size_t i = 0; i < vertices_src_.size(); i += 3) {
    vertices_src_[i] -= (x_min + x_max) / 2.;
    vertices_src_[i] /= delta;
    vertices_src_[i + 1] -= (y_min + y_max) / 2.;
    vertices_src_[i + 1] /= delta;
    vertices_src_[i + 2] -= (z_min + z_max) / 2.;
    vertices_src_[i + 2] /= delta;
  }
}

void ObjectFile::dropObject() {
  vertices_src_.clear();
  vertices_.clear();
  indices_.clear();
  file_path_.clear();
  file_name_.clear();
}

}  // namespace ViewerApp