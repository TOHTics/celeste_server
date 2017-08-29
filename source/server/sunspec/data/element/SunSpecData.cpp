//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/23/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file ${HEADER_FILENAME}
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/23/17.
 * 
 * @brief No description available.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#include <boost/property_tree/xml_parser.hpp>
#include "SunSpecData.hpp"
#include "../util/SDX_Tags.hpp"

namespace sunspec
{
    namespace data
    {
        using namespace boost::property_tree;
        using node = std::pair<std::string, ptree>;

        void SunSpecData::add_device(const DeviceData &device)
        {
            devices.push_back(device);
        }

        SunSpecData SunSpecData::from_xml(const boost::property_tree::ptree &ss_element)
        {
            // Verify that there is data
            if ( ss_element.data().empty() )
                throw XMLError("Empty value for ModelData.");

            // Get attributes of the subtree ss_element
            ptree attr = ss_element.get_child("<xmlattr>");
            if ( attr.empty() )
                throw XMLError("Empty attributes for model.");

            // Declare model
            SunSpecData result;

            // Get attributes and set them to the result
            for (const node& n : attr)
            {
                std::string attr_name = n.first;
                std::string attr_data = n.second.data();

                if ( attr_name == sdx::SDX_SUNSPEC_DATA_VERSION )
                {
                    result.v = attr_data;
                }
            }

            // Get devices
            ptree device_elements;
            for ( const node& de : ss_element)
            {
                std::string element_tag = de.first;
                if ( element_tag == sdx::SDX_DEVICE )
                {
                    ptree point_element = de.second;
                    DeviceData d = DeviceData::from_xml(point_element);
                    result.add_device(d);
                }
            }
            return result;
        }

        SunSpecData SunSpecData::from_xml(const std::string &ss_record)
        {
            if ( ss_record.empty() )
                throw XMLError("SunSpec record must be a non-empty string");

            // Parse XML into a ptree
            std::istringstream iss(ss_record);
            ptree xml;
            xml_parser::read_xml<ptree>(iss, xml);

            try
            {
                // Get the child node which represents the model
                xml = xml.get_child(sdx::SDX_SUNSPEC_DATA);
            } catch (ptree_bad_path e)
            {
                throw XMLError("XML Model record does not contain the <m> tag.");
            }

            SunSpecData result = SunSpecData::from_xml(xml);
            return result;
        }
    }
}