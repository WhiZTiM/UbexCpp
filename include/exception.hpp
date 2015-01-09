#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <stdexcept>

namespace timl
{
    class value_exception : public std::logic_error
    {
    public:
        value_exception(const char* msg) noexcept
            : std::logic_error(msg) {}
    };

    class bad_value_cast : public value_exception
    {
    public:
        bad_value_cast(const char* msg)
            : value_exception(msg) {}
    };

    class parsing_exception : public value_exception
    {
    public:
        parsing_exception(const char* msg)
            : value_exception(msg) {}
    };

}
#endif // EXCEPTION_H
