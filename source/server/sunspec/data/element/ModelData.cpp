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
#include "ModelData.hpp"
#include "../error.hpp"
#include "../util/sdx_tags.hpp"

namespace sunspec {
    namespace data {
        using namespace boost::property_tree;
        using node = std::pair<std::string, ptree>;

        ModelData ModelData::from_xml(const ptree &model_element)
        {
            // Verify that there is data
            if ( model_element.data().empty() )
                throw XMLError("Empty value for ModelData.");

            // Get attributes of the subtree model_element
            ptree attr = model_element.get_child("<xmlattr>");
            if ( attr.empty() )
                throw XMLError("Empty attributes for model. At least the id is mandatory.");

            // Declare model
            ModelData result;

            // Get attributes and set them to the result
            for (const node& n : attr)
            {
                std::string attr_name = n.first;
                std::string attr_data = n.second.data();

                if ( attr_name == SDX_MODEL_ID )
                {
                    result.id = attr_data;
                } else if ( attr_name == SDX_MODEL_NAMESPACE )
                {
                    result.ns = attr_data;
                } else if ( attr_name == SDX_MODEL_INDEX )
                {
                    result.x = attr_data;
                }

            }

            if ( result.id.empty() )
                throw XMLError("Blank id for model.");

            // Get points
            ptree point_elements;
            for ( const node& pe : model_element)
            {
                std::string element_tag = pe.first;
                if ( element_tag == SDX_POINT )
                {
                    ptree point_element = pe.second;
                    PointData p = PointData::from_xml(point_element);
                    result.add_point(p);
                }
            }

            return result;
        }

        ModelData ModelData::from_xml(const std::string& model_record)
        {
            // Verify if model record is empty
            if ( model_record.empty() )
                throw XMLError("Model record is empty.");

            // Parse XML into a ptree
            std::istringstream iss(model_record);
            ptree xml;
            xml_parser::read_xml<ptree>(iss, xml);

            try
            {
                // Get the child node which represents the model
                xml = xml.get_child(SDX_MODEL);
            } catch (boost::property_tree::ptree_bad_path e)
            {
                throw XMLError("XML Model record does not contain the <m> tag.");
            }

            // Build PointData
            ModelData result = ModelData::from_xml(xml);

            return result;
        }
    }
}