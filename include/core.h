#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <llama.h>

// Helpers
void debugLog(std::string log);

//LlamaCPP Classes
class LLM {
  private:
    const char* model_path;
    llama_model_params model_params;
    llama_model * model;
    llama_context_params ctx_params;
    llama_context * ctx;
    const llama_vocab* vocab;
    int n_ctx;
    std::vector<llama_token> tokens;

  public:
    LLM(const char* model_path, int n_ctx);
    void inference(std::string prompt);
};

class Miko {
  private:
    LLM* llm;
  
  public:
    Miko(const char* llm_model_path, int n_ctx);
    ~Miko();
    void chat(std::string message);
};
