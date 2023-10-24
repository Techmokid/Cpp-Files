#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdexcept>
#include <vector>
#include <string>
#include <cmath>

std::string stringToLower(std::string in) {
   std::string result = "";
   for (int i = 0; i < in.length(); i++) {
      result += std::tolower(in[i]);
   }
   return result;
}

std::string stringToUpper(std::string in) {
   std::string result = "";
   for (int i = 0; i < in.length(); i++) {
      result += std::toupper(in[i]);
   }
   return result;
}

std::string stringToLowerCapitalized(std::string in) {
   std::string result = stringToLower(in);
   result[0] = std::toupper(result[0]);
   return result;
}

std::string TimeFormatter(double x) {
	int digits = 2;
	int mult = std::pow(10,digits);
	
	if (x >= 7 * 24 * 60 * 60 * 1000) {
		return std::to_string(std::floor(x*mult/(7*1000*60*60*24))/mult) + " weeks";
	} else if (x >= 24 * 60 * 60 * 1000) {
		return std::to_string(std::floor(x*mult/(1000*60*60*24))/mult) + " days";
	} else if (x >= 60 * 60 * 1000) {
		return std::to_string(std::floor(x*mult/(1000*60*60))/mult) + " hours";
	} else if (x >= 60 * 1000) {
		return std::to_string(std::floor(x*mult/(1000*60))/mult) + " minutes";
	} else if (x >= 1000) {
		return std::to_string(std::floor(x*mult/1000)/mult) + " seconds";
	} else if (x >= 1) {
		return std::to_string(std::floor(x*mult)/mult) + " milliseconds";
	} else if (x >= 1/1000) {
		return std::to_string(std::floor(x*mult*1000)/mult) + " microseconds";
	} else if (x >= 1/(1000*1000)) {
		return std::to_string(std::floor(x*mult*1000*1000)/mult) + " nanoseconds";
	} else if (x >= 1/(1000*1000*1000)) {
		return std::to_string(std::floor(x*mult*1000*1000*1000)/mult) + " picoseconds";
	} else if (x <= 1/(1000*1000*1000)) {
		return "Time too small for formatting";
	} else {
		return "Time too large for formatting";
	}
}

std::string DataSizeFormatter(unsigned long long int x) {
	int digits = 2;
	int mult = std::pow(10,digits);
	unsigned long long int MB_CALC = 1000*1000;
	unsigned long long int GB_CALC = 1000*1000*1000;
	
	if (x < 0) {
		throw std::invalid_argument("Data Formatting Error: Cannot format a negative memory size? Was given: " + std::to_string(x));
	}
	
	if (x < 1000) {
		return std::to_string(x) + " B";
	} else if (x < MB_CALC) {
		return std::to_string(x/1000) + " KB";
	} else if (x < GB_CALC) {
		return std::to_string(x/MB_CALC) + " MB";
	} else if (x < GB_CALC*1000) {
		return std::to_string(x/GB_CALC) + " GB";
	} else {
		throw std::invalid_argument("Data Formatting Error: Data size too large. Size given: " + std::to_string(x/GB_CALC) + " GB");
	}
	
	return "";
}

std::vector<std::string> SplitString(std::string inputVal, char delimiter) {
	std::vector<std::string> result;
	result.push_back("");
	int x = 0;
	for (int i = 0; i < inputVal.length(); i++) {
		if (inputVal[i] == delimiter) {
			result.push_back("");
			x++;
		} else {
			result[x] += inputVal[i];
		}
	}
	return result;
}

std::string ASCII_To_Numeric(std::string x) {
	std::string result = "";
	for (int i = 0; i < x.length(); i++) {
		if ((x[i] >= '0' && x[i] <= '9') || (x[i] == '.'))
			result += x[i];
	}
	return result;
}

std::string RemoveSpecificCharacter(std::string x, char delim) {
	std::string result = "";
	for (int i = 0; i < x.length(); i++) {
		if (x[i] != delim)
			result += x[i];
	}
	return result;
}

#endif