#include <core.hpp>
#include <ui.hpp>
#include <filetools.hpp>

#include <map>
#include <fstream>
#include <string>
#include <functional>
#include <filesystem>

std::map<std::string, std::function<void(std::string)>> functions;

void loadProfile(std::string& modelpath, std::string& systemprompt) {
  std::string configDir = std::getenv("HOME") + std::string("/.config/miko");
  std::string modelDir = std::getenv("HOME") + std::string("/.local/share/");
  if (!std::filesystem::exists(configDir)) {
    std::filesystem::create_directory(configDir);
    std::filesystem::create_directory(configDir + "/profiles");
  }
  if (!std::filesystem::exists(modelDir)) {
    std::filesystem::create_directory(modelDir);
    std::cerr << "Model folder is empty! Please download a gguf and place it in '" << modelDir << "'\n";
    std::exit(1);
  }
  if (!std::filesystem::exists(configDir + "/miko.conf")) {
    std::ofstream file(configDir + "/miko.conf");
    file << "profile_choice=\nmodel_choice=\n";
    file.close();
  }

  configmap map = file_to_map(configDir + "miko.conf");
  modelpath = map[map["model_choice"]];
  if (modelpath.empty()) {
    std::cerr << "Invalid model choice!\n";
    std::exit(1);
  }
}

int main(int argc, const char* argv[]) {
  std::string homeDir = std::getenv("HOME");
  std::string modelPath;
  std::string systemPrompt;

  debugLog("Loading Model...");
  loadProfile(modelPath, systemPrompt);
  Miko miko(modelPath.c_str(), 8192, systemPrompt);
  
  Screen screen;
  screen.render();

  std::string userInput;
  while (true) {
    std::getline(std::cin, userInput);
    miko.chat(userInput);
  }
}
