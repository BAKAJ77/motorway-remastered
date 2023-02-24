#ifndef FORMATTED_EXCEPTION_H
#define FORMATTED_EXCEPTION_H

#include <exception>
#include <string_view>

class FormattedException : public std::exception
{
private:
	va_list m_args;
public:
	FormattedException(std::string_view str, ...);
	~FormattedException();

	// Returns the additional arguments given to replace the format specifiers in the given exception string.
	va_list GetArgs();
};

#endif
