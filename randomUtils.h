#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <cstdlib>
#include <ctime>


// Internal variables
namespace random_utils_internal {
	inline bool _initialized = false;
}


// Header-File Definitions
void CheckInit();
bool GetRandomBool();
float GetRandomFloat();
float GetRandomFloat(float max);
float GetRandomFloat(float min, float max);


// Function Definitions
void CheckInit() {
	if (!random_utils_internal::_initialized) {
		random_utils_internal::_initialized = true;
		srand(time(0));
	}
}
bool GetRandomBool() { CheckInit(); return rand() > (RAND_MAX / 2); }
float GetRandomFloat() { return GetRandomFloat(0,1); }
float GetRandomFloat(float max) { return GetRandomFloat(0, max); }
float GetRandomFloat(float min, float max) {
	CheckInit();
	return min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(max-min)));
}

#endif