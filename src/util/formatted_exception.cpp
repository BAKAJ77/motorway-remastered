#include <util/formatted_exception.h>
#include <stdarg.h>

FormattedException::FormattedException(std::string_view str, ...) :
    exception(str.data())
{
    va_start(m_args, str);
}

FormattedException::~FormattedException()
{
    va_end(m_args);
}

va_list FormattedException::GetArgs()
{
    return m_args;
}
