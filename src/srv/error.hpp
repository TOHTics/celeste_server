#ifndef CELESTE_SERVER_ERRORS_HPP
#define CELESTE_SERVER_ERRORS_HPP

#include <exception>
#include <string>

namespace celeste
{
    struct BadRequest : public std::runtime_error
    {
        BadRequest(const std::string& msg)
            : std::runtime_error("bad request: " + msg)
        {}
    };

    struct AuthError : public std::runtime_error
    {
        AuthError(const std::string& msg)
            : std::runtime_error("auth error: " + msg)
        {}
    };

    struct MissingFieldError : public BadRequest
    {
        MissingFieldError(const std::string& field_name)
            : celeste::BadRequest("Missing field: " + field_name)
        {}
    };

    struct DatabaseError : public std::runtime_error
    {
    public:
        DatabaseError(const std::string& msg)
            : std::runtime_error("database error: " + msg)
        {}
    };

    struct XMLError : public std::runtime_error
    {
        XMLError(const std::string& msg)
            : std::runtime_error("xml syntax error: " + msg)
        {}
    };
}
#endif