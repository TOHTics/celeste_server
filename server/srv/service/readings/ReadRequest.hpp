/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#ifndef SERVER_RESOURCE_READING_REQUEST_DATA_HPP
#define SERVER_RESOURCE_READING_REQUEST_DATA_HPP
#include <string>

namespace celeste
{
namespace resource
{
    struct LastReadRequest
    {
        int             DeviceId;
        std::string     ModelId;
        std::string     PointId;
    };

    struct RangeReadRequest
    {
        int             DeviceId;
        std::string     ModelId;
        std::string     PointId;

        std::string     start;
        std::string     end;
    };

    struct TodayReadRequest
    {
        int             DeviceId;
        std::string     ModelId;
        std::string     PointId;
    };

    struct YesterdayReadRequest
    {
        int             DeviceId;
        std::string     ModelId;
        std::string     PointId;
    };

    struct MonthReadRequest
    {
        int             DeviceId;
        std::string     ModelId;
        std::string     PointId;

        int             month;
    };

    struct YearReadRequest
    {
        int             DeviceId;
        std::string     ModelId;
        std::string     PointId;

        int             year;
    };

}
}

#endif