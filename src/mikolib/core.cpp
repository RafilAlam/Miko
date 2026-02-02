#include <core.hpp>

// Helpers
void debugLog(std::string log) {
  std::cout << "MikoCPP | " << log << std::endl;
}

// LlamaCPP
LLM::LLM(const char* model_path, int n_ctx) 
  : model_path(model_path),  
  model_params(llama_model_default_params()),
  n_ctx(n_ctx),
  formatted(n_ctx),
  prev_len(0)
{
  ggml_backend_load_all();

  this->model_params.n_gpu_layers = 99;
  this->model = llama_model_load_from_file(model_path, this->model_params);
  this->vocab = llama_model_get_vocab(this->model);

  this->ctx_params = llama_context_default_params();
  this->ctx_params.n_ctx = n_ctx;
  this->ctx_params.n_batch = n_ctx;
  this->ctx = llama_init_from_model(this->model, this->ctx_params);

  this->sampler = llama_sampler_chain_init(llama_sampler_chain_default_params());
  llama_sampler_chain_add(this->sampler, llama_sampler_init_top_p(0.9f, 1));
  llama_sampler_chain_add(this->sampler, llama_sampler_init_temp(0.2f));
  llama_sampler_chain_add(this->sampler, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

  this->tmpl = llama_model_chat_template(model, nullptr);
}
LLM::~LLM() {
  llama_sampler_free(this->sampler);
  llama_free(this->ctx);
  llama_model_free(this->model);
}
    
std::string LLM::generate(std::string prompt) {
  std::string response;

  const bool is_first = llama_memory_seq_pos_max(llama_get_memory(this->ctx), 0) == -1;

  // tokenize the prompt
  const int n_prompt_tokens = -llama_tokenize(this->vocab, prompt.c_str(), prompt.size(), NULL, 0, is_first, true);
  std::vector<llama_token> prompt_tokens(n_prompt_tokens);
  if (llama_tokenize(this->vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), is_first, true) < 0) {
    GGML_ABORT("failed to tokenize the prompt\n");
  }

  // prepare a batch for the prompt
  llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
  llama_token new_token_id;
  while (true) {
    // check if we have enough space in the context to evaluate this batch
    int n_ctx = llama_n_ctx(this->ctx);
    int n_ctx_used = llama_memory_seq_pos_max(llama_get_memory(this->ctx), 0) + 1;
    if (n_ctx_used + batch.n_tokens > n_ctx) {
      printf("\033[0m\n");
      fprintf(stderr, "context size exceeded\n");
      exit(0);
    }

    int ret = llama_decode(this->ctx, batch);
    if (ret != 0) {
      GGML_ABORT("failed to decode, ret = %d\n", ret);
    }

    // sample the next token
    new_token_id = llama_sampler_sample(this->sampler, this->ctx, -1);

    // is it an end of generation?
    if (llama_vocab_is_eog(this->vocab, new_token_id)) {
      break;
    }

    // convert the token to a string, print it and add it to the response
    char buf[256];
    int n = llama_token_to_piece(this->vocab, new_token_id, buf, sizeof(buf), 0, true);
    if (n < 0) {
      GGML_ABORT("failed to convert token to piece\n");
    }
    std::string piece(buf, n);
    printf("%s", piece.c_str());
    fflush(stdout);
    response += piece;

    // prepare the next batch with the sampled token
    batch = llama_batch_get_one(&new_token_id, 1);
  }

  return response;
}

int LLM::system(std::string systemPrompt) {
  if (systemPrompt.empty()) {
    return 0;
  }

  this->messages.push_back({"system", strdup(systemPrompt.c_str())});
  int new_len = llama_chat_apply_template(this->tmpl, this->messages.data(), this->messages.size(), true, this->formatted.data(), this->formatted.size());
  if(new_len > (int)this->formatted.size()) {
    this->formatted.resize(new_len);
    new_len = llama_chat_apply_template(this->tmpl, this->messages.data(), this->messages.size(), true, this->formatted.data(), this->formatted.size());
  }
  if (new_len < 0) {
    fprintf(stderr, "failed to apply the chat template on system prompt\n");
  }

  return 0;
}

int LLM::inference(std::string userInput) {
  if (userInput.empty()) {
    return 0;
  }

  // add the user input to the message list and format it
  this->messages.push_back({"user", strdup(userInput.c_str())});
  int new_len = llama_chat_apply_template(this->tmpl, this->messages.data(), this->messages.size(), true, this->formatted.data(), this->formatted.size());
  if (new_len > (int)this->formatted.size()) {
    this->formatted.resize(new_len);
    new_len = llama_chat_apply_template(this->tmpl, this->messages.data(), this->messages.size(), true, this->formatted.data(), this->formatted.size());
  }
  if (new_len < 0) {
    fprintf(stderr, "failed to apply the chat template on user input\n");
    return 1;
  }

  // remove previous messages to obtain the prompt to generate the response
  std::string prompt(this->formatted.begin() + this->prev_len, this->formatted.begin() + new_len);

  // generate a response
  printf("\033[33m");
  std::string response = this->generate(prompt);
  printf("\n\033[0m");

  // add the response to the messages
  this->messages.push_back({"assistant", strdup(response.c_str())});
  this->prev_len = llama_chat_apply_template(this->tmpl, this->messages.data(), this->messages.size(), false, nullptr, 0);
  if (this->prev_len < 0) {
    fprintf(stderr, "failed to apply the chat template on assistant response\n");
    return 1;
  }

  return 0;
}

Miko::Miko(const char* llm_model_path, int n_ctx, std::string systemPrompt) : llm(new LLM(llm_model_path, n_ctx)) {
  this->llm->system(systemPrompt);
}
Miko::~Miko() {
  delete this->llm;
}

void Miko::chat(std::string message) {
  this->llm->inference(message);
}


