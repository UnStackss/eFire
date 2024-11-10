#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <algorithm>
#include <ctime>
#include <limits>
#include <wininet.h> // Include per WinINet

namespace fs = std::filesystem;

// Pastebin API configuration (only used if the user provides an API key)
std::string PASTEBIN_API_KEY = ""; // Empty by default, will be set by user input
const std::string PASTEBIN_API_URL = "https://pastebin.com/api/api_post.php";

// Set console color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Enable UTF-8 output
void enableUTF8Output() {
    SetConsoleOutputCP(CP_UTF8);
}

// Set console window icon (Windows only)
void setConsoleIcon(const std::string& iconFilePath) {
    HWND consoleWindow = GetConsoleWindow();
    HICON hIcon = (HICON)LoadImage(NULL, iconFilePath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    SendMessage(consoleWindow, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(consoleWindow, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

// Animated loading bar
void displayLoadingBar() {
    const int width = 50;
    const int crimsonColor = 12;  // Fiery red
    setColor(crimsonColor);

    std::cout << "\n🔥 Progress: ";
    for (int i = 0; i <= width; ++i) {
        std::cout << "\r[";

        for (int j = 0; j < width; ++j) {
            if (j < i) {
                std::cout << "█";
            } else {
                std::cout << " ";
            }
        }
        
        std::cout << "] " << (i * 100 / width) << "% 🔥";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(20 + i));
    }

    setColor(15);
    std::cout << "\n";
}

// Display fiery animated banner
void showAnimatedBanner() {
    std::string banner[] = {
        "🔥🔥🔥   _____       _____ _         🔥🔥🔥",
        "🔥🔥   |  ___|     |  ___| |        🔥🔥",
        "🔥🔥   | |_ __ ___ | |__ | |_ ___  🔥🔥",
        "🔥🔥   | | '__/ _ \\|  __|| __/ _ \\🔥🔥",
        "🔥🔥   | | | | (_) | |___| ||  __/🔥🔥",
        "🔥🔥   |_|_|  \\___/\\____/ \\__\\___|🔥🔥",
    };
    setColor(12);

    // Display banner with animation
    for (const auto& line : banner) {
        std::cout << line << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    setColor(15);
}

// Get current date and time as a string
std::string getCurrentDateTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

// Export directory tree in YAML format with icons
void writeDirectoryTreeToYAML(const fs::path& path, std::ofstream& outputFile, int depth) {
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string indentation(depth * 2, ' ');
            if (entry.is_directory()) {
                outputFile << indentation << "📂 " << entry.path().filename().string() << ":\n";
                writeDirectoryTreeToYAML(entry.path(), outputFile, depth + 1);
            } else {
                outputFile << indentation << "📄 - " << entry.path().filename().string() << "\n";
            }
        }
    } catch (const std::exception& e) {
        setColor(4);
        std::cerr << "🔥 Error accessing directory: " << e.what() << std::endl;
    }
}

// Upload file content to Pastebin
std::string uploadToPastebin(const std::string& content) {
    if (PASTEBIN_API_KEY.empty()) {
        return "No Pastebin API key provided, skipping upload.";
    }

    HINTERNET hInternet, hConnect;
    DWORD bytesWritten;
    std::string response;

    // Initialize WinINet
    hInternet = InternetOpen("DirectoryScanner", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); // Usato "DirectoryScanner" come stringa char*
    if (hInternet == NULL) {
        return "Error initializing WinINet.";
    }

    // Connect to Pastebin API
    hConnect = InternetOpenUrlA(hInternet, PASTEBIN_API_URL.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        InternetCloseHandle(hInternet);
        return "Error connecting to Pastebin.";
    }

    // Prepare data to send
    std::string postData = "api_dev_key=" + PASTEBIN_API_KEY + 
                           "&api_option=paste" +
                           "&api_paste_code=" + content;
    
    // Send POST request
    const char* headers = "Content-Type: application/x-www-form-urlencoded\r\n";
    BOOL result = HttpSendRequestA(hConnect, headers, -1, (LPVOID)postData.c_str(), postData.length());
    
    if (result == 0) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "Error sending request.";
    }

    // Read response from Pastebin
    char buffer[512];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}

// Main function
int main() {
    enableUTF8Output();
    setColor(12);

    // Prompt for Pastebin API key
    std::cout << "Enter your Pastebin API key (or press Enter to skip): ";
    std::getline(std::cin, PASTEBIN_API_KEY);

    if (PASTEBIN_API_KEY.empty()) {
        std::cout << "No Pastebin API key provided, skipping upload." << std::endl;
    }

    setConsoleIcon("icon.ico");

    showAnimatedBanner();
    setColor(12);
    std::cout << "🔥 Directory Scanner in Inferno Mode 🔥\n";

    while (true) {
        try {
            std::string path;
            std::cout << "\n💀 Enter the directory path (e.g., C:/Users/username/Documents): ";
            std::getline(std::cin, path);

            std::replace(path.begin(), path.end(), '\\', '/');

            if (!fs::exists(path)) {
                setColor(4);
                std::cerr << "💀 Error: Path does not exist." << std::endl;
                continue;
            }

            setColor(13);
            displayLoadingBar();

            std::string dateTime = getCurrentDateTime();
            std::string outputFileName = "tree_" + dateTime + ".yml";

            std::ofstream outputFile(outputFileName);
            if (!outputFile.is_open()) {
                setColor(4);
                std::cerr << "💀 Error: Unable to create output file." << std::endl;
                continue;
            }

            setColor(12);
            outputFile << path << ":\n";
            writeDirectoryTreeToYAML(path, outputFile, 1);
            outputFile.close();

            // Upload to Pastebin
            std::ifstream fileStream(outputFileName);
            std::string fileContent((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
            fileStream.close();

            std::string pastebinURL = uploadToPastebin(fileContent);
            
            if (pastebinURL.find("http") != std::string::npos) {
                setColor(12);
                std::cout << "🔥 Directory tree saved as " << fs::absolute(outputFileName) << " and uploaded to Pastebin: " << pastebinURL << " 🎉" << std::endl;
            } else {
                setColor(4);
                std::cerr << "💀 Failed to upload to Pastebin. Only saved locally." << std::endl;
            }

            setColor(15);
            char choice;
            setColor(12);
            std::cout << "\nDo you want to scan another directory? (y/n): ";
            setColor(15);
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice != 'y' && choice != 'Y') {
                break;
            }
        } catch (const std::exception& e) {
            setColor(4);
            std::cerr << "💀 Error: " << e.what() << std::endl;
        }
    }

    setColor(15);
    return 0;
}
