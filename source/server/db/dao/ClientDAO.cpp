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
#include <db/db_exceptions.hpp>
#include "ClientDAO.hpp"

using namespace std;
using namespace boost::gregorian;

namespace solarplant
{
namespace db
{
namespace dao
{
ClientDAO::ClientDAO(const std::shared_ptr<sql::Connection> conn)
        : conn(conn)
{
    if ( conn == nullptr )
    {
        cerr << "Warning: Passing a nullptr connection to ClientDAO's constructor" << endl;
    }
}

entity::Client ClientDAO::get(ClientDAO::key_type id)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;
    entity::Client             client;

    if ( conn == nullptr || !conn->isValid())
    {
        throw DAOException("Invalid connection! Attempting to query the db will result in a segfault");
    }

    try
    {
        stmt = unique_ptr<sql::Statement>(conn->createStatement());
        res  = unique_ptr<sql::ResultSet>(
                stmt->executeQuery("SELECT * FROM " + TABLE_NAME + " WHERE " + columns[ "ID" ] + "=" + to_string(id))
        );

        // Advance to first entry
        res->next();

        // Build client
        client.id         = id;
        client.age        = res->getUInt(columns[ "AGE_COLUMN" ]);
        client.first_name = res->getString(columns[ "FIRST_NAME_COLUMN" ]);
        client.last_name  = res->getString(columns[ "LAST_NAME_COLUMN" ]);
        client.dob        = from_string(res->getString(columns[ "DOB_COLUMN" ]));

    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

    return client;
}

void ClientDAO::save(const entity::Client &client)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;

    if ( conn == nullptr || !conn->isValid())
    {
        throw DAOException("Invalid connection! Attempting to query the db will result in a segfault");
    }

    try
    {
        stmt = unique_ptr<sql::Statement>(conn->createStatement());
        res  = unique_ptr<sql::ResultSet>(
                stmt->executeQuery("INSERT INTO"
                                   + TABLE_NAME +
                                   "(`id`,"
                                           " `first_name`,"
                                           " `last_name`,"
                                           " `age`"
                                           ")"
                                           " VALUES ("
                                           client.id
                                           " 'Yo',"
                                           " 'Yee',"
                                           " '21'"
                                           ");")
        );
    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

}
}
}
}

