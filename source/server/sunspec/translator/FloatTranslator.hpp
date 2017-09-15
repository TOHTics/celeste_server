//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/18/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file FloatTranslator.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/18/17.
 * 
 * @brief Contains the class definitions for FloatTranslator
 * and also puts the translator into the boost::property_tree namespace.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#ifndef SUNSPEC_FLOATTRANSLATOR_HPP
#define SUNSPEC_FLOATTRANSLATOR_HPP


#include <boost/property_tree/ptree.hpp>
#include <string>

/**
 * @class FloatTranslator
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/18/17.
 * 
 * @brief Translates from `string` to `float` and `float` to `string`.
 *
 * # Description
 * We made this class because we will need to translate between `float` and
 * `string` in our application. Using the `boost::optional` semantics, this
 * class is well abstracted. Please consult the `boost` docs for more infor-
 * mation: http://www.boost.org/doc/libs/1_64_0/libs/optional/doc/html/index.html
 */
struct FloatTranslator
{
    typedef std::string internal_type;
    typedef int external_type;

    // Converts a string to float
    boost::optional<external_type> get_value( const internal_type &str )
    {
        float f = std::stof(str);
        return boost::optional<external_type>(f);
    }

    // Converts a float to string
    boost::optional<internal_type> put_value( const external_type &f )
    {
        std::string s = std::to_string(f);
        return boost::optional<internal_type>(s);
    }
};

// Puts our translator in the boost::property_tree namespace.
namespace boost
{
namespace property_tree
{

template < typename Ch, typename Traits, typename Alloc >
struct translator_between<std::basic_string<Ch, Traits, Alloc>, float>
{
    typedef FloatTranslator type;
};

}
}


#endif //SERVER_FLOATTRANSLATOR_HPP
