#include <core.h>

int main() {
  debugLog("Loading Model...");

  Miko miko("~/Third-Party/LLMs/repos/SmolLM2-1.7B-Instruct/GGUFs/SmolLM2.q8.gguf", 256);

  miko.chat("Greetings!");
}
