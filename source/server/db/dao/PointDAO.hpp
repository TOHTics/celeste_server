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
 * @brief Contains class declarations for PointDAO
 */
//</editor-fold>
#ifndef SERVER_DB_POINTDAO_HPP
#define SERVER_DB_POINTDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <vector>
#include "GenericDAO.hpp"
#include "db/entity/Point.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class PointDAO
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 9/8/17.
 * 
 * @brief No description available.
 * 
 * # Description
 * No full length description available.
 */
class PointDAO : public GenericDAO< entity::Point, std::string >
{
    const std::string TABLE_NAME = "Point";  ///< Name associated table in the db
    const std::vector<std::string> columns {
        "id",
        "model_id",
        "type",
        "u",
        "d"
    };                                  ///< Set of columns associated with the table in the db

public:
    /**
     * @brief       Constructs a Point given the database MySQL connection.
     * @param conn  A MySQL connection to the database.
     */
    PointDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * @brief        Retrieves a Point uniquely identified by `id` from the database.
     * @param[in]    id The id of the Point.
     * @return       Point from the database.
     * @throws      `SQLException` if an error with the retrieval of information happens.
     * @throws      `DAOException` if the connection is invalid. This is most likely to
     *              `conn == nullptr` evaluating to `true`.
     */
    entity::Point get(const key_type & id);

    /**
     * @brief      Saves a Point to the database.
     * @param[in]  Point  The Point to save.
     */
    void save(const entity::Point & Point);
};
}
}
}


#endif //SERVER_POINTDAO_HPP
