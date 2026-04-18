#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <ctime>

using json = nlohmann::json;
using namespace std;

// ---------------------------------------------------------
// PART 1: The Callback Function for cURL
// WHY: cURL downloads data in chunks. We need a function to 
// append those chunks into a single C++ string.
// ---------------------------------------------------------
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ---------------------------------------------------------
// PART 2: OS Detection
// WHY: The AI needs to know if it should generate a Windows 
// PowerShell command or a Linux Bash command.
// ---------------------------------------------------------
string getOS() {
    #ifdef _WIN32
        return "Windows";
    #elif __APPLE__
        return "macOS";
    #elif __linux__
        return "Linux";
    #else
        return "Unknown OS";
    #endif
}

// ---------------------------------------------------------
// PART 3: The Core Function
// ---------------------------------------------------------
int main(int argc, char* argv[]) {
    // 1. Validate Input
    if (argc < 2) {
        cerr << "Usage: devpilot \"<your request>\"" << endl;
        return 1;
    }

    // Combine arguments into one prompt string
    string user_prompt = "";
    for (int i = 1; i < argc; ++i) {
        user_prompt += argv[i];
        user_prompt += " ";
    }

    // 2. Security Check: Read API Key from Environment Variable
    // WHY: Hardcoding API keys in code is a massive red flag in interviews.
    const char* api_key = getenv("GROQ_API_KEY");
    if (!api_key) {
        cerr << "Error: GROQ_API_KEY environment variable not set." << endl;
        return 1;
    }

    // 3. Construct the System Prompt
    // WHY: We must force the AI to return ONLY the code, without formatting like ```bash
    string os_name = getOS();
    string system_instruction = "You are a command line assistant. The user is on " + os_name + 
        ". Provide ONLY the raw terminal command to fulfill their request. "
        "Do not use markdown, do not use backticks, no explanations.";

    // 4. Create the JSON Payload (Using nlohmann/json)
    json payload = {
        {"model", "llama-3.1-8b-instant"}, // or a Groq/Gemini model
        {"messages", {
            {{"role", "system"}, {"content", system_instruction}},
            {{"role", "user"}, {"content", user_prompt}}
        }},
        {"temperature", 0.0} // 0.0 ensures factual, deterministic output
    };
    string json_string = payload.dump();

    // 5. Initialize cURL and send HTTP POST
    CURL* curl = curl_easy_init();
    string readBuffer; // This will hold the AI's response

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        string auth_header = string("Authorization: Bearer ") + api_key;
        headers = curl_slist_append(headers, auth_header.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.groq.com/openai/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "Network Error: " << curl_easy_strerror(res) << endl;
            return 1;
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    // 6. Parse the JSON Response
    try {
        json response = json::parse(readBuffer);
        
        if (response.contains("error")) {
            cerr << "API Error: " << response["error"]["message"] << endl;
            return 1;
        }

        string generated_command = response["choices"][0]["message"]["content"];

        // 7. Safety & Execution
        // WHY: Executing AI code blindly can destroy your system. Always ask!
        cout << "\n🤖 DevPilot suggests: \033[1;32m" << generated_command << "\033[0m\n";
        cout << "Execute this command? (y/n): ";
        
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
        cout << "Executing...\n";

        // Execute command
        int result = system(generated_command.c_str()); 
        if (result != 0) cout << "Command exited with code: " << result << endl;

        // ---------------- LOGGING FEATURE ----------------
        ofstream logFile("devpilot_history.log", ios::app);

        if (logFile.is_open()) {
            // Get current time
            time_t now = time(0);
            char* dt = ctime(&now);

            logFile << "---------------------------------\n";
            logFile << "Time: " << dt;
            logFile << "User Prompt: " << user_prompt << "\n";
            logFile << "Executed Command: " << generated_command << "\n";
            logFile << "---------------------------------\n\n";

            logFile.close();
        }
        }
    else {
                cout << "Operation aborted." << endl;
            }

    } catch (json::parse_error& e) {
        cerr << "JSON Parsing error: " << e.what() << endl;
    }

    return 0;
}