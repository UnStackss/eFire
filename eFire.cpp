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
#include <wininet.h>

namespace fs = std::filesystem;

std::string PASTEBIN_API_KEY = "";
const std::string PASTEBIN_API_URL = "https://pastebin.com/api/api_post.php";

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void enableUTF8Output() {
    SetConsoleOutputCP(CP_UTF8);
}

void setConsoleIcon(const std::string& iconFilePath) {
    HWND consoleWindow = GetConsoleWindow();
    HICON hIcon = (HICON)LoadImage(NULL, iconFilePath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    SendMessage(consoleWindow, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(consoleWindow, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

void displayLoadingBar() {
    const int width = 50;
    const int crimsonColor = 12;
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

    for (const auto& line : banner) {
        std::cout << line << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    setColor(15);
}

std::string getCurrentDateTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

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

std::string uploadToPastebin(const std::string& content) {
    if (PASTEBIN_API_KEY.empty()) {
        return "No Pastebin API key provided, skipping upload.";
    }

    HINTERNET hInternet, hConnect;
    DWORD bytesWritten;
    std::string response;

    hInternet = InternetOpen("DirectoryScanner", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        return "Error initializing WinINet.";
    }

    hConnect = InternetOpenUrlA(hInternet, PASTEBIN_API_URL.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        InternetCloseHandle(hInternet);
        return "Error connecting to Pastebin.";
    }

    std::string postData = "api_dev_key=" + PASTEBIN_API_KEY + 
                           "&api_option=paste" +
                           "&api_paste_code=" + content;
    
    const char* headers = "Content-Type: application/x-www-form-urlencoded\r\n";
    BOOL result = HttpSendRequestA(hConnect, headers, -1, (LPVOID)postData.c_str(), postData.length());
    
    if (result == 0) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "Error sending request.";
    }

    char buffer[512];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}

int main() {
    enableUTF8Output();
    setColor(12);

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
