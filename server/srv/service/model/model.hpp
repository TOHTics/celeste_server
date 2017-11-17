/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#ifndef SERVER_RESOURCE_MODEL_HPP
#define SERVER_RESOURCE_MODEL_HPP

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
    void get_model(const std::shared_ptr<restbed::Session> session,
                   const std::shared_ptr<mysqlx::Session> dbSession);
    
    void save_model(const std::shared_ptr<restbed::Session> session,
                    const std::shared_ptr<mysqlx::Session> dbSession);
    
    void delete_model(const std::shared_ptr<restbed::Session> session,
                     const std::shared_ptr<mysqlx::Session> dbSession);

    std::shared_ptr<restbed::Resource> make_model(const std::shared_ptr<mysqlx::Session> dbSession);
}
}


#endif