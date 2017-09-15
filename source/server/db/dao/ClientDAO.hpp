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
 * @brief No description available.
 */
//</editor-fold>
#ifndef SERVER_DB_CLIENTDAO_HPP
#define SERVER_DB_CLIENTDAO_HPP

#include <memory>
#include <string>
#include <cppconn/connection.h>
#include <unordered_map>
#include "db/entity/Client.hpp"

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class ClientDAO
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 9/8/17.
 * 
 * @brief No description available.
 * 
 * # Description
 * No full length description available.
 */
class ClientDAO
{
    std::shared_ptr<sql::Connection> conn;
public:
    typedef int key_type;

   std::string TABLE_NAME = "Client";
   std::unordered_map<std::string, std::string> columns{
            {"id", "id"},
            {"age", "age"},
            {"first_name", "first_name"},
            {"last_name", "last_name"},
            {"dob", "dob"},
    };

    /**
     * Constructs a client given the database MySQL connection.
     * @param conn A MySQL connection to the database.
     */
    ClientDAO(const std::shared_ptr<sql::Connection> conn);

    /**
     * Retrieves a client uniquely identified by `id` from the database.
     * @param id The id of the client.
     * @return Client from the database.
     * @throws `SQLException` if an error with the retrieval of information happens.
     * @throws `DAOException` if the connection is invalid. This is most likely to
     * `conn == nullptr` evaluating to `true`.
     */
    entity::Client get(key_type id);


    void save(const entity::Client &client);
};
}
}
}


#endif //SERVER_CLIENTDAO_HPP
