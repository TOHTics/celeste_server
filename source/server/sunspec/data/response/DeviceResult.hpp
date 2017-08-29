//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains class declarations for `DeviceResult`
 */
//</editor-fold>
#ifndef SERVER_DEVICEFAILURE_HPP
#define SERVER_DEVICEFAILURE_HPP


#include <string>
#include <boost/property_tree/ptree.hpp>

/**
 * @class DeviceResult
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/29/17.
 * 
 * @brief Describes the result of processing a device record
 */
struct DeviceResult
{
    std::string reason;
    std::string message;
    std::string code;

    DeviceResult() = default;
    DeviceResult(const DeviceResult& other) = default;

    static std::string to_xml(const DeviceResult& dev_result,
                              std::shared_ptr<boost::property_tree::ptree> ptOut = nullptr);
};


#endif //SERVER_DEVICEFAILURE_HPP
