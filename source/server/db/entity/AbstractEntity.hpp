/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 20/10/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_ABSTRACT_ENTITY_HPP
#define SERVER_DB_ABSTRACT_ENTITY_HPP

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace solarplant
{
namespace db
{
namespace entity
{

template <
    typename KeyType
> struct Entity
{
    typedef KeyType key_type;

    KeyType id;

    Entity() = default;
    Entity (KeyType _id) : id(_id)
    {}
};
}
}
}
#endif //SERVER_CLIENT_HPP