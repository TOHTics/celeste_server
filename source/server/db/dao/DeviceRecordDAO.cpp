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
 * @brief Contains class definitions for DeviceRecordDAO
 */
//</editor-fold>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "DeviceRecordDAO.hpp"
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
DeviceRecordDAO::DeviceRecordDAO(const std::shared_ptr<sql::Connection> conn)
        : GenericDAO( conn )
{
    assert(conn != nullptr);
}

entity::DeviceRecord DeviceRecordDAO::get(const DeviceRecordDAO::key_type & id)
{
    unique_ptr<sql::Statement>       stmt;
    unique_ptr<sql::ResultSet>       res;
    entity::DeviceRecord             deviceRecord;

    if ( conn == nullptr || !conn->isValid())
    {
        throw sql::SQLException( "Invalid connection! Attempting to query the db will result in a segfault" );
    }

    try
    {
        // Build condition.
        std::string cond;
        size_t idx = 0;
        cond += "WHERE ";

        cond += pk_columns[0] + "=" + to_string( std::get<0>(id) ) + ", ";
        cond += pk_columns[1] + "=" + to_string( std::get<1>(id) );


        cout << cond << endl;
        // Execute statement
        stmt = unique_ptr<sql::Statement>( conn->createStatement() );
        res  = unique_ptr<sql::ResultSet>(
                stmt->executeQuery("SELECT * FROM " + TABLE_NAME + cond + ";")
        );

        // Advance to first entry
        res->next();

        // Build deviceRecord
        deviceRecord.device_id  = res->getInt( "device_id" );
        deviceRecord.index      = res->getInt( "index" );
        deviceRecord.t          = boost::posix_time::time_from_string(res->getString( "t" ));
        deviceRecord.cid        = res->getString( "cid" );
        deviceRecord.lif        = res->getString( "lif" );
        deviceRecord.lid        = res->getString( "lid" );

    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

    return deviceRecord;
}

void DeviceRecordDAO::save(const entity::DeviceRecord & deviceRecord)
{
    unique_ptr<sql::PreparedStatement> stmt;
    unique_ptr<sql::ResultSet>         res;

    if ( conn == nullptr || !conn->isValid())
        throw sql::SQLException( "Invalid or NULL connection to database." );

    std::vector<std::string> values{
        to_string(deviceRecord.device_id),
        to_string(deviceRecord.index),
        to_string(deviceRecord.t),
        to_string(deviceRecord.cid),
        to_string(deviceRecord.lif),
        to_string(deviceRecord.lid)
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

