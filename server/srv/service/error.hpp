#ifndef CELESTE_SERVICE_ERROR_HPP
#define CELESTE_SERVICE_ERROR_HPP

#include <exception>
#include <string>
#include <sstream>


class APIError : std::exception
{
    std::string m_description;
    int m_code;

public:

    APIError(const std::string& _description, int _code)
        : m_code(_code),
          m_description(_description)  
    {}

    virtual const char* what() const noexcept
    {
        return m_description.c_str();
    }

    std::string description()
    {
        return m_description;
    }

    int code()
    {
        return m_code;
    }
};


class MissingAttributeError : public APIError
{
    std::string attr_name;

public:
    MissingAttributeError(const std::string& attr_name)
        : APIError(
                   "Missing attribute: " + attr_name,
                   400
                   )
    {}
};


class GetError : public APIError
{
public:
    GetError(const std::string& obj_name, const std::string& id = "")
        : APIError(
                   "Could not get " + obj_name + " " + id,
                   500
                   )
    {}
};

class InsertError : public APIError
{

};

class RemoveError : public APIError
{

};




#endif