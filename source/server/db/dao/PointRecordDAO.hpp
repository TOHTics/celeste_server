/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_POINTRECORDDAO_HPP
#define SERVER_DB_POINTRECORDDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <vector>
#include <tuple>
#include "GenericDAO.hpp"
#include "db/entity/PointRecord.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class   PointRecordDAO
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 * 
 * No full length description available.
 */
class PointRecordDAO : public GenericDAO< entity::PointRecord, std::tuple<int, std::string, std::string, int, int, int> >
{
    const std::string TABLE_NAME = "PointRecord";  ///< Name associated table in the db
    const std::vector<std::string> columns {
        "device_id",
        "model_id",
        "point_id",
        "DeviceRecord_index",
        "Model_index",
        "index",
        "sf",
        "t"
    };                                  ///< Set of columns associated with the table in the db

    const std::vector<std::string> pk_columns {
        "device_id",
        "model_id",
        "point_id",
        "DeviceRecord_index",
        "Model_index",
        "index"
    }; 

    constexpr static int n = 6;
public:
    /**
     * @brief       Constructs a PointRecord given the database MySQL connection.
     * @param conn  A MySQL connection to the database.
     */
    PointRecordDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * @brief        Retrieves a PointRecord uniquely identified by `id` from the database.
     * @param[in]    id The id of the PointRecord.
     * @return       PointRecord from the database.
     * @throws      `SQLException` if an error with the retrieval of information happens.
     * @throws      `DAOException` if the connection is invalid. This is most likely to
     *              `conn == nullptr` evaluating to `true`.
     */
    entity::PointRecord get(const key_type & id);

    /**
     * @brief      Saves a PointRecord to the database.
     * @param[in]  PointRecord  The PointRecord to save.
     */
    void save(const entity::PointRecord & PointRecord);
};
}
}
}


#endif
