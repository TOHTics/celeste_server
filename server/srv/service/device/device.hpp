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

using namespace std;

namespace celeste
{   
    namespace service
    {
        struct Device
        {
            int id;
            std::string client_id;
            std::string man;
            std::string mod;
            std::string sn;

            static void
            to_json(nlohmann::json& j, const Device& p);

            static void
            from_json(const nlohmann::json& j, Device& p);
        };

        class Devices
        {
            const std::shared_ptr<mysqlx::Session> dbSession;
            Devices(const std::shared_ptr<mysqlx::Session> session);

        private:
            nlohmann::json newDevice(const nlohmann::json& data);
            nlohmann::json deleteDevice();
            nlohmann::json updateDevice();

        public:
            void create(const Device& dev);
            void erase(int id);
            void update(const Device& dev);
        };

        std::shared_ptr<restbed::Resource> make_device_resource(std::string path);
    }

    namespace service
    {
        restbed::Service get_device_service();
    }
}

#endif