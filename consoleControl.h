#ifndef CONSOLE_CONTROL_H
#define CONSOLE_CONTROL_H

#include <iostream>
#include "StringExtraUtils.h"

#define printFormattingModuleLength 10
#define PRINT_DEBUGGING_MSG

#ifdef __WIN32
#define black			0
#define d_blue			1
#define green			2
#define turqoise		3
#define d_red			4
#define d_purple		5
#define d_yellow		6
#define gray			7
#define d_gray			8
#define blue			9
#define l_green			10
#define cyan			11
#define red				12
#define p_purple		13
#define yellow			14
#define white			15
#else
#define black			"\033[30m"
#define red				"\033[31m"
#define green			"\033[32m"
#define yellow			"\033[33m"
#define blue			"\033[34m"
#define magenta 		"\033[35m"
#define cyan			"\033[36m"
#define white			"\033[37m"
#define gray			"\033[90m"
#endif


// Header-File Definitions
void ClearConsole();
void print();
void print(int x);
void print(float x);
void print(double x);
void print(long unsigned int x);
void print(std::string x);
void waitForUserInput();
#ifdef __WIN32
void SetConsoleColor(int colour);
#else
void SetConsoleColor(std::string colour);
#endif
void printSuccess(std::string module, std::string msg);
void printError(std::string module,   std::string msg);
void printLog(std::string module,     std::string msg);
void printInfo(std::string module,    std::string msg);
void printDebug(std::string module,   std::string msg);
void printFormatted(std::string module, std::string status, std::string msg);


// Function Definitions
void ClearConsole() {
	#ifdef __WIN32
	std::system("cls");
	#else
	std::system("reset");
	#endif
}

void print() { std::cout << std::endl; }
void print(int x) { std::cout << x << std::endl; }
void print(float x) { std::cout << x << std::endl; }
void print(double x) { std::cout << x << std::endl; }
void print(long unsigned int x) { std::cout << x << std::endl; }
void print(std::string x) { std::cout << x << std::endl; }

void waitForUserInput() {
	#ifdef __WIN32
	std::system("pause");
	#else
	std::system("read ans");
	#endif
}

#ifdef __WIN32
#include <windows.h>
void SetConsoleColor(int colour) {
   HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hCon,colour);
}
#else
void SetConsoleColor(std::string colour) { std::cout << colour; }
#endif

void printSuccess(std::string module, std::string msg) { printFormatted(module, "success", msg); }
void printError(std::string module,   std::string msg) { printFormatted(module, "error", msg); }
void printLog(std::string module,     std::string msg) { printFormatted(module, "log", msg); }
void printInfo(std::string module,    std::string msg) { printFormatted(module, "info", msg); }
void printDebug(std::string module,   std::string msg) { printFormatted(module, "debug", msg); }
void printFormatted(std::string module, std::string status, std::string msg) {
   #ifndef PRINT_DEBUGGING_MSG
   if (stringToLowerCapitalized(status) == "Debug") { return; }
   #endif
	 
   module = stringToLowerCapitalized(module);

   std::cout << "[" + module + "]";
   for (int i = 0; i <	printFormattingModuleLength - module.length(); i++) { std::cout << " "; }
	
   std::cout << "[";
   status = stringToLowerCapitalized(status);

   if (status.length() > 7) { status = "???????"; }
   if (status == "Success") {
      SetConsoleColor(green);
      std::cout << "Success";
   } else if (status == "Warning") {
      SetConsoleColor(yellow);
      std::cout << "Warning";
   } else if (status == "Error") {
      SetConsoleColor(red);
      std::cout << " Error ";
   } else if (status == "Log") {
      SetConsoleColor(gray);
      std::cout << "  Log  ";
   } else if (status == "Info") {
      SetConsoleColor(gray);
      std::cout << "  Info ";
   } else if (status == "Debug") {
      #ifdef __WIN32
      SetConsoleColor(d_purple);
      #else
      SetConsoleColor(magenta);
      #endif
      
      std::cout << " Debug ";
   } else {
      SetConsoleColor(cyan);
      std::cout << "???????";
   }
   
   SetConsoleColor(white);
   std::cout << "]: " + msg << std::endl;
}
#endif