#include "error.hpp"

namespace celeste
{
namespace resource
{
    // --- API ERROR DEFINITIONS -----
    APIError::APIError(const std::string& _description, int _code)
    : m_code(_code),
      m_description(_description)  
    {}

    const char* APIError::what() const noexcept
    {
        return ("API Error: " + m_description).c_str();
    }

    std::string APIError::description()
    {
        return m_description;
    }

    int APIError::code()
    {
        return m_code;
    }

    APIError::operator int()
    {
        return m_code;
    }
}
}

