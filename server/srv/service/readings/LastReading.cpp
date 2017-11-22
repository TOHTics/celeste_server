/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 3/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#include <string>
#include <mysql_devapi.h>
#include <functional>

#include "srv/service/common.hpp"
#include "srv/db/db.hpp"
#include "readings.hpp"

using namespace std;

using json = nlohmann::json;

namespace celeste
{
namespace resource
{
    LastReading::LastReading(const shared_ptr<mysqlx::Session> _dbSession)
        : dbSession(_dbSession)
    {}


    vector<string>
    LastReading::required_fields() const
    {
        return {"PointId", "ModelId", "DeviceId"};
    }


    json
    LastReading::get(json&& j) const
    {
        return get(forward<json>(j));
    }


    json
    LastReading::get(const json& j) const
    {
        return get(j["PointId"], j["ModelId"], j["DeviceId"]);
    }


    json
    LastReading::get(const string& point_id,
                          const string& model_id,
                          int device_id) const
    {
        // Get contents
        auto res = dbSession->sql(
                                    "SELECT a.sf, a.t, a.data FROM PointRecord a"
                                    " INNER JOIN ("
                                        "SELECT "
                                            "Point_id,"
                                            "Model_id,"
                                            "Device_id,"
                                            "Model_idx,"
                                            "max(DeviceRecord_idx) DeviceRecord_idx,"
                                            "max(idx) idx "
                                        " FROM PointRecord"
                                        " WHERE Point_id    = ?"   // Bind 
                                        " AND Model_id      = ?"   // .
                                        " AND Device_id     = ?"   // .
                                        " GROUP BY Point_id, Model_id, Device_id, Model_idx) b"
                                        " ON a.Point_id = b.Point_id AND "
                                        " a.Model_id = b.Model_id AND "
                                        " a.Device_id = b.Device_id AND "
                                        " a.Model_idx = b.Model_idx AND "
                                        " a.idx = b.idx AND "
                                        " a.DeviceRecord_idx = b.DeviceRecord_idx;"
                                    ).bind(ValueList{
                                        point_id.c_str(),
                                        model_id.c_str(),
                                        device_id
                                    }).execute();
        json j = move(res);

        // Add value converted to type
        int type = dbSession->sql(
                                    "SELECT a.type FROM Point a "
                                    "WHERE a.id = ? AND a.Model_id = ?;"
                                    ).bind(ValueList{
                                        point_id.c_str(),
                                        model_id.c_str()
                                    }).execute().fetchOne().get(0);

        for (auto it = j.begin(); it != j.end(); it++)
        {
            auto data = it->at("data").get<string>();
            switch(type)
            {
                case PointType::INT:
                    (*it)["value"] = stoi(data);
                    break;
                case PointType::DOUBLE:
                    (*it)["value"] = stod(data);
                    break;
                case PointType::FLOAT:
                    (*it)["value"] = stof(data);
                    break;
                case PointType::STRING:
                    (*it)["value"] = data;
                    break;
                default:
                    throw invalid_argument("Critical error. PointType is not set properly in the Celeste DB."
                                           "Please forward this error to carlos.brito524@gmail.com");
            }
        }
        return j;
    }
}
}
