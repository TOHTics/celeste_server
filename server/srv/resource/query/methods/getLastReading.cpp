/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 3/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#include <string>
#include <mysql_devapi.h>
#include "methods.hpp"
#include <type_traits>

using namespace std;
using namespace celeste::srv;

using json = nlohmann::json;


namespace nlohmann {

    template <>
    struct adl_serializer<mysqlx::Value>
    {
        static void to_json(json &j, const mysqlx::Value& val)
        {
            if (val.isNull())
            {
                j = nullptr;
                return;
            }

            auto type = val.getType();
            using ValueType = mysqlx::Value::Type;
            switch (type)
            {
                case ValueType::FLOAT:
                    j = val.get<float>();
                    break;
                case ValueType::DOUBLE:
                    j = val.get<double>();
                    break;
                case ValueType::INT64:
                    j = val.get<int>();
                    break;
                case ValueType::STRING:
                    j = val.get<mysqlx::string>();
                    break;
                case ValueType::BOOL:
                    j = val.get<bool>();
                    break;
                default:
                    throw invalid_argument("Critical Error. Unhandled data type.");
            }
        }   
    };


    template <>
    struct adl_serializer<mysqlx::SqlResult>
    {
        static void to_json(json &j, mysqlx::SqlResult res)
        {
            json jr;
            int i = 0;
            for (const mysqlx::Row& row : res.fetchAll())
            {
                int j = 0;
                for (const mysqlx::Column& column : res.getColumns())
                {
                    jr[to_string(i)][column.getColumnName()] = row[j];
                    j++;
                }
                i++;
            }
            j = jr; 
        }
    };

    template <>
    struct adl_serializer<mysqlx::RowResult>
    {
        static void to_json(json &j, mysqlx::RowResult res)
        {
            json jr;
            int i = 0;
            for (const mysqlx::Row& row : res.fetchAll())
            {
                int j = 0;
                for (const mysqlx::Column& column : res.getColumns())
                {
                    jr[to_string(i)][column.getColumnName()] = row[j];
                    j++;
                }
                i++;
            }
            jr["count"] = i;

            j = jr; 
        }
    };
}


namespace celeste
{
namespace srv
{
namespace query
{
    using ValueList = std::vector<mysqlx::Value>;

    using ValueMap = std::map<std::string, mysqlx::Value>;

    enum PointType
    {
        STRING = 0,
        INT,
        DOUBLE,
        FLOAT
    };

    json getLastReading(const string& point_id, const string& model_id, int device_id, shared_ptr<mysqlx::Session> dbSession)
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
        json j = std::move(res);

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
            auto data = it->at("data").get<std::string>();
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
}
