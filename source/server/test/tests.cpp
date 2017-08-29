//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file tests.hpp
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
#include <string>
#include <iostream>
#include "tests.hpp"
#include "../sunspec/data/data.hpp"

bool SUNSPEC_DATA_XML_PARSE()
{
        using namespace std;
        using namespace sunspec::data;
        string xml = ""
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<SunSpecData v=\"1\">\n"
            "<d man=\"gsc\" mod=\"gw23\" sn=\"A123456\" t=\"2011-05-12T09:20:50Z\">\n"
            "<m id=\"501\">\n"
            "<p id=\"Stat\">1</p> <p id=\"Evt\">0</p>\n"
            "</m> </d>\n"
            "<d man=\"gsc\" mod=\"md800\" sn=\"Q123456\" t=\"2011-05-12T09:20:50Z\"> <m id=\"502\">\n"
            "<p id=\"Stat\">1</p>\n"
            "<p id=\"Evt\">0</p>\n"
            "<p id=\"Tms\">123456789</p> <p id=\"OutW\">678</p>\n"
            "<p id=\"Tmp\">48.3</p>\n"
            "</m> </d>\n"
            "<d man=\"gsc\" mod=\"md800\" sn=\"Q234567\" t=\"2011-05-12T09:20:50Z\"> <m id=\"502\">\n"
            "<p id=\"Stat\">1</p>\n"
            "<p id=\"Evt\">0</p>\n"
            "<p id=\"Tms\">123456795</p> <p id=\"OutW\">634</p>\n"
            "<p id=\"Tmp\">49.1</p>\n"
            "</m> </d>\n"
            "</SunSpecData>";

    SunSpecData data;
    data = SunSpecData::from_xml(xml);

    for (auto dev = data.devices.begin(); dev != data.devices.end(); dev++)
    {
        cout << dev->man << ": " << dev->sn << endl;
        for (auto mod = dev->models.begin(); mod != dev->models.end(); mod++)
        {
            cout << mod->id << endl;
            for (auto point = mod->points.begin(); point != mod->points.end(); point++)
            {
                cout << point->id << ": " << point->value << endl;
            }
        }
    }
}
