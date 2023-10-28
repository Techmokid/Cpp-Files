#ifndef OS_UTILS_H
#define OS_UTILS_H

#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include "ConsoleControl.h"

#ifdef _WIN32
std::string expandEnvironmentVariables(const std::string& inputPath) {
    char output[MAX_PATH];
    ExpandEnvironmentStringsA(inputPath.c_str(), output, MAX_PATH);
    return std::string(output);
}
#else
std::string expandEnvironmentVariables(const std::string& inputPath) {
    std::string output;
    size_t i = 0;
    while (i < inputPath.length()) {
        if (inputPath[i] == '$') {
            std::string var;
            i++;
            while (i < inputPath.length() && inputPath[i] != '/') {
                var += inputPath[i];
                i++;
            }
            const char* value = std::getenv(var.c_str());
            if (value == nullptr) {
                std::cerr << "Environment variable " << var << " is not set.\n";
                exit(EXIT_FAILURE);
            }
            output += value;
        }
        else {
            output += inputPath[i];
            i++;
        }
    }
    return output;
}
#endif

void quit() {
	print();
	print();
	print("Press enter to close the program");
	std::cin.get();
	exit(1);
}

void delay(unsigned int t) {
	std::chrono::system_clock::time_point waitUntil = std::chrono::system_clock::now() + std::chrono::seconds(t);
	std::this_thread::sleep_until(waitUntil);
}

void SetWallpaper(std::string path) {
	if(!std::filesystem::exists(path)) {
		std::cout << "Wallpaper path does not exist!";
		return;
	}
	
    // Convert std::string to a C-style string
    const char* cPath = path.c_str();

    // Use SystemParametersInfo to set the wallpaper
    int result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*) cPath, SPIF_UPDATEINIFILE);

    if (result) {
        std::cout << "Wallpaper set\n";
    } else {
        std::cout << "Wallpaper not set\n";
        std::cout << "SPI returned " << result << '\n';
    }
}

#endif