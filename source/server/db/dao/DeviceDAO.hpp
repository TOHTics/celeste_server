//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains class declarations for DeviceDAO
 */
//</editor-fold>
#ifndef SERVER_DB_DEVICEDAO_HPP
#define SERVER_DB_DEVICEDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <vector>
#include "GenericDAO.hpp"
#include "db/entity/Device.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class DeviceDAO
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 9/8/17.
 * 
 * @brief No description available.
 * 
 * # Description
 * No full length description available.
 */
class DeviceDAO : public GenericDAO< entity::Device, int >
{
    const std::string TABLE_NAME = "Device";  ///< Name associated table in the db
    const std::vector<std::string> columns {
        "id",
        "client_id",
        "man",
        "mod",
        "sn"
    };                                  ///< Set of columns associated with the table in the db

public:
    /**
     * @brief       Constructs a Device given the database MySQL connection.
     * @param conn  A MySQL connection to the database.
     */
    DeviceDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * @brief        Retrieves a Device uniquely identified by `id` from the database.
     * @param[in]    id The id of the device.
     * @return       Device from the database.
     * @throws      `SQLException` if an error with the retrieval of information happens.
     * @throws      `DAOException` if the connection is invalid. This is most likely to
     *              `conn == nullptr` evaluating to `true`.
     */
    entity::Device get(const key_type & id);

    /**
     * @brief      Saves a Device to the database.
     * @param[in]  Device  The Device to save.
     */
    void save(const entity::Device & device);
};
}
}
}


#endif //SERVER_DEVICEDAO_HPP
