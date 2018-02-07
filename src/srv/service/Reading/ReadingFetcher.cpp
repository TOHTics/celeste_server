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

#include "srv/service/common.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    ReadingFetcher::ReadingFetcher(const string& dbSettings)
        :   m_sql(mysql, dbSettings),
            m_dbSettings(dbSettings)
    {}

    ReadingFetcher::ReadingFetcher(const ReadingFetcher& other)
        :   ReadingFetcher(other.m_dbSettings)
    {}

    // ==== LastReadRequest fetch implementations ========
    template <>
    Reading
    ReadingFetcher::fetch(const LastReadRequest& req)
    const
    {
        // "ping" the server
        try
        {
            m_sql << "select 1 from dual";
        } catch (mysql_soci_error&)
        {
            m_sql.reconnect();
        } catch (exception&)
        {
            m_sql.open(mysql, m_dbSettings);
        }

        Reading out;
        m_sql 
            << "select sf, t, v from PointRecord "
            << "where Device_id = :DeviceId "
            << "and Model_id = :ModelId "
            << "and Point_id = :PointId "
            << "order by t desc limit 1",
            into(out),
            use(req.DeviceId, "DeviceId"),
            use(req.ModelId, "ModelId"),
            use(req.PointId, "PointId");

        int type;
        m_sql << "select type from Point where id = :PointId and Model_id = :ModelId",
        into(type), use(req.PointId, "PointId"), use(req.ModelId, "ModelId");

        string2point(type, boost::get<string>(out.value), out.value);

        return out;
    }

    // ==== RangeReadRequest fetch implementations =======
    template <>
    vector<Reading> 
    ReadingFetcher::fetch(const RangeReadRequest& req)
    const
    {
        session sql(mysql, m_dbSettings);
        Reading read;
        statement stmt = (sql.prepare 
                          << "select sf, t, v from PointRecord "
                          << "where "
                          << "Device_id = :DeviceId "
                          << "and Model_id = :ModelId "
                          << "and Point_id = :PointId "
                          << "and t > :start "
                          << "and t < :end "
                          << "order by t desc "
                          << "limit :limn",
                          into(read),
                          use(req.DeviceId,    "DeviceId"),
                          use(req.ModelId,     "ModelId"),
                          use(req.PointId,     "PointId"),
                          use(req.start,       "start"),
                          use(req.end,         "end"),
                          use(req.limit,       "limn")
                          );
        try
        {
           stmt.execute(true); 
        } catch (mysql_soci_error&)
        {
            sql.reconnect();
        }

        int type;
        sql << "select type from Point where id = :PointId and Model_id = :ModelId",
        into(type), use(req.PointId, "PointId"), use(req.ModelId, "ModelId");

        vector<Reading> readings;
        readings.reserve(req.limit);
        while (stmt.fetch())
        {
            string2point(type, boost::get<string>(read.value), read.value);
            readings.push_back(read);
        }

        return readings;
    }

    // ==== YesterdayReadRequest fetch implementations ===
    template <>
    map<string, vector<pair<int, double>>> ReadingFetcher::fetch(const YesterdayReadRequest& req)
    const
    {
        session sql(mysql, m_dbSettings);
        pair<int, double> avg;
        indicator ind;
        string deviceId;
        statement stmt = (sql.prepare
                          << "select hour(t), avg(v) from PointRecord "
                          << "where Device_id = :DeviceId and "
                          << "Model_id = :ModelId and "
                          << "Point_id = :PointId and "
                          << "day(t) = day(curdate() - 1) "
                          << "group by hour(t)",
                          into(avg, ind),
                          use(deviceId,     "DeviceId"),
                          use(req.ModelId,  "ModelId"),
                          use(req.PointId,  "PointId")
                          );  

        map<string, vector<pair<int, double>>> devMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;
            stmt.execute(true); 
            vector<pair<int, double>> avgs;
            if (sql.got_data())
            {
                avgs.reserve(24); // capacity is number of hours in a day
                do 
                {
                    avgs.push_back(avg);
                } while (stmt.fetch());
            }
            devMap.insert({deviceId, std::move(avgs)});
        }
        return devMap;
    }

    // ==== TodayReadRequest fetch implementations =======
    template <>
    map<string, vector<pair<int, double>>> ReadingFetcher::fetch(const TodayReadRequest& req)
    const
    {
        session sql(mysql, m_dbSettings);
        pair<int, double> avg;
        indicator ind;
        string deviceId;
        statement stmt = (sql.prepare
                          << "select hour(t), avg(v) from PointRecord "
                          << "where Device_id = :DeviceId and "
                          << "Model_id = :ModelId and "
                          << "Point_id = :PointId and "
                          << "day(t) = day(curdate()) "
                          << "group by hour(t)",
                          into(avg, ind),
                          use(deviceId,     "DeviceId"),
                          use(req.ModelId,  "ModelId"),
                          use(req.PointId,  "PointId")
                          );  
        
        map<string, vector<pair<int, double>>> devMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;

            stmt.execute(true);

            vector<pair<int, double>> avgs;
            if (sql.got_data())
            {
                avgs.reserve(24); // capacity is numberof hours in a day
                do 
                {
                    avgs.push_back(avg);
                } while (stmt.fetch());
            }
            devMap.insert({deviceId, std::move(avgs)});
        }
        return devMap;
    }

    // --- AccumulatedReadRequest fetch implementations --
    template <>
    map<string, double> 
    ReadingFetcher::fetch(const AccumulatedReadRequest& req)
    const
    {
        session sql(mysql, m_dbSettings);
        double total;
        string deviceId;
        indicator ind;
        statement stmt = (sql.prepare
                           << "select sum(v) "
                           << "from PointRecord "
                           << "where Device_id = :DeviceId and "
                           << "Model_id = :ModelId and "
                           << "Point_id = :PointId and "
                           << "t > :start and "
                           << "t < :end",
                           into(total, ind),
                           use(deviceId,    "DeviceId"),
                           use(req.ModelId, "ModelId"),
                           use(req.PointId, "PointId"),
                           use(req.start,   "start"),
                           use(req.end,     "end")
                           );  
        
        map<string, double> devMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;
            stmt.execute(true);
            if (ind != i_ok)
                total = 0;
            devMap.insert({deviceId, total});
        }
        return devMap;
    }

    // --- AverageReadRequest ----------------------------
    template <>
    map<string, double> ReadingFetcher::fetch(const AverageReadRequest& req)
    const
    {
        session sql(mysql, m_dbSettings);
        double avg;
        indicator ind;
        string deviceId;
        statement stmt = (sql.prepare
                           << "select avg(v) "
                           << "from PointRecord "
                           << "where Device_id = :DeviceId and "
                           << "Model_id = :ModelId and "
                           << "Point_id = :PointId and "
                           << "t > :start and "
                           << "t < :end",
                           into(avg, ind),
                           use(deviceId,    "DeviceId"),
                           use(req.ModelId, "ModelId"),
                           use(req.PointId, "PointId"),
                           use(req.start,   "start"),
                           use(req.end,     "end")
                           );  
        
        map<string, double> devMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;
            stmt.execute(true);

            if (ind != i_ok)
                avg = 0;
            devMap.insert({deviceId, avg});
        }
        return devMap;
    }

    // --- DayReadRequest ----------------------------
    template <>
    map<string, vector<pair<int, double>>> ReadingFetcher::fetch(const DayReadRequest& req)
    const
    {
        session sql(mysql, m_dbSettings);
        pair<int, double> avg;
        indicator ind;
        string deviceId;
        statement stmt = (sql.prepare
                          << "select hour(t), avg(v) from PointRecord "
                          << "where Device_id = :DeviceId and "
                          << "Model_id = :ModelId and "
                          << "Point_id = :PointId and "
                          << "day(t) = day(:day) "
                          << "group by hour(t)",
                          into(avg),
                          use(deviceId,     "DeviceId"),
                          use(req.ModelId,  "ModelId"),
                          use(req.PointId,  "PointId"),
                          use(req.day,      "day")
                          );  
        
        map<string, vector<pair<int, double>>> devMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;
            stmt.execute(true); 
            vector<pair<int, double>> avgs;
            if (sql.got_data())
            {
                avgs.reserve(24); // capacity is number of hours in a day
                do 
                {
                    avgs.push_back(avg);
                } while (stmt.fetch());
            }
            devMap.insert({deviceId, std::move(avgs)});
        }
        return devMap;
    }

    // --- MonthReadRequest ----------------------------
    template <>
    map<string, vector<pair<int, double>>> ReadingFetcher::fetch(const MonthReadRequest& req)
    const
    {
    session sql(mysql, m_dbSettings);   
        pair<int, double> avg;
        indicator ind;
        string deviceId;
        statement stmt = (sql.prepare
                          << "select day(t), avg(v) from PointRecord "
                          << "where Device_id = :DeviceId and "
                          << "Model_id = :ModelId and "
                          << "Point_id = :PointId and "
                          << "month(t) = month(:month) "
                          << "group by day(t)",
                          into(avg, ind),
                          use(deviceId,     "DeviceId"),
                          use(req.ModelId,  "ModelId"),
                          use(req.PointId,  "PointId"),
                          use(req.month,    "month")
                          );  
        
        map<string, vector<pair<int, double>>> devMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;
            stmt.execute(true); 
            vector<pair<int, double>> avgs;
            if (sql.got_data())
            {
                avgs.reserve(31); // capacity is max number of days in a month
                do 
                {
                    avgs.push_back(avg);
                } while (stmt.fetch());
            }
            devMap.insert({deviceId, std::move(avgs)});
        }
        return devMap;
    }

    // --- YearReadRequest ----------------------------
    template <>
    map<string, vector<pair<int, double>>> ReadingFetcher::fetch(const YearReadRequest& req)
    const
    {
        session sql(mysql, m_dbSettings);
        pair<int, double> avg;
        indicator ind;
        string deviceId;
        statement stmt = (sql.prepare
                          << "select month(t), avg(v) from PointRecord "
                          << "where Device_id = :DeviceId and "
                          << "Model_id = :ModelId and "
                          << "Point_id = :PointId and "
                          << "year(t) = year(:year) "
                          << "group by month(t)",
                          into(avg, ind),
                          use(deviceId,     "DeviceId"),
                          use(req.ModelId,  "ModelId"),
                          use(req.PointId,  "PointId"),
                          use(req.year, "year")
                          );  
        map<string, vector<pair<int, double>>> devMap;
        for (const auto& id : req.DeviceIds)
        {
            deviceId = id;
            stmt.execute(true); 
            vector<pair<int, double>> avgs;
            if (sql.got_data())
            {
                avgs.reserve(12); // capacity is number of months in a year
                do 
                {
                    avgs.push_back(avg);
                } while (stmt.fetch());
            }
            devMap.insert({deviceId, std::move(avgs)});
        }
        return devMap;
    }
}
}