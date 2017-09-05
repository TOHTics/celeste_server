//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains class definitions for `DeviceResult`
 */
//</editor-fold>
#include <boost/property_tree/xml_parser.hpp>
#include "DeviceResult.hpp"
#include "sunspec/util/sdx_tags.hpp"

using namespace boost::property_tree;

namespace
{
    void add_attribute(std::string attr, std::string tag, ptree& tree)
    {
        if (! attr.empty())
            tree.put("<xmlattr>." + tag, attr);
    }

}

namespace sunspec
{
    namespace data
    {
        std::string DeviceResult::to_xml(const DeviceResult &dresult,
                                         std::shared_ptr<boost::property_tree::ptree> ptOut)
        {
            // Body of response
            ptree body;

            // Put response message
            if (! dresult.message.empty())
                body.put(sdx::SDX_DRESULT_MESSAGE, dresult.message);

            // Put dresult code
            if (! dresult.code.empty())
                body.put(sdx::SDX_DRESULT_CODE, dresult.code);

            // Put dresult reason
            if (! dresult.reason.empty())
                body.put(sdx::SDX_DRESULT_REASON, dresult.reason);

            // Add device data
            add_attribute(dresult.devData.id, sdx::SDX_DEVICE_ID, body);
            add_attribute(dresult.devData.sn, sdx::SDX_DEVICE_SN, body);
            add_attribute(dresult.devData.ns, sdx::SDX_DEVICE_NS, body);
            add_attribute(dresult.devData.mod, sdx::SDX_DEVICE_MOD, body);
            add_attribute(dresult.devData.man, sdx::SDX_DEVICE_MAN, body);
            add_attribute(dresult.devData.cid, sdx::SDX_DEVICE_CID, body);
            add_attribute(dresult.devData.ifc, sdx::SDX_DEVICE_IFC, body);
            add_attribute(dresult.devData.t, sdx::SDX_DEVICE_TIME, body);
            add_attribute(dresult.devData.lid, sdx::SDX_DEVICE_LID, body);

            // XML response
            ptree xml;
            // Put body
            xml.put_child(sdx::SDX_DRESULT, body);

            // Write XML to stream
            std::ostringstream oss;
            write_xml(oss, xml);

            // Return the ptree if necessary
            if (ptOut != nullptr)
            {
                *ptOut = xml;
            }
            return oss.str();
        }
    }
}


