//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file SunSpecDataResponse.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/29/17.
 * 
 * @brief No description available.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "SunSpecDataResponse.hpp"
#include "../util/SDX_Tags.hpp"

namespace sunspec
{
    namespace data
    {
        std::string SunSpecDataResponse::to_xml(const SunSpecDataResponse &response)
        {
            using boost::property_tree::ptree;

            ptree xml_tree;
            xml_tree.put(sdx::SDX_MESSAGE, this->message);


            return std::string();
        }
    }
}