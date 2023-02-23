#ifndef TIME_H
#define TIME_H

#include <string>

namespace Time
{
	// Returns the current date and time as a string.
	extern std::string GetCurrentTimestamp();

	// Returns the amount of seconds passed since startup of the application.
	extern float GetSecondsSinceEpoch();
}

#endif
