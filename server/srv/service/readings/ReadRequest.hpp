/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#ifndef SERVER_RESOURCE_READING_REQUEST_DATA_HPP
#define SERVER_RESOURCE_READING_REQUEST_DATA_HPP

#include <vector>
#include <string>

namespace celeste
{
namespace resource
{
    struct LastReadRequest
    {
        std::string     DeviceId;
        std::string     ModelId;
        std::string     PointId;
    };

    struct RangeReadRequest
    {
        std::string     DeviceId;
        std::string     ModelId;
        std::string     PointId;

        std::string     start;
        std::string     end;
    };

    struct TodayReadRequest
    {
        std::string     DeviceId;
        std::string     ModelId;
        std::string     PointId;
    };

    struct YesterdayReadRequest
    {
        std::string     DeviceId;
        std::string     ModelId;
        std::string     PointId;
    };

    struct MonthReadRequest
    {
        std::string     DeviceId;
        std::string     ModelId;
        std::string     PointId;

        int             month;
    };

    struct YearReadRequest
    {
        std::string     DeviceId;
        std::string     ModelId;
        std::string     PointId;

        int             year;
    };

    struct AccumulatedReadRequest
    {
        std::vector<std::string>    DeviceIds;
        std::string                 ModelId;
        std::string                 PointId;

        std::string                 start;
        std::string                 end;
    };

}
}

#endif