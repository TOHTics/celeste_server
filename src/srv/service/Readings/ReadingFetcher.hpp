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
        
        /**
         * @brief      Fetches a reading based on a request.
         *
         * @param[in]   req Incoming request.
         *
         * @tparam     Response   Representation of response.
         * @tparam     Request    Type of request.
         *
         * @return     The response in its own representation.
         */
        template <class Response, class Request>
        Response fetch(Request&& req) const;

    private:
        // --- PRIVATE MEMBERS -------
        mutable soci::session   m_sql;
        std::string             m_dbSettings;
    };

    // --- DEFAULT SPECIALIZATIONS ---
    template <>
    Reading
    ReadingFetcher::fetch(const LastReadRequest&)
    const;

    template <>
    std::vector<Reading>
    ReadingFetcher::fetch(const RangeReadRequest&)
    const;

    template <>
    std::map<std::string, std::vector<std::pair<int, double>>>
    ReadingFetcher::fetch(const YesterdayReadRequest&)
    const;

    template <>
    std::map<std::string, std::vector<std::pair<int, double>>>
    ReadingFetcher::fetch(const TodayReadRequest&)
    const;

    template <>
    std::map<std::string, double>
    ReadingFetcher::fetch(const AccumulatedReadRequest&)
    const;

    template <>
    std::map<std::string, double>
    ReadingFetcher::fetch(const AverageReadRequest&)
    const;

    template <>
    std::map<std::string, std::vector<std::pair<int, double>>>
    ReadingFetcher::fetch(const DayReadRequest&)
    const;

    template <>
    std::map<std::string, std::vector<std::pair<int, double>>>
    ReadingFetcher::fetch(const MonthReadRequest&)
    const;

    template <>
    std::map<std::string, std::vector<std::pair<int, double>>>
    ReadingFetcher::fetch(const YearReadRequest&)
    const;
}
}

#endif
