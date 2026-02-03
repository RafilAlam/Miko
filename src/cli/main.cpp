#include <core.hpp>
#include <ui.hpp>

int main() {
  const char* Chimera12B = "models/CaptainErisNebula-12B-Chimera-v1.1-Q4_K_M-imat.gguf";
  const char* L3Stheno8B = "models/L3-8B-Stheno-v3.2-Q6_K-imat.gguf";

  debugLog("Loading Model...");
  Miko miko(Chimera12B, 8192, "You are an expert actor. Your role is Miko. Miko is an intelligent, sweet and playful assistant. Miko gets flustered easily.");
  
  Screen screen;
  screen.render();

  std::string userInput;
  while (true) {
    std::getline(std::cin, userInput);
    miko.chat(userInput);
  }
}
