/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <soci/mysql/soci-mysql.h>
#include "Point.hpp"
#include "srv/service/common.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Points<nlohmann::json>::Points(const std::string& dbSettings)
        : m_db_settings(dbSettings)
    {
        set_path("/point");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Point Points<nlohmann::json>::get(const std::string& pointId,
                                      const std::string& modelId)
    {
        session sql(mysql, m_db_settings);
        Point point;

        sql << "select * from Point where id = :PointId and Model_id = :ModelId",
            use(pointId), use(modelId), into(point);
        if (sql.got_data())
            return point;
        else
            throw status::POINT_NOT_FOUND;
    }

    void Points<nlohmann::json>::insert(const value_type& point)
    {
        session sql(mysql, m_db_settings);
        statement stmt = (sql.prepare 
                          << "insert into Point(id, Model_id, type, u, d) "
                          << "values(:PointId, :ModelId, :type, :u, :d)",
                          use(point)
                          );
        try
        {
            stmt.execute(true);
        } catch (const mysql_soci_error& e)
        {
            sql.reconnect();
            stmt.execute(true);
        }
    }

    void Points<nlohmann::json>::remove(const std::string& pointId,
                                        const std::string& modelId)
    {
        session sql(mysql, m_db_settings);
        statement stmt = (sql.prepare 
                          << "delete from Point "
                          << "where id = :PointId and Model_id = :ModelId",
                          use(pointId), use(modelId)
                        );
        try
        {
            stmt.execute(true);
        } catch (const mysql_soci_error& e)
        {
            sql.reconnect();
            stmt.execute(true);
        }
    }

    void Points<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["PointId"].is_null())
            throw status::MISSING_FIELD_POINTID;

        if (data["ModelId"].is_null())
            throw status::MISSING_FIELD_MODELID;

        // get Point from db
        json_type response = this->get(data["PointId"], data["ModelId"]);

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
    }

    void Points<nlohmann::json>::POST(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["PointId"].is_null())
            throw status::MISSING_FIELD_POINTID;

        if (data["ModelId"].is_null())
            throw status::MISSING_FIELD_MODELID;

        if (data["type"].is_null())
            throw status::MISSING_FIELD_TYPE;

        if (data["u"].is_null())
            data["u"] = nullptr;

        if (data["d"].is_null())
            data["d"] = nullptr;

        this->insert(data.get<Point>());

        // close
        session->close(restbed::OK);
    }

    void Points<nlohmann::json>::DELETE(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["PointId"].is_null())
            throw status::MISSING_FIELD_POINTID;

        if (data["ModelId"].is_null())
            throw status::MISSING_FIELD_MODELID;

        // remove Point from DB.
        this->remove(data["PointId"], data["ModelId"]);

        // close
        session->close(restbed::OK);
    }
}
}

// --- SERIALIZERS -------------------
namespace soci
{
    using namespace celeste::resource;

    void type_conversion<Point>::from_base(values const& v, indicator , Point& p)
    {
        p.PointId    = v.get<string>("id");
        p.ModelId    = v.get<string>("Model_id");
        p.type       = v.get<int>("type");
        p.u          = v.get<string>("u");
        p.d          = v.get<string>("d");
    }

    void type_conversion<Point>::to_base(const Point& p, values& v, indicator& ind)
    {
        v.set("PointId",    p.PointId);
        v.set("ModelId",    p.ModelId);
        v.set("type",       p.type);
        v.set("u",          p.u);
        v.set("d",          p.d);
        ind = i_ok;
    }
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<Point>::to_json(json& j, const Point& obj)
    {
        j = json {
            {"PointId", obj.PointId},
            {"ModelId", obj.ModelId},
            {"type", obj.type},
            {"u", obj.u},
            {"d", obj.d}
        };
    }

    void adl_serializer<Point>::from_json(const json& j, Point& obj)
    {
        obj.PointId = j.at("PointId");
        obj.ModelId = j.at("ModelId");
        obj.type = j.at("type");
        obj.u = j.at("u").get<decltype(obj.u)>();
        obj.d = j.at("d").get<decltype(obj.d)>();
    }
}