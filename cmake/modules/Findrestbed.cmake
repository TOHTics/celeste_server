# ---- Find Restbed ----------------------------
message("${yellow}Looking for restbed...${reset}")
find_path(restbed_INCLUDE restbed 
          PATHS 
          "${PROJECT_SOURCE_DIR}/dependency/restbed/source"
          "/usr/include"
          "/usr/local/include"
          "/opt/local/include"
          )

find_library(restbed_LIBRARY restbed 
             HINTS
             "${PROJECT_SOURCE_DIR}/build/dependency/restbed"
             "/usr/local/lib"
             "/opt/local/lib"
             )

# ---- Verify we found the dependency ----------
if (restbed_INCLUDE)
    message(STATUS "${green}Found restbed include files at: ${restbed_INCLUDE}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate restbed include files.${reset}")    
endif()

if (restbed_LIBRARY)
   message(STATUS "${green}Found restbed library at: ${restbed_LIBRARY}${reset}")
endif()


if (restbed_LIBRARY)
    set(LIBRESTBED_FOUND TRUE)
endif()

if (restbed_INCLUDE)
    set(RESTBED_FOUND TRUE)
endif()
