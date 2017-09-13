//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief No description available.
 */
//</editor-fold>
#ifndef SERVER_CLIENTDAO_HPP
#define SERVER_CLIENTDAO_HPP

#include <memory>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include "srv/entity/Client.hpp"

namespace solarplant
{
namespace srv
{
namespace dao
{
/**
 * @class ClientDAO
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 9/8/17.
 * 
 * @brief No description available.
 * 
 * # Description
 * No full length description available.
 */
class ClientDAO
{
    typedef int key_type;

    std::shared_ptr<sql::Connection> conn;
public:
    ClientDAO( const std::shared_ptr<sql::Connection> conn );

    entity::Client get( key_type id );

    void save( entity::Client client );
};
}
}
}


#endif //SERVER_CLIENTDAO_HPP
