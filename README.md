# 🤖 DevPilot - C++ GenAI Command Line Assistant

DevPilot is a high-performance, cross-platform CLI tool built entirely in **C++**. It leverages Generative AI (LLMs) to translate natural language into terminal commands, acting as an intelligent co-pilot for your operating system.

![C++](https://img.shields.io/badge/C++-17-blue)
![Build](https://img.shields.io/badge/Build-CMake-success)
![License](https://img.shields.io/badge/License-MIT-green)

## Demo
**
`User:` ./devpilot "find all pdf files modified in the last 7 days"
`DevPilot:` find . -name "*.pdf" -mtime -7
`Execute this command? (y/n):` y

<p align="center">
  <img src="" width="600"/>
</p>

## Key Features
- **Cross-Platform:** Uses C++ preprocessor directives to auto-detect Windows, macOS, or Linux.
- **Human-in-the-Loop Safety:** Never executes code blindly; requires explicit user permission via `std::cin` to prevent destructive OS operations.
- **Audit Logging:** Implements C++ File I/O (`std::ofstream`) to maintain a persistent history of all executed commands.
- **Secure Secret Management:** Reads API keys securely from host environment variables, never hardcoded.
- **Memory Safe:** Strict C-level memory management during `libcurl` network requests to guarantee zero memory leaks.

## Tech Stack & Libraries
* **Language:** C++ (Core CS Fundamentals, OOPs, Memory Management)
* **Build System:** CMake (Enterprise build standards)
* **Networking:** `libcurl` (HTTP REST API integration)
* **Data Serialization:** `nlohmann/json` (JSON parsing)
* **OS Interfacing:** `<cstdlib>` `system()` calls and Environment Variables.


## How to Build and Run

### 1. Prerequisites
C++ compiler, `CMake`, and `libcurl` installed in the system.

### 2. Build the Project
```bash
mkdir build
cd build
cmake ..
make
