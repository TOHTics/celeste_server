#ifndef CELESTE_AUTH_ERRORS_HPP
#define CELESTE_AUTH_ERRORS_HPP

#include <exception>

class AuthenticationError : public std::exception
{
public:
    AuthenticationError(const std::string& msg)
        : m_msg("authentication error: " + msg)
    {}

    virtual const char* what() const
    {
        return m_msg.c_str();
    }
private:
    std::string m_msg;
};

#endif