/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <cmath>
#include <soci/mysql/soci-mysql.h>
#include "ReadingFetcher.hpp"
#include "ReadRequest.hpp"
#include "Reading.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    ReadingFetcher::ReadingFetcher(const std::string& dbSettings)
        :   sql(mysql, dbSettings),
            dbSettings(dbSettings)
    {}

    ReadingFetcher::ReadingFetcher(const ReadingFetcher& other)
        :   ReadingFetcher(other.dbSettings)
    {}

    // --- LastReadRequest fetch implementations ---------
    template <>
    Reading
    ReadingFetcher::fetch(const LastReadRequest& req) const
    {
        Reading out;
        sql 
            << "select sf, t, v from PointRecord "
            << "where Device_id = :DeviceId "
            << "and Model_id = :ModelId "
            << "and Point_id = :PointId "
            << "order by t desc limit 1",
            use(req.DeviceId, "DeviceId"), use(req.ModelId, "ModelId"), use(req.PointId, "PointId"),
            into(out.sf), into(out.t), into(out.value);
        return out;
    }

    // --- RangeReadRequest fetch implementations --------
    template <>
    std::vector<Reading> 
    ReadingFetcher::fetch(const RangeReadRequest& req) const
    {
        rowset<Reading> res = (sql.prepare
                               << "select sf, t, v from PointRecord "
                               << "where "
                               << "Device_id = :DeviceId "
                               << "and Model_id = :ModelId "
                               << "and Point_id = :PointId "
                               << "and t > :start "
                               << "and t < :end",
                               use(req.DeviceId, "DeviceId"),
                               use(req.ModelId, "ModelId"),
                               use(req.PointId, "PointId"),
                               use(req.start, "start"),
                               use(req.end, "end"));
        std::vector<Reading> readings;
        readings.reserve(200);
        for (auto it = res.begin(); it != res.end(); ++it)
            readings.push_back(*it);
        return readings;
    }

    // --- YesterdayReadRequest fetch implementations ----

    // --- TodayReadRequest fetch implementations --------

    // --- AccumulatedReadRequest fetch implementations --
    template <>
    std::map<string, double> 
    ReadingFetcher::fetch(const AccumulatedReadRequest& req) const
    {
        std::string deviceId;
        double total;
        statement stmt = (sql.prepare
                           << "select sum(v) "
                           << "from PointRecord "
                           << "where Device_id = :DeviceId ",
                           use(deviceId), into(total)
                           );  
        
        std::map<string, double> accMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;
            stmt.execute(true);
            accMap.insert({deviceId, total});
        }
        return accMap;
    }
}
}