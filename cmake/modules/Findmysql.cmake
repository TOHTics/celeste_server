# ---- Find JSON ----------------------------
message("${yellow}Looking for MySQL...${reset}")
find_path(mysql_INCLUDE mysql.h 
          PATHS 
          "${PROJECT_SOURCE_DIR}/dependency/mysql/"
          "/usr/include"
          "/usr/local/include"
          "/opt/local/include"
          "/opt/local/include/mysql57/mysql/"
          "/opt/local/include/mysql/"
          "/usr/local/include/mysql57/mysql/"
          "/usr/local/include/mysql/"
          )

# ---- Verify we found the dependency ----------
if (mysql_INCLUDE)
    message(STATUS "${green}Found MySQL include files at: ${mysql_INCLUDE}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate MySQL include files.${reset}")    
endif()


if (mysql_INCLUDE)
    set(JSON_FOUND TRUE)
endif()
