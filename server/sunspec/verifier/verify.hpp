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
#ifndef SUNSPEC_VERIFY_HPP
#define SUNSPEC_VERIFY_HPP

#include <sunspec/data/data.hpp>

namespace sunspec
{
namespace data
{
    struct verifier
    {
        template <class SunSpecDataObject>
        static bool verify (const SunSpecDataObject& obj);
    };

    template <>
    bool verifier::verify(const DeviceData& obj);
}
}


#endif //SERVER_VERIFY_HPP
