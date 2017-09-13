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
#ifndef SERVER_VERIFY_HPP
#define SERVER_VERIFY_HPP

#include <sunspec/data/data.hpp>

namespace sunspec
{
namespace data
{
namespace verifier
{
/**
 * Verifies that the `PointData` meets all the rules and it's
 * clear for manipulation. It will also check that the point
 * exists in the database.
 * @param point The `PointData` to be analyzed.
 */
void verify_point( const PointData &point );

void verify_model( const ModelData &model );

void verify_device( const DeviceData &data );
}
}
}


#endif //SERVER_VERIFY_HPP
