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
    template <
        class DeviceId,
        class ModelId,
        class PointId
    >
    struct basic_read_request
    {
        DeviceId  deviceId;
        ModelId   modelId;
        PointId   pointId;
    };

    using read_request = basic_read_request<int, std::string, std::string>;
    using LastReadRequest = read_request;
}
}

#endif