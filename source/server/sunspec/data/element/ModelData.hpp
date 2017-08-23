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
#include "PointData.hpp"
#include "../error.hpp"


namespace sunspec
{
    namespace data
    {
        using std::string;
        /**
         * @class ModelData
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/17/17.
         *
         * @brief Class describing the data that a model record contains
         *
         * # Description
         * A model usually consists of 4 parts:
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

            string id;                  /// Device Model Identifier
            string ns = "sunspec";      /// Namespace under the model is valid. Defaults to `"sunspec"`.
            string record_index;        /// This optional attribute is used within aggregate devices, where a single physical
                                        /// device may include multiple instances of the same model.
            point_list_type point_list; /// List of the point records' data.

            /**
             * Empty constructor.
             */
            ModelData() = default;

            /**
             * Constructs the ModelData with `this->id=id`.
             * @param id The id of the model. The Device Model Identifier.
             */
            ModelData(string id) : id(id) {}

            /**
             * Constructs the ModelData with `this->id=id` and
             * a `point_list`
             * @param id
             * @param point_list
             */
            ModelData(string id, point_list_type point_list) : id(id), point_list(point_list) {}

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
                    throw PointDataError("PointData's id field must not be empty as "
                                                 "mandated by the SunSpec Model Data"
                                                 " Exchange specification.");
                point_list.push_back(p);
            }
        };

    }
}


#endif //SERVER_MODELDATA_HPP
