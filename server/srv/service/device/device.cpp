/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <iostream>
#include "srv/db/db.hpp"
#include "device.hpp"

using namespace std;

using json = nlohmann::json;

namespace celeste
{
namespace service
{
    json newDevice(const nlohmann::json& data, const std::shared_ptr<mysqlx::Session> dbSession)
    {
        auto sch = dbSession->getSchema("Celeste");
        auto table = sch.getTable("Device");

        if (data["autogen"].get<bool>())
        {
            int max_id = table.select("MAX(id)").execute().fetchOne().get(0);

            table.insert("id", "Client_id", "man", "mod", "sn").
            values(++max_id, data["client_id"], data["man"], data["mod"], data["sn"]).execute();
            
            return json(max_id);
        } else 
        {
            table.insert("id", "Client_id", "man", "mod", "sn").
            values(data["id"], data["client_id"], data["man"], data["mod"], data["sn"]).execute();
            return data["id"];
        }
    }
}
}