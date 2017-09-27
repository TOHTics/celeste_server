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
 * @brief Contains class declarations for ModelDAO
 */
//</editor-fold>
#ifndef SERVER_DB_MODELDAO_HPP
#define SERVER_DB_MODELDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <vector>
#include "GenericDAO.hpp"
#include "db/entity/Model.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class ModelDAO
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 9/8/17.
 * 
 * @brief No description available.
 * 
 * # Description
 * No full length description available.
 */
class ModelDAO : public GenericDAO< entity::Model, std::string >
{
    const std::string TABLE_NAME = "Model";  ///< Name associated table in the db
    const std::vector<std::string> columns {
        "id",
        "index",
        "device_id",
        "ns"
    };                                  ///< Set of columns associated with the table in the db

public:
    /**
     * @brief       Constructs a Model given the database MySQL connection.
     * @param conn  A MySQL connection to the database.
     */
    ModelDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * @brief        Retrieves a Model uniquely identified by `id` from the database.
     * @param[in]    id The id of the Model.
     * @return       Model from the database.
     * @throws      `SQLException` if an error with the retrieval of information happens.
     * @throws      `DAOException` if the connection is invalid. This is most likely to
     *              `conn == nullptr` evaluating to `true`.
     */
    entity::Model get(const key_type & id);

    /**
     * @brief      Saves a Model to the database.
     * @param[in]  Model  The Model to save.
     */
    void save(const entity::Model & Model);
};
}
}
}


#endif
