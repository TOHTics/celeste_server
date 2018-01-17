/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef CELESTE_SERVICES_READING_FETCHER
#define CELESTE_SERVICES_READING_FETCHER

#include <memory>
#include <restbed>
#include <soci.h>
#include <boost/variant.hpp>
#include <vector>

#include "Reading.hpp"
#include "ReadRequest.hpp"

#include "srv/service/common.hpp"

namespace celeste
{
namespace resource
{
    class ReadingFetcher
    {
    public:
        // --- CONSTRUCTORS ----------
        ReadingFetcher() = default;
        ReadingFetcher(const std::string& dbSettings);
        ReadingFetcher(const ReadingFetcher& other);
        ReadingFetcher(ReadingFetcher&& other) = delete;
        
        // --- OPERATORS -------------
        ReadingFetcher& operator=(const ReadingFetcher&) = delete;
        ReadingFetcher& operator=(ReadingFetcher&&) = delete;

        // --- PUBLIC METHODS --------
        template <class Response, class Request>
        Response fetch(Request&&) const;

    private:
        // --- PRIVATE MEMBERS -------
        mutable soci::session   sql;
        std::string             dbSettings;
    };

    // --- DEFAULT SPECIALIZATIONS ---
    template <>
    Reading ReadingFetcher::fetch(const LastReadRequest&)
    const;

    template <>
    std::vector<Reading> ReadingFetcher::fetch(const RangeReadRequest&)
    const;

    template <>
    std::map<std::string, double> ReadingFetcher::fetch(const AccumulatedReadRequest&)
    const;
}
}

#endif
