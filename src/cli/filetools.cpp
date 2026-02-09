#include <filetools.hpp>

configmap file_to_map(std::string filepath) {
  configmap map;
  std::ifstream file(filepath);
  std::string name;
  std::string path;
  while (std::getline(file, name, '=') && std::getline(file, path)) {
    map[name] = path;
  }
  file.close();
  return map;
}

void map_to_file(configmap map, std::string filepath) {
  std::ofstream file(filepath);
  for (const auto& pair : map) {
    file << pair.first << '=' << pair.second << std::endl;
  }
  file.close();
}
