//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/17/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains the class declarations for `ModelData`
 */
//</editor-fold>
#ifndef SUNSPEC_MODELDATA_HPP
#define SUNSPEC_MODELDATA_HPP


#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "PointData.hpp"
#include "sunspec/util/exceptions.hpp"

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
 * > This element defines the SunSpec Model used to understand the following group
 * > of data point records. SunSpec-aware hosts use the model id to ‘look’
 * > up repetitive information such as “What is the unit of measure for data point A,
 * > from the SunSpec Device Model 103?”
 *
 *
 * A model usually consists of 4 attributes:
 * - The model id
 * - The model id namespace
 * - The model record index
 * - The set of data point records
 *
 * TODO
 * Nothing.
 */
struct ModelData
{
    typedef std::vector<PointData> point_list_type; ///< List of points
    typedef point_list_type::iterator iterator;        ///< Iterator wrapper
    typedef point_list_type::const_iterator const_iterator;  ///< Const iterator wrapper

    std::string id;                  ///< Device Model Identifier
    std::string ns = "sunspec";      ///< Namespace under the model is valid. Defaults to `"sunspec"`.
    std::string x;                   ///< Index used in aggregated devices.
    point_list_type points;          ///< List of the point records' data.

    /**
     * Empty constructor.
     */
    ModelData() = default;

    /**
     * Constructs the ModelData with `id`
     * @param id The id of the model. The Device Model Identifier.
     */
    ModelData(std::string id) : id(id)
    { }

    /**
     * Constructs the ModelData with a given point list
     * @param point_list List of points
     */
    ModelData(const point_list_type &point_list) : points(point_list)
    { }

    /**
     * Initializes by reserving `n` spaces for `n` points.
     * Calling this before hand will increase performance when adding new
     * points.
     * @param n Number of spaces to reserve
     */
    ModelData(size_t n);


    /**
     * Copy constructor. Copies the data from `other` over to `this`.
     * @param other The other ModelData.
     */
    ModelData(const ModelData &other) = default;

    /**
     * @param p `PointData` to add to the list of point records of this model.
     */
    void add_point(const PointData &p);

    /**
     * Returns an iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `end()`.
     * @return An iterator to the first `PointData`
     */
    iterator begin();

    /**
     * Returns an iterator to the element following the last element of the container.
     * This element acts as a placeholder; attempting to access it results in undefined
     * behavior.
     * @return Iterator to the element following the last `PointData`.
     */
    iterator end();

    /**
     * Returns a const iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `cend()`.
     * @return An iterator to the first `PointData`
     */
    const_iterator cbegin() const;

    /**
     * Returns a const iterator to the element following the last element of the container.
     * This element acts as a placeholder; attempting to access it results in undefined
     * behavior.
     * @return Iterator to the element following the last `PointData`.
     */
    const_iterator cend() const;

    /**
     * Returns the number of points contained by this object
     * @return Number of points contained
     */
    size_t size();

    /**
     * Builds the ModelData out of a SDX specification. This specification
     * can be found in the SunSpec Data Model Exchange Specification and
     * its syntax is XML.
     * @param model_element The `ptree` object (in XML) element of the model
     * record.
     * @return Returns a `ModelData` object with the same data as the element.
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `model_element` is empty or is malformed.
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
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `model_record` is empty or is malformed.
     */
    static ModelData from_xml(const std::string &model_record);
};
}
}
#endif //SERVER_MODELDATA_HPP
