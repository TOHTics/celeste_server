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
 * @brief Contains class definitions for ModelDAO
 */
//</editor-fold>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "ModelDAO.hpp"
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
ModelDAO::ModelDAO(const std::shared_ptr<sql::Connection> conn) : GenericDAO( conn )
{
    assert(conn != nullptr);
}

entity::Model ModelDAO::get(const ModelDAO::key_type & id)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;
    entity::Model             model;

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

        // Build model
        model.id        = id;
        model.index     = res->getInt("index");
        model.device_id = res->getInt("device_id");
        model.ns        = res->getString("ns");
    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

    return model;
}

void ModelDAO::save(const entity::Model & model)
{
    unique_ptr<sql::PreparedStatement> stmt;
    unique_ptr<sql::ResultSet>         res;

    if ( conn == nullptr || !conn->isValid())
        throw sql::SQLException( "Invalid or NULL connection to database." );

    std::vector<std::string> values{
        to_string(model.id),
        to_string(model.index),
        to_string(model.device_id),
        to_string(model.ns)
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

