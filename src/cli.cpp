#include <core.h>

int main() {
  debugLog("Loading Model...");
  Miko miko("models/SmolLM2.q8.gguf", 8192);
  
  std::string userInput;
  std::cin >> userInput;
  miko.chat(userInput);
}
