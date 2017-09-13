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
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "ClientDAO.hpp"

using namespace std;
namespace solarplant
{
namespace srv
{
namespace dao
{

ClientDAO::ClientDAO(const std::shared_ptr<sql::Connection> conn)
        : conn( conn )
{
    if (conn == nullptr || !conn->isValid())
        throw sql::SQLException("Failed to create ClientDAO due to invalid connection.");
}

entity::Client ClientDAO::get(ClientDAO::key_type id)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;
    entity::Client client;
    try
    {
        stmt = unique_ptr<sql::Statement>( conn->createStatement() );
        res = unique_ptr<sql::ResultSet>(
                stmt->executeQuery( "SELECT * FROM Client WHERE id = " + to_string( id ) )
        );

        // Advance to first entry
        res->next();

        // Build client
        client.id = id;
        client.age = res->getUInt( "age" );
        client.first_name = res->getString( "first_name" );
        client.last_name = res->getString( "last_name" );

    } catch (sql::SQLException e)
    {
        // Rethrow
        throw e;
    }

    return client;
}

}
}
}

