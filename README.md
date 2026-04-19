# 🤖 DevPilot: Your AI Command Line Co-Pilot

Ever found yourself staring at the terminal, completely blanking on the exact `find` or `tar` command you need? Me too. Instead of constantly tabbing out to Google or ChatGPT, I decided to build a tool that brings the AI directly into the terminal.

**DevPilot** is a lightning-fast, cross-platform CLI assistant written purely in **C++**. You type what you want to do in plain English, and it uses **Groq's Llama 3.1** to instantly generate the exact terminal command for your specific operating system.

![C++](https://img.shields.io/badge/C++-17-blue)
![Build](https://img.shields.io/badge/Build-CMake-success)
![LLM](https://img.shields.io/badge/Powered%20By-Groq%20%7C%20Llama%203.1-orange)
## See it in Action
<p align="center">
  <img src="devpilot linux terminal.png" width="600"/>
</p>

> **Me:** `./devpilot "find all pdf files modified in the last 7 days"`  
> **DevPilot:** `find . -name "*.pdf" -mtime -7`  
> **Execute this command? (y/n):** `y`  
> **Executing...** 

## Why I Built This (And What It Does)

I wanted an AI tool that actually respected my system's security and didn't hog system memory. Here’s what makes DevPilot special:

1. **Human-in-the-Loop Safety:** AI hallucinations are real. DevPilot *never* runs a command blindly. It always stops and asks for your `(y/n)` permission before touching your operating system.
2. **Lightning Fast (Groq API):** Integrated with Groq's Llama-3.1 endpoint, the response time is practically instant. 
3. **Audit History:** Every time you execute a command, DevPilot logs the timestamp, your prompt, and the executed command into a `devpilot_history.log` file using C++ File I/O.
4. **OS-Aware:** It uses C++ preprocessor macros (`#ifdef`) to figure out if you're on Windows, Mac, or Linux, so it doesn't accidentally hand you a Bash script when you need PowerShell.
5. **No Hardcoded Secrets:** It pulls the API key securely from your environment variables (`GROQ_API_KEY`).

##  What's Under the Hood?
* **Language:** C++17 (Because CLI tools should be fast and lightweight).
* **Build System:** CMake.
* **Network Requests:** `libcurl` for talking to the Groq API.
* **JSON Parsing:** `nlohmann/json` to handle the API payloads.
* **Process Management:** C++ `<cstdlib>` for safe OS-level execution.

### Developer Note: Why C++ instead of Python?
Most GenAI projects are written in Python, so why did I choose C++? 
Python is fantastic for *training* models, but for *deployment* and building CLI utilities, C++ is king. By writing this in C++, DevPilot compiles into a single, standalone binary. It doesn't force the user to install Python, set up virtual environments, or run `pip install`. Plus, manually managing memory during the cURL network requests was a great way to put core CS fundamentals into practice!

---

## How to Run It Yourself

### 1. Prerequisites
We'll need a C++ compiler, **CMake**, and **libcurl** installed on your machine.

### 2. Build the Project
Clone the repo, then run:
```bash
mkdir build
cd build
cmake ..
make
```

### 3. Add Your Groq API Key
Since DevPilot looks for an environment variable for security, you'll need to set it up in your terminal:
```bash
# On Linux / macOS
export GROQ_API_KEY="your_api_key_here"

# On Windows (Command Prompt)
set GROQ_API_KEY=your_api_key_here
```

### 4. Take it for a spin!
```bash
./devpilot "show me all active network ports"
```
