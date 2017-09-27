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
 * @brief Contains the definition of a Point
 */
//</editor-fold>
#ifndef SERVER_DB_POINT_HPP
#define SERVER_DB_POINT_HPP

#include <string>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class Point
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 20/Sep/17.
 *
 * @brief This describes a Point.
 * 
 * A point is 
 */
struct Point
{
    std::string id;
    std::string model_id;
    std::string type;
    std::string u;
    std::string d;
};
}
}
}
#endif //SERVER_POINT_HPP
