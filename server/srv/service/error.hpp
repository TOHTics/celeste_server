#ifndef CELESTE_SERVICE_ERROR_HPP
#define CELESTE_SERVICE_ERROR_HPP

#include <exception>
#include <string>
#include <sstream>
#include "common.hpp"

namespace celeste
{
namespace resource
{
    class APIError : std::exception
    {
        std::string m_description;
        int m_code;

    public:
        APIError(const std::string& _description, int _code);

        virtual const char* what() const noexcept;

        std::string description();
        int code();
        operator int();
    };
}
}
#endif