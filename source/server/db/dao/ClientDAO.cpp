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
 * @brief Contains class definitions for ClientDAO
 */
//</editor-fold>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "ClientDAO.hpp"
#include "db/exceptions.hpp"
#include "db/util.hpp"

using namespace std;
using namespace boost::gregorian;

namespace solarplant
{
namespace db
{
namespace dao
{
ClientDAO::ClientDAO(const std::shared_ptr<sql::Connection> conn)
        : conn( conn )
{
    if ( conn == nullptr )
    {
        cerr << "Warning: Passing a nullptr connection to ClientDAO's constructor" << endl;
    }
}

entity::Client ClientDAO::get(const ClientDAO::key_type & id)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;
    entity::Client             client;

    if ( conn == nullptr || !conn->isValid())
    {
        throw DAOException( "Invalid connection! Attempting to query the db will result in a segfault" );
    }

    try
    {
        stmt = unique_ptr<sql::Statement>( conn->createStatement() );
        res  = unique_ptr<sql::ResultSet>(
                stmt->executeQuery("SELECT * FROM " + TABLE_NAME + " WHERE " + "ID" + "=" + to_string( id ))
        );

        // Advance to first entry
        res->next();

        // Build client
        client.id         = id;
        client.age        = res->getUInt( "age" );
        client.first_name = res->getString( "first_name" );
        client.last_name  = res->getString( "last_name" );
        client.dob        = from_string( res->getString( "dob" ));

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
    {
        throw DAOException( "Invalid connection! Attempting to query the db will result in a segfault" );
    }
    try
    {
        stmt = unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement(
                "Here goes string"

        ));
        std::cout << stmt->execute() << std::endl;
        std::cout << stmt->getUpdateCount() << std::endl;
    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

}
}
}
}

