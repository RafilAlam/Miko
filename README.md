# Miko
A personal desktop AI Assistant. The improved successor to [Hermes](https://github.com/RafilAlam/Hermes-the-AI-Assistant).

## Aim
Providing an `open-source` Interactive Assistant that you can be *sure of* isn't stealing your data behind your back.

## Features Roadmap ([x] - Usable, 🚧 - Currently being worked on)
- [x] Local LLM
- [ ] Local TTS
- [ ] Model Tool-Calling
- [ ] Memory Tools (smart context window handling)
- [x] Terminal Interface (🚧)
- [ ] Graphical User Interface
- [ ] Local Server Inference
- [ ] Model Hot-Swapping (Multi-Modal Support)

## CLI Usage:
```bash
# Register a local model file placed inside '$HOME/.local/share/miko'. Here 'model_name' is what you use to identify and call the model, 'my_model.gguf' is the file's name.
miko add -m model_name my_model.gguf

# Register/modify a model profile card. Here 'profile_name' is what you use to identify and call the profile, 'my_profile.txt' is the file's name.
miko add -p profile_name my_profile.txt

# Switch current model or profile respectively. Here 'model_name' and 'profile_name' is the identifier assigned to the model/profile card.
miko cd -m model_name   # Switch to model_name
miko cd -p profile_name # Switch to profile_name

# List currently used model/profile card and registered models/profile cards respectively.
miko ls -m # List models
miko ls -p # List profile cards

# Wake the chatbot and chat with it.
miko wake
```
