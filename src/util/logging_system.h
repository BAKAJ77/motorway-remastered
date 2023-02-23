#ifndef LOGGING_SYSTEM_H
#define LOGGING_SYSTEM_H

#include <string>

typedef void* HANDLE;

class LoggingSystem
{
private:
#ifndef _DEBUG
	FILE* m_logFileStream = nullptr;
#else
	HANDLE m_consoleOut;
#endif

	LoggingSystem();
public:
	enum class Severity { INFO, WARNING, FATAL };

	~LoggingSystem();

	// Writes the given string to a logging output.
	// This will be the console if in Debug mode, or a log file if in Release mode.
	// Note that this function follows the same formatting rules as the printf() functions.
	void Output(std::string_view str, Severity severity, ...);
	void Output(std::string_view str, Severity severity, va_list args);

	// Returns singleton instance of the class.
	static LoggingSystem& GetInstance();
};

#endif
