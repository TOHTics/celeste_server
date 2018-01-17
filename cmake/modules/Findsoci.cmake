# ---- Find SOCI ----------------
message("${yellow}Looking for SOCI...${reset}")
find_path(soci_INCLUDE soci.h
          PATHS 
          "${PROJECT_SOURCE_DIR}/dependency/soci/include/soci"
          "/usr/include"
          "/usr/local/include"
          "/opt/local/include"
          )

find_path(soci_LIBRARIES soci_core
            HINTS
            "${PROJECT_SOURCE_DIR}/build/dependency/soci/lib"
            "${PROJECT_SOURCE_DIR}/build/lib"
            "/usr/local/lib"
            "/opt/local/lib"
            )

find_path(soci_CONFIG_INCLUDE soci-config.h
             HINTS
             "${PROJECT_SOURCE_DIR}/build/dependency/soci/include/soci"
             "${PROJECT_SOURCE_DIR}/soci/"
             "/usr/local/lib"
             "/opt/local/lib"
             )

find_library(soci_mysql_LIBRARY soci_mysql
             HINTS
             "${PROJECT_SOURCE_DIR}/build/dependency/soci/lib"
             "${PROJECT_SOURCE_DIR}/build/lib"
             "/usr/local/lib"
             "/opt/local/lib"
             )

# ---- Verify we found the dependency ----------
if (soci_INCLUDE)
    message(STATUS "${green}Found SOCI include files at: ${soci_INCLUDE}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate SOCI include files.${reset}")    
endif()

if (soci_LIBRARY)
   message(STATUS "${green}Found SOCI library at: ${soci_LIBRARY}${reset}")
   set(LIBSOCICORE_FOUND TRUE)
endif()

if (soci_INCLUDE)
    set(SOCICORE_FOUND TRUE)
endif()

# ---- MySQL -----------------------------------
if (soci_mysql_LIBRARY)
   message(STATUS "${green}Found SOCI library at: ${soci_mysql_LIBRARY}${reset}")
   set(LIBSOCICORE_FOUND TRUE)
endif()
