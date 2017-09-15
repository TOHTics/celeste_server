//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/13/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief No description available.
 */
//</editor-fold>
#ifndef SERVER_DB_UTIL_HPP
#define SERVER_DB_UTIL_HPP

#include <string>
#include <vector>

std::string as_comma_list(std::vector<std::string> v);
std::string quote(std::string str, char q);

#endif