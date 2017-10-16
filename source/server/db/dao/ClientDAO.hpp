/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_CLIENTDAO_HPP
#define SERVER_DB_CLIENTDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <vector>
#include "GenericDAO.hpp"
#include "db/entity/Client.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class   ClientDAO
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 * 
 * No full length description available.
 */
class ClientDAO : public GenericDAO< entity::Client, std::string >
{
    const std::string TABLE_NAME = "Client";  ///< Name associated table in the db
    const std::vector<std::string> columns {
        "id",
        "first_name",
        "last_name",
        "dob"
    };                                  ///< Set of columns associated with the table in the db

public:
    /**
     * @brief       Constructs a client given the database MySQL connection.
     * @param conn  A MySQL connection to the database.
     */
    ClientDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * @brief        Retrieves a client uniquely identified by `id` from the database.
     * @param[in]    id The id of the client.
     * @return       Client from the database.
     * @throws      `SQLException` if an error with the retrieval of information happens.
     * @throws      `DAOException` if the connection is invalid. This is most likely to
     *              `conn == nullptr` evaluating to `true`.
     */
    entity::Client get(const key_type & id);

    /**
     * @brief      Saves a client to the database.
     * @param[in]  client  The client to save.
     */
    void save(const entity::Client & client);
};
}
}
}


#endif
