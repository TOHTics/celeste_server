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
#ifndef SERVER_DB_MODEL_HPP
#define SERVER_DB_MODEL_HPP

#include <string>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class Model
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 20/Sep/17.
 *
 * @brief No description available.
 *
 * # Description
 * No full length description available.
 */
struct Model
{
    std::string id;
    int index;
    int device_id;
    std::string ns;
};
}
}
}
#endif
