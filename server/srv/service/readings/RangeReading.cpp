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
    RangeReading::RangeReading(const shared_ptr<mysqlx::Session> _dbSession)
        : dbSession(_dbSession)
    {}

    vector<string>
    RangeReading::required_fields() const
    {
        return {"PointId", "ModelId", "DeviceId", "start", "end"};
    }


    json
    RangeReading::get(json&& j) const
    {
        return get(forward<json>(j));
    }


    json
    RangeReading::get(const json& j) const
    {
        return get_data(j["PointId"], j["ModelId"], j["DeviceId"], j["start"], j["end"]);
    }


    json
    RangeReading::get_data(const string& point_id,
                           const string& model_id,
                           int device_id,
                           const string& start,
                           const string& end) const
    {
        json j;

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
        return j;
    }
}
}