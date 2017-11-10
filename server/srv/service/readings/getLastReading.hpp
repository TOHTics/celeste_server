/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 3/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_QUERY_LASTREADING_HPP
#define SERVER_RESOURCE_QUERY_LASTREADING_HPP

#include <restbed>
#include <memory>
#include <mysql_devapi.h>
#include <json.hpp>

namespace celeste
{
namespace resource
{
    std::shared_ptr<restbed::Resource> make_new_device(std::string path,
                                                       const std::shared_ptr<mysqlx::Session> dbSession);
namespace reading
{
    /**
     * @brief      Gets the last measurement from one of the models on the device.
     *
     * @param[in]  point_id   The point identifier
     * @param[in]  model_id   The model identifier
     * @param[in]  device_id  The device identifier
     *
     * @return     Returns a JSON of the following structure:
     * ````
     * {
     *      "0" : {
     *              "sf" : 0.0
     *              "t" : "2011-05-12 09:20:50"
     *              "value" : value0
     *            }
     *      .
     *      .
     *      .
     *      
     *      "N" : {
     *              "sf" : 0.0
     *              "t" : 2012-05-12 09:20:50
     *              "value" : valueN
     *            }
     *      
     *      "count" : N
     * }
     * 
     * Where each index represents an aggregated model. Ie. a device may have multiple models.
     * ````
     */
    nlohmann::json getLastReading(const std::string& point_id,
                                  const std::string& model_id,
                                  int device_id,
                                  const std::shared_ptr<mysqlx::Session> dbSession);
}
}
}

#endif