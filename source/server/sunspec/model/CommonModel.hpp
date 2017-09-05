//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/30/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief No description available.
 */
//</editor-fold>
#ifndef SERVER_COMMONMODEL_HPP
#define SERVER_COMMONMODEL_HPP


#include <string>

/**
 * @class ${CLASS_NAME}
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/30/17.
 * 
 * @brief Contains class declarations for CommonModel
 * 
 * # Description
 * No full length description available.
 */
struct CommonModel
{
    std::string id;     ///< Model ID
    std::string l;      ///< Length
    std::string mn;     ///< Manufacturer
    std::string md;     ///< Product model
    std::string opt;    ///< Model options
    std::string vr;     ///< Firmware version
    std::string sn;     ///< Serial number
    std::string da;     ///< Modbus Device ID
    std::string pad;    ///< Padding

    /**
     * Initializes a common model.
     * @param id Model ID
     * @param l Length
     * @param mn Manufacturer
     * @param md Product model
     * @param opt Model options
     * @param vr Firmware version
     * @param sn Serial Number
     * @param da Modbus Device ID
     * @param pad Padding
     */
    CommonModel (std::string id = "",
                 std::string l = "",
                 std::string mn = "",
                 std::string md = "",
                 std::string opt = "",
                 std::string vr = "",
                 std::string sn = "",
                 std::string da = "",
                 std::string pad = "") : id(id),
                                         l(l),
                                         mn(mn),
                                         md(md),
                                         opt(opt),
                                         vr(vr),
                                         sn(sn),
                                         da(da),
                                         pad(pad) {}

    /**
     * Virtual destructor
     */
    virtual ~CommonModel() = default;

};


#endif //SERVER_COMMONMODEL_HPP
