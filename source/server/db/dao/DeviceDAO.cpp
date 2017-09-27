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
 * @brief Contains class definitions for DeviceDAO
 */
//</editor-fold>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "DeviceDAO.hpp"
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
DeviceDAO::DeviceDAO(const std::shared_ptr<sql::Connection> conn) : GenericDAO( conn )
{
    assert(conn != nullptr);
}

entity::Device DeviceDAO::get(const DeviceDAO::key_type & id)
{
    unique_ptr<sql::Statement> stmt;
    unique_ptr<sql::ResultSet> res;
    entity::Device             device;

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

        // Build device
        device.id         = id;
        device.client_id  = res->getString("client_id");
        device.man        = res->getString("man");
        device.mod        = res->getString("mod");
        device.sn         = res->getString("sn");

    } catch ( const sql::SQLException &e )
    {
        throw e; // Rethrow
    }

    return device;
}

void DeviceDAO::save(const entity::Device & device)
{
    unique_ptr<sql::PreparedStatement> stmt;
    unique_ptr<sql::ResultSet>         res;

    if ( conn == nullptr || !conn->isValid())
        throw sql::SQLException( "Invalid or NULL connection to database." );

    std::vector<std::string> values{
        to_string(device.id),
        to_string(device.client_id),
        to_string(device.man),
        to_string(device.mod),
        to_string(device.sn)
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

