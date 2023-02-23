#include <util/time.h>

#include <GLFW/glfw3.h>
#include <ctime>

std::string Time::GetCurrentTimestamp()
{
	time_t rawTime;
	struct tm timeData;

	time(&rawTime);
	localtime_s(&timeData, &rawTime);
	char* timeInfo = new char[50];
	asctime_s(timeInfo, 50, &timeData);

	std::string currentTimeStr = timeInfo;
	currentTimeStr.pop_back(); // Remove the '\n' from the end of the string
	delete[] timeInfo;

	return "[" + currentTimeStr + "]";
}

float Time::GetSecondsSinceEpoch()
{
    return (float)glfwGetTime();
}
