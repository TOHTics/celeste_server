#ifndef CELESTE_SERVER_ERRORS_HPP
#define CELESTE_SERVER_ERRORS_HPP

#include <exception>
#include <string>
#include <stdexcept>

namespace celeste
{
    struct BadRequest : public std::runtime_error
    {
        BadRequest(const std::string& msg)
            : std::runtime_error("bad request: " + msg)
        {}
    };

    struct MissingHeaderError : public BadRequest
    {
        MissingHeaderError(const std::string& header_name)
            : celeste::BadRequest("missing header: " + header_name)
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
            : celeste::BadRequest("missing field: " + field_name)
        {}
    };

    struct BadFieldError : public BadRequest
    {
        BadFieldError(const std::string& msg, const std::string& field)
            : celeste::BadRequest("bad field: \'" + field + "\': " + msg)
        {}

        BadFieldError(const std::string& msg)
            : celeste::BadRequest("bad field: " + msg)
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
