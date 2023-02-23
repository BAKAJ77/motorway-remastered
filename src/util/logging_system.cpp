#include <util/logging_system.h>
#include <util/formatted_exception.h>
#include <util/time.h>

#include <iostream>
#include <ctime>
#include <windows.h>

LoggingSystem::LoggingSystem()
{
#ifndef _DEBUG
	fopen_s(&m_logFileStream, "runtime_log.txt", "w"); // Open the logging file
#else
	m_consoleOut = GetStdHandle(STD_OUTPUT_HANDLE); // Get console output handle
#endif
}

LoggingSystem::~LoggingSystem()
{
#ifndef _DEBUG
	fclose(m_logFileStream);
	m_logFileStream = nullptr;
#endif
}

void LoggingSystem::Output(std::string_view str, Severity severity, va_list args)
{
#ifndef _DEBUG
	if (m_logFileStream)
	{
		// Output the log to the runtime logging file
		switch (severity)
		{
		case Severity::INFO:
			std::vfprintf(m_logFileStream, (Time::GetCurrentTimestamp() + " INFO -> " + str.data() + "\n").c_str(), args);
			break;
		case Severity::WARNING:
			std::vfprintf(m_logFileStream, (Time::GetCurrentTimestamp() + " WARNING -> " + str.data() + "\n").c_str(), args);
			break;
		case Severity::FATAL:
			std::vfprintf(m_logFileStream, (Time::GetCurrentTimestamp() + " ERROR -> " + str.data() + "\n").c_str(), args);
			break;
		};
	}
#else
	// Output the log to the console
	switch (severity)
	{
	case Severity::INFO:
		SetConsoleTextAttribute(m_consoleOut, 15);
		std::vprintf((Time::GetCurrentTimestamp() + " INFO -> " + str.data() + "\n").c_str(), args);
		break;
	case Severity::WARNING:
		SetConsoleTextAttribute(m_consoleOut, 14);
		std::vprintf((Time::GetCurrentTimestamp() + " WARNING -> " + str.data() + "\n").c_str(), args);
		break;
	case Severity::FATAL:
		SetConsoleTextAttribute(m_consoleOut, 12);
		std::vprintf((Time::GetCurrentTimestamp() + " ERROR -> " + str.data() + "\n").c_str(), args);
		break;
	};
#endif
}

void LoggingSystem::Output(std::string_view str, Severity severity, ...)
{
	va_list args;
	va_start(args, severity);

	this->Output(str, severity, args);

	va_end(args);
}

LoggingSystem& LoggingSystem::GetInstance()
{
	static LoggingSystem instance;
	return instance;
}
