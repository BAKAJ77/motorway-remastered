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

	va_list GetArgs();
};

#endif
