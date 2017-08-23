//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/17/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file ModelData.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 * 
 * @brief No description available.
 *
 */
//</editor-fold>
#ifndef SERVER_MODELDATA_HPP
#define SERVER_MODELDATA_HPP


#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "PointData.hpp"
#include "../error.hpp"

namespace sunspec
{
    namespace data
    {
        /**
         * @class ModelData
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/17/17.
         *
         * @brief Class describing the data that a model record contains
         *
         * # Description
         * The class is based on the representation of a model record as per
         * the SunSpec Data Exchange Specification (SDX). From the SDX spec:
         *
         * ````
         * This element defines the SunSpec Model used to understand the following group
         * of data point records. SunSpec-aware hosts use the model id to ‘look’
         * up repetitive information such as “What is the unit of measure for data point A,
         * from the SunSpec Device Model 103?”
         * ````
         *
         * A model usually consists of 4 attributes:
         * - The model id
         * - The model id namespace
         * - The model record index
         * - The set of data point records
         *
         * # TODO
         * Nothing for the moment.
         */
        struct ModelData {
            typedef std::vector<PointData> point_list_type;

            std::string id;                  /// Device Model Identifier
            std::string ns = "sunspec";      /// Namespace under the model is valid. Defaults to `"sunspec"`.
            std::string x;                   /// Index used in aggregated devices.
            point_list_type point_list;      /// List of the point records' data.

            /**
             * Empty constructor.
             */
            ModelData() = default;

            /**
             * Constructs the ModelData with `this->id=id`.
             * @param id The id of the model. The Device Model Identifier.
             */
            ModelData(std::string id) : id(id) {}

            /**
             * Constructs the ModelData with `this->id=id` and
             * a `point_list`
             * @param id
             * @param point_list
             */
            ModelData(std::string id, point_list_type point_list) : id(id), point_list(point_list) {}

            /**
             * Copy constructor. Copies the data from `other` over to `this`.
             * @param other The other ModelData.
             */
            ModelData(const ModelData& other) = default;

            /**
             * @throws PointDataError The PointData passed was not formed correctly. Usually
             * meaning that you haven't set the `p.id` field of `PointData p`.
             * @param p `PointData` to add to the list of point records of this model.
             */
            void add_point(const PointData& p)
            {
                if (p.id.empty())
                    throw PointDataError("PointData's id field must not be empty as"
                                                 " mandated by the SunSpec Model Data"
                                                 " Exchange specification.");
                point_list.push_back(p);
            }

            /**
             * Builds the ModelData out of a SDX specification. This specification
             * can be found in the SunSpec Data Model Exchange Specification and
             * its syntax is XML.
             * @param model_element The `ptree` object (in XML) element of the model
             * record.
             * @return Returns a `ModelData` object with the same data as the element.
             */
            static ModelData from_xml(const boost::property_tree::ptree &model_element);

            /**
             * Builds the ModelData out of a SDX specification. This specification
             * can be found in the SunSpec Data Model Exchange Specification and
             * its syntax is XML.
             * @param model_record A `std::string` containing the XML representation
             * of the model record.
             * @return Returns a `ModelData` object with the same data as the XML
             * representation that was passed.
             */
            static ModelData from_xml(const std::string &model_record);
        };
    }
}
#endif //SERVER_MODELDATA_HPP
