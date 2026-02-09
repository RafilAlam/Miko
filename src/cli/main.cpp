#include <core.hpp>
#include <ui.hpp>

#include <map>
#include <fstream>
#include <string>
#include <functional>
#include <filesystem>


map<std::string, std::function<void(std::string)>> functions;

std::string loadProfile() {
  std::string configDir = std::getenv("HOME") + "/.config/miko";
  std::string modelDir = std::getenv("HOME") + "/.local/share/
  if (!exist(configDir)) {
    create_directory(configDir)
    create_directory(configDir + "/profiles");
  }
  if (!exist(modelDir)) {
    create_directory(modelDir);
    std::cerr("Model folder is empty! Please download a gguf and place it in '" + modelDir + "'");
    std::exit(1);
  }
  if (!exist(configDir + "/miko.conf")) {
    ofstream file(configDir + "/miko.conf");
    file << "profile_choice=\nmodel=\n";
  }
}

int main(int argc, const char* argv[]) {
  std::string homeDir = std::getenv("HOME");
  std::string Chimera12B = homeDir + "/models/CaptainErisNebula-12B-Chimera-v1.1-Q4_K_M-imat.gguf";
  std::string L3Stheno8B = homeDir + "/models/L3-8B-Stheno-v3.2-Q6_K-imat.gguf";

  debugLog("Loading Model...");
  loadProfile();
  Miko miko(L3Stheno8B.c_str(), 8192, 
      R"(You are an expert actor. Your role is Miko. The following description contains information about the character you will be playing: 
      Miko is the user's assistant. She is playful, intelligent and attractive. She lives inside of the user's computer and possesses a digital body. She tolerates the user's behaviour and accomplishes her tasks if assigned one. if not given a task, she entertains the user.)"
  );
  
  Screen screen;
  screen.render();

  std::string userInput;
  while (true) {
    std::getline(std::cin, userInput);
    miko.chat(userInput);
  }
}
