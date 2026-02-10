#include <core.hpp>
#include <ui.hpp>
#include <filetools.hpp>

#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <filesystem>

std::map<std::string, std::function<void(std::vector<std::string>)>> functions;

const std::string configDir = std::getenv("HOME") + std::string("/.config/miko");
const std::string modelDir = std::getenv("HOME") + std::string("/.local/share/miko/");

void loadProfile(std::string& modelpath, std::string& systemprompt) {
  configmap map = file_to_map(configDir + "/models.conf");
  modelpath = map[map["model_choice"]];
  std::cout << map["model_choice"] << std::endl;
  if (modelpath.empty()) {
    std::cerr << "Invalid model choice!\n";
    std::exit(1);
  }
  modelpath = modelDir + modelpath;

  map = file_to_map(configDir + "/profiles.conf");
  std::string data;
  std::ifstream file(configDir + "/profiles/" + map[map["profile_choice"]]);
  while (std::getline(file, data)) {
    systemprompt += data;
  }

  if (systemprompt.empty()) {
    debugLog("WARNING: Running model without a system prompt!");
  }
}

void init() {
  if (!std::filesystem::exists(configDir)) {
    std::filesystem::create_directory(configDir);
    std::filesystem::create_directory(configDir + "/profiles");
  }
  if (!std::filesystem::exists(modelDir)) {
    std::filesystem::create_directory(modelDir);
    std::cerr << "Model folder is empty! Please download a gguf and place it in '" << modelDir << "'\n";
    std::exit(1);
  }
  if (!std::filesystem::exists(configDir + "/models.conf")) {
    std::ofstream file(configDir + "/models.conf");
    file << "model_choice=\n";
    file.close();
  }
  if (!std::filesystem::exists(configDir + "/profiles.conf")) {
    std::ofstream file(configDir + "/profiles.conf");
    file << "profile_choice=\n";
    file.close();
  }

  functions[""] = [](std::vector<std::string> args) {
    debugLog("Your personal desktop AI Assistant.");
    std::cout
    << "\nmiko add -m model_name my_model.gguf    - register a local model file\n"
    << "miko add -p profile_name my_profile.txt - register a profile card file\n"
    << "miko cd -m model_name                   - swap to a registered model\n"
    << "miko cd -p profile_name                 - swap to a registered profile\n"
    << "miko ls -m                              - list registered models\n"
    << "miko ls -p                              - list registered profiles\n"
    << "miko wake                               - wake up & chat\n";
  };

  functions["cd-m"] = [](std::vector<std::string> args) {
    configmap map = file_to_map(configDir + "/models.conf");
    map["model_choice"] = args[3];
    map_to_file(map, configDir + "/models.conf");
  };

  functions["cd-p"] = [](std::vector<std::string> args) {
    configmap map = file_to_map(configDir + "/profiles.conf");
    map["profile_choice"] = args[3];
    map_to_file(map, configDir + "/profiles.conf");
  };

  functions["wake"] = [](std::vector<std::string> args) {
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
  };

  functions["add"] = [](std::vector<std::string> args) {
    std::cout << "Invalid usage of add.\n add -m <file_name.gguf> - Adds a model to registry.\n add -p <file_name> - Adds a profile to registry.\n";
    std::exit(1);
  };

  functions["add-m"] = [](std::vector<std::string> args) {
    if (args.size()<=3) {
      functions["add"](args);
    }
    configmap map = file_to_map(configDir + "/models.conf");
    map[args[3]] = args[4];
    map_to_file(map, configDir + "/models.conf");
  };

  functions["add-p"] = [](std::vector<std::string> args) {
    if (args.size()<=3) {
      functions["add"](args);
    }
    if (std::string(std::getenv("EDITOR")).empty()) {
      debugLog("ERROR: Global Env Variable 'EDITOR' is not set, please set it to your preferred text editor.");
      std::exit(1);
    }
    configmap map = file_to_map(configDir + "/profiles.conf");
    map[args[3]] = args[4];
    std::system(("$EDITOR " + configDir + "/profiles/" + args[4]).c_str());
    map_to_file(map, configDir + "/profiles.conf");
  };

  functions["ls"] = [](std::vector<std::string> args) {
    std::cout << "Invalid usage of ls.\n ls -m - Shows list of models\n ls -p - Shows list of profiles\n";
  };

  functions["ls-m"] = [](std::vector<std::string> args) {
    configmap map = file_to_map(configDir + "/models.conf");
    for (const auto& pair : map) {
      std::cout << pair.first << " - " << pair.second << std::endl;
    }
  };

  functions["ls-p"] = [](std::vector<std::string> args) {
    configmap map = file_to_map(configDir + "/profiles.conf");
    for (const auto& pair : map) {
      std::cout << pair.first << " - " << pair.second << std::endl;
    }
  };
}

int main(int argc, const char* argv[]) {
  init();
  std::vector<std::string> args(argv, argv+argc);
  if (argc==1) {
    functions[""](args);
    return 0;
  }
  else {
    std::string function = args[1];
    if (argc>2) function += args[2];
    functions[function](args);
    return 0;
  }
}
