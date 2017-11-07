# ---- Find JSON ----------------------------
message("${yellow}Looking for json...${reset}")
find_path(json_INCLUDE json.hpp 
          PATHS 
          "${PROJECT_SOURCE_DIR}/dependency/json/src"
          "/usr/include"
          "/usr/local/include"
          "/opt/local/include"
          )

# ---- Verify we found the dependency ----------
if (json_INCLUDE)
    message(STATUS "${green}Found JSON include files at: ${json_INCLUDE}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate JSON include files.${reset}")    
endif()


if (json_INCLUDE)
    set(JSON_FOUND TRUE)
endif()
