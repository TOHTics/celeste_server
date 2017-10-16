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
#include "ModelRecordDAO.hpp"
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
ModelRecordDAO::ModelRecordDAO(const std::shared_ptr<sql::Connection> conn)
        : GenericDAO( conn )
{
    assert(conn != nullptr);
}

entity::ModelRecord ModelRecordDAO::get(const ModelRecordDAO::key_type & id)
{
    unique_ptr<sql::Statement>       stmt;
    unique_ptr<sql::ResultSet>       res;
    entity::ModelRecord              modelRecord;

    if ( conn == nullptr || !conn->isValid())
    {
        throw sql::SQLException( "Invalid connection! Attempting to query the db will result in a segfault" );
    }

    try
    {
        // Build condition.
        std::string cond;
        size_t idx = 0;
        cond += " WHERE ";

        static_for<0, n>([&](auto i) 
        {
            if constexpr (i < n - 1)
                cond += pk_columns[i] + "=" + quote(to_string(std::get<i.value>(id)), '\'') + " AND ";
            else
                cond += pk_columns[n - 1] + "=" + quote(to_string(std::get<i.value>(id)), '\'');
        });

        // Execute statement
        stmt = unique_ptr<sql::Statement>( conn->createStatement() );
        res  = unique_ptr<sql::ResultSet>(
                stmt->executeQuery("SELECT * FROM " + TABLE_NAME + cond + ";")
        );

        // Advance to first entry
        res->next();

        // Build ModelRecord
        modelRecord.device_id = res->getInt("device_id");
        modelRecord.model_id = res->getString("model_id");
        modelRecord.device_record_idx = res->getInt("deviceRecord_index");
        modelRecord.model_idx = res->getInt("model_index");

    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

    return modelRecord;
}

void ModelRecordDAO::save(const entity::ModelRecord & modelRecord)
{
    unique_ptr<sql::PreparedStatement> stmt;
    unique_ptr<sql::ResultSet>         res;

    if ( conn == nullptr || !conn->isValid())
        throw sql::SQLException( "Invalid or NULL connection to database." );

    std::vector<std::string> values{
        to_string(modelRecord.device_id),
        to_string(modelRecord.model_id),
        to_string(modelRecord.device_record_idx),
        to_string(modelRecord.model_idx)
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