# ---- Find object_pool ------------------------
message("${yellow}Looking for object_pool...${reset}")
find_path(object_pool_INCLUDE object_pool.hpp 
          PATHS 
          "${PROJECT_SOURCE_DIR}/dependency/object_pool/"
          "/usr/include"
          "/usr/local/include"
          "/opt/local/include"
          )

# ---- Verify we found the dependency ----------
if (object_pool_INCLUDE)
    message(STATUS "${green}Found object_pool include files at: ${object_pool_INCLUDE}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate object_pool include files.${reset}")    
endif()


if (object_pool_INCLUDE)
    set(object_pool_FOUND TRUE)
endif()
