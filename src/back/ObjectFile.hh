#ifndef CPP4_3DVIEWER_CODE_BACK_OBJECTFILE_H_
#define CPP4_3DVIEWER_CODE_BACK_OBJECTFILE_H_

#include <string>
#include <vector>

namespace ViewerApp {

/// @brief A class for handling an object file loading.
class ObjectFile {
 public:
  /// @brief Makes an empty object.
  ObjectFile();
  /// @brief Loads an object file by the given file path. Makes an empty object
  /// on failure.
  ObjectFile(const std::string& filepath);
  /// @brief Resets vertices to the initial values.
  void resetVertices();

  std::vector<double>& getVertices();
  std::vector<unsigned int>& getIndices();
  const std::string& getFilePath() { return file_path_; }
  const std::string& getFileName() { return file_name_; }

 private:
  void parseFile(const std::string& filepath);
  void parseVertex(std::istringstream& iss, std::vector<double>& vertices);
  void parseFace(std::istringstream& iss,
                 std::vector<std::vector<unsigned int>>& faces);
  void buildIndices(std::vector<std::vector<unsigned int>>& faces);
  void normalizeVertices();
  void dropObject();

  std::vector<double> vertices_src_;
  std::vector<double> vertices_;
  std::vector<unsigned int> indices_;

  std::string file_path_;
  std::string file_name_;
};

}  // namespace ViewerApp

#endif  // CPP4_3DVIEWER_CODE_BACK_OBJECTFILE_H_