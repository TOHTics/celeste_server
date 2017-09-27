/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_DEVICERECORDDAO_HPP
#define SERVER_DB_DEVICERECORDDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <vector>
#include <tuple>
#include "GenericDAO.hpp"
#include "db/entity/DeviceRecord.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class   DeviceRecordDAO
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 * 
 * No full length description available.
 */
class DeviceRecordDAO : public GenericDAO< entity::DeviceRecord, std::tuple<int, int> >
{
    const std::string TABLE_NAME = "DeviceRecord";  ///< Name associated table in the db
    const std::vector<std::string> columns {
        "device_id",
        "index",
        "t",
        "cid",
        "if",
        "lid"
    };                                  ///< Set of columns associated with the table in the db

    const std::vector<std::string> pk_columns {
        "device_id",
        "index"
    };
public:
    /**
     * @brief       Constructs a DeviceRecord given the database MySQL connection.
     * @param conn  A MySQL connection to the database.
     */
    DeviceRecordDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * @brief        Retrieves a DeviceRecord uniquely identified by `id` from the database.
     * @param[in]    id The id of the DeviceRecord.
     * @return       DeviceRecord from the database.
     * @throws      `SQLException` if an error with the retrieval of information happens.
     * @throws      `DAOException` if the connection is invalid. This is most likely to
     *              `conn == nullptr` evaluating to `true`.
     */
    entity::DeviceRecord get(const key_type & id);

    /**
     * @brief      Saves a DeviceRecord to the database.
     * @param[in]  DeviceRecord  The DeviceRecord to save.
     */
    void save(const entity::DeviceRecord & DeviceRecord);
};
}
}
}


#endif
