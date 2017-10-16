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
#include "PointDAO.hpp"
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
PointDAO::PointDAO(const std::shared_ptr<sql::Connection> conn) : GenericDAO( conn )
{
    assert(conn != nullptr);
}

entity::Point PointDAO::get(const PointDAO::key_type & id)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;
    entity::Point             point;

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

        // Build point
        point.id       = id;
        point.model_id = res->getString("model_id");
        point.type     = res->getString("type");
        point.u        = res->getString("u");
        point.d        = res->getString("d");

    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

    return point;
}

void PointDAO::save(const entity::Point & point)
{
    unique_ptr<sql::PreparedStatement> stmt;
    unique_ptr<sql::ResultSet>         res;

    if ( conn == nullptr || !conn->isValid())
        throw sql::SQLException( "Invalid or NULL connection to database." );

    std::vector<std::string> values{
        to_string(point.id),
        to_string(point.model_id),
        to_string(point.type),
        to_string(point.u),
        to_string(point.d)
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

