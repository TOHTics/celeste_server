/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#ifndef SERVER_RESOURCE_DEVICEOPS_HPP
#define SERVER_RESOURCE_DEVICEOPS_HPP

#include <string>
#include <iostream>
#include <memory>
#include <mysql_devapi.h>
#include <json.hpp>
#include <restbed>
#include <functional>

namespace celeste
{   
namespace resource
{
    void get_device(const std::shared_ptr<restbed::Session> session,
                    const std::shared_ptr<mysqlx::Session> dbSession);

    void save_device(const std::shared_ptr<restbed::Session> session,
                     const std::shared_ptr<mysqlx::Session> dbSession);
    
    void delete_device(const std::shared_ptr<restbed::Session> session,
                     const std::shared_ptr<mysqlx::Session> dbSession);

    void add_model(const std::shared_ptr<restbed::Session> session,
                           const std::shared_ptr<mysqlx::Session> dbSession);

    std::shared_ptr<restbed::Resource> make_device(const std::shared_ptr<mysqlx::Session> dbSession);
    std::shared_ptr<restbed::Resource> make_add_model(const std::shared_ptr<mysqlx::Session> dbSession);

}
}


#endif