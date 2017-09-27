/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class definitions.
 */
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "ClientDAO.hpp"
#include "db/exceptions.hpp"
#include "db/util.hpp"

using namespace std;
using namespace solarplant::db::util;

namespace solarplant
{
namespace db
{
namespace dao
{
ClientDAO::ClientDAO(const std::shared_ptr<sql::Connection> conn)
        : GenericDAO( conn )
{
    assert(conn != nullptr);
}

entity::Client ClientDAO::get(const ClientDAO::key_type & id)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;
    entity::Client             client;

    if ( conn == nullptr || !conn->isValid())
    {
        throw sql::SQLException( "Invalid connection! Attempting to query the db will result in a segfault" );
    }

    try
    {
        stmt = unique_ptr<sql::Statement>( conn->createStatement() );
        res  = unique_ptr<sql::ResultSet>(
                stmt->executeQuery("SELECT * FROM " + TABLE_NAME + " WHERE " + "id" + "=" + to_string( id ))
        );

        // Advance to first entry
        res->next();

        // Build client
        client.id         = id;
        client.first_name = res->getString( "first_name" );
        client.last_name  = res->getString( "last_name" );
        client.dob        = string_to_date( res->getString("dob") );

    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

    return client;
}

void ClientDAO::save(const entity::Client & client)
{
    unique_ptr<sql::PreparedStatement> stmt;
    unique_ptr<sql::ResultSet>         res;

    if ( conn == nullptr || !conn->isValid())
        throw sql::SQLException( "Invalid or NULL connection to database." );

    std::vector<std::string> values{
        to_string(client.id),
        to_string(client.first_name),
        to_string(client.last_name),
        to_string(client.dob)
    };

    try
    {
        stmt = unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement(
                "INSERT INTO " + TABLE_NAME + "(" + util::as_comma_list(util::as_quote_vector(columns, '`')) + ") " +
                "VALUES (" + util::as_comma_list(util::as_quote_vector(values)) + ");"
        ));

        stmt->execute();
    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

}
}
}
}

