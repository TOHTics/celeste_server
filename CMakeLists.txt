# ---- Set cmake minimum version ---------------
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

# ---- Set project details ---------------------
project(celeste VERSION 0.0.1 LANGUAGES CXX)

# ---- Build options ---------------------------
option(BUILD_CLIENT     "Builds the Celeste Client"    OFF)
option(BUILD_SERVER     "Builds the Celeste Server"    ON )

# ---- Configuration ---------------------------
include("${PROJECT_SOURCE_DIR}/cmake/configuration.cmake")

# ---- Print copyright message -----------------
message("${blue} ------------------------------------------------------- ${reset}")
message("${blue} --- Copyright (C) TOH Tics 2017 All rights reserved --- ${reset}")
message("${blue} ------------------------------------------------------- ${reset}")

# ---- Build -----------------------------------
message("${blue}Generating makefiles...${reset}")
if (BUILD_SERVER)
    add_subdirectory("${PROJECT_SOURCE_DIR}/server")
endif()

if (BUILD_CLIENT)
    add_subdirectory("${PROJECT_SOURCE_DIR}/client")
endif()