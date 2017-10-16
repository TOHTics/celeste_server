/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_MODELRECORDDAO_HPP
#define SERVER_DB_MODELRECORDDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <vector>
#include <tuple>
#include "GenericDAO.hpp"
#include "db/entity/ModelRecord.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class   ModelRecordDAO
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 * 
 * No full length description available.
 */
class ModelRecordDAO : public GenericDAO< entity::ModelRecord, std::tuple<int, std::string, int, int> >
{
    const std::string TABLE_NAME = "ModelRecord";  ///< Name associated table in the db
    const std::vector<std::string> columns {
        "device_id",
        "model_id",
        "Model_index"
    };                                  ///< Set of columns associated with the table in the db

    const std::vector<std::string> pk_columns {
        "device_id",
        "model_id",
        "Model_index",
    }; 

    constexpr static int n = 3;
public:
    /**
     * @brief       Constructs a ModelRecord given the database MySQL connection.
     * @param conn  A MySQL connection to the database.
     */
    ModelRecordDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * @brief        Retrieves a ModelRecord uniquely identified by `id` from the database.
     * @param[in]    id The id of the ModelRecord.
     * @return       ModelRecord from the database.
     * @throws      `SQLException` if an error with the retrieval of information happens.
     * @throws      `DAOException` if the connection is invalid. This is most likely to
     *              `conn == nullptr` evaluating to `true`.
     */
    entity::ModelRecord get(const key_type & id);

    /**
     * @brief      Saves a ModelRecord to the database.
     * @param[in]  ModelRecord  The ModelRecord to save.
     */
    void save(const entity::ModelRecord & ModelRecord);
};
}
}
}


#endif
