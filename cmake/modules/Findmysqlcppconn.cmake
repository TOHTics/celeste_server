# ---- Find MySQL C++ connector ----------------
message("${yellow}Looking for MySQL C++ connector...${reset}")
find_path(mysqlcppconn_INCLUDE mysql_devapi.h 
          HINTS 
          "${PROJECT_SOURCE_DIR}/dependency/mysql-cpp-connector/include"
          # "/usr/include"
          # "/usr/local/include"
          # "/opt/local/include"
          )

find_library(mysqlcppconn_LIBRARY mysqlcppconn8
              HINTS
              "${PROJECT_SOURCE_DIR}/../build/dependency/mysql-cpp-connector/build"
              "/usr/local/lib"
              "/opt/local/lib"
              )

# ---- Verify we found the dependency ----------
if (mysqlcppconn_INCLUDE)
    message(STATUS "${green}Found MySQL C++ connector include files at: ${mysqlcppconn_INCLUDE}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate MySQL C++ connector include files.${reset}")    
endif()

if (mysqlcppconn_LIBRARY)
    message(STATUS "${green}Found MySQL C++ connector library at: ${mysqlcppconn_LIBRARY}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate MySQL C++ connector library.${reset}")  
endif()


if (mysqlcppconn_INCLUDE AND mysqlcppconn_LIBRARY)
    set(MYSQLPPCONN_FOUND TRUE)
endif()