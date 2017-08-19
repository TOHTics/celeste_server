//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/18/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file IntegerTranslator.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/18/17.
 * 
 * @brief No description available.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#ifndef SERVER_INTEGERTRANSLATOR_HPP
#define SERVER_INTEGERTRANSLATOR_HPP


#include <string>
#include <boost/property_tree/ptree.hpp>

/**
 * @class
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/18/17.
 * 
 * @brief Translates from `int` to `string` and `string` to `int`
 * 
 * # Description
 * We made this class because we will need to translate between `float` and
 * `string` in our application. Using the `boost::optional` semantics, this
 * class is well abstracted. Please consult the `boost` docs for more infor-
 * mation: http://www.boost.org/doc/libs/1_64_0/libs/optional/doc/html/index.html
 */
struct IntegerTranslator {
    typedef std::string internal_type;
    typedef int         external_type;

    // Converts a string to bool
    boost::optional<external_type> get_value(const internal_type& str)
    {
        int i = std::stoi(str);
        return boost::optional<external_type >(i);
    }

    // Converts a bool to string
    boost::optional<internal_type> put_value(const external_type& i)
    {
        std::string s = std::to_string(i);
        return boost::optional<internal_type>(s);
    }
};


namespace boost {
    namespace property_tree {

        template<typename Ch, typename Traits, typename Alloc>
        struct translator_between<std::basic_string< Ch, Traits, Alloc >, int>
        {
            typedef IntegerTranslator type;
        };

    }
}

#endif //SERVER_INTEGERTRANSLATOR_HPP
