/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_MODELRECORD_HPP
#define SERVER_DB_MODELRECORD_HPP

#include <string>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class   ModelRecord
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 *
 * No full length description available.
 */
struct ModelRecord
{
    int device_id;
    std::string model_id;
    int device_record_idx;
    int model_idx;
};
}
}
}
#endif //SERVER_MODELRECORD_HPP
