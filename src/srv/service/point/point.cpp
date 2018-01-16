/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include "point.hpp"
#include "srv/service/common.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Points<nlohmann::json>::Points(const std::string& dbSettings)
        :   sqlPool(10, session(mysql, dbSettings))
    {
        set_path("/point");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Point Points<nlohmann::json>::get(const std::string& pointId,
                                      const std::string& modelId)
    {
        auto sql = sqlPool.acquire_wait();
        Point point;
        *sql    << "select * from Point where id = :PointId and Model_id = :ModelId",
                use(pointId), use(modelId), into(point);
        return point;
    }

    void Points<nlohmann::json>::insert(const value_type& point)
    {
        auto sql = sqlPool.acquire_wait();
        *sql    << "insert into Point(id, Model_id, type, u, d) values(:PointId, :ModelId, :type, :u, :d)",
                use(point);
    }

    void Points<nlohmann::json>::remove(const std::string& pointId,
                                        const std::string& modelId)
    {
        lock_guard<mutex> guard(point_mutex);

        pointTable.
        remove().
        where("id = :PointId AND Model_id = :ModelId").
        bind(ValueMap{
            {"PointId", pointId.c_str()},
            {"ModelId", modelId.c_str()}
        }).
        execute();
    }

    void Points<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
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

        // remove Point from DB.
        this->remove(data["PointId"], data["ModelId"]);

        // close
        session->close(restbed::OK);
    }
}
}

// --- SERIALIZERS -------------------
namespace mysqlx
{
    using Point = celeste::resource::Point;

    void row_serializer<Point>::to_row (SerializableRow& row, const Point& obj)
    {
        row.set(0, EnhancedValue{obj.PointId});
        row.set(1, EnhancedValue{obj.ModelId});
        row.set(2, obj.type);
        row.set(3, EnhancedValue{obj.u});
        row.set(4, EnhancedValue{obj.d});
    }

    void row_serializer<Point>::from_row (const SerializableRow& row, Point& obj)
    {
        SerializableRow tmp = row; // row.get() is not marked const, hence we need this tmp
        obj = Point {
            .PointId = tmp.get(0).get<std::string>(),
            .ModelId = tmp.get(1).get<std::string>(),
            .type = tmp.get(2),
            .u = tmp.get(3),
            .d = tmp.get(4)
        };
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
        obj = Point {
            .PointId = j.at("PointId"),
            .ModelId = j.at("ModelId"),
            .type = j.at("type"),
            .u = j.at("u"),
            .d = j.at("d")
        };
    }
}