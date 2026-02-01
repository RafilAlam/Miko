#include <core.h>

// Helpers
void debugLog(std::string log) {
  std::cout << "MikoCPP | " << log << std::endl;
}

// LlamaCPP
LLM::LLM(const char* model_path, int n_ctx) 
  : model_path(model_path),  
  model_params(llama_model_default_params()),
  model(llama_model_load_from_file(model_path, model_params)),
  ctx_params(llama_context_default_params()),
  ctx(llama_init_from_model(model, ctx_params)),
  vocab(llama_model_get_vocab(model)),
  n_ctx(n_ctx),
  tokens(n_ctx)
{}
    
void LLM::inference(std::string prompt) {
  int n_tokens = llama_tokenize(this->vocab, prompt.c_str(), prompt.length(), this->tokens.data(), this->tokens.size(), true, true);

  llama_batch batch = llama_batch_get_one(tokens.data(), n_tokens);
  llama_decode(this->ctx, batch);

  llama_token next = llama_sampler_sample(llama_sampler_chain_init(llama_sampler_chain_default_params()), this->ctx, -1);
  char buf[this->n_ctx];
  int n = llama_token_to_piece(this->vocab, next, buf, sizeof(buf), 0, true);
  std::cout << std::string(buf, n) << std::endl;

  llama_free(this->ctx);
  llama_model_free(this->model);
}

Miko::Miko(const char* llm_model_path, int n_ctx) : llm(new LLM(llm_model_path, n_ctx)) {}
Miko::~Miko() {
  delete llm;
}

void Miko::chat(std::string message) {
  this->llm->inference(message);
}


