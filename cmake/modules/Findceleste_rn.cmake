# ---- Find celeste_rn ----------------------------
message("${yellow}Looking for celeste_rn...${reset}")
find_path(celeste_rn_INCLUDE sunspec.hpp
          PATHS 
          "${PROJECT_SOURCE_DIR}/dependency/celeste_rn/src/"
          "/usr/include"
          "/usr/local/include"
          "/opt/local/include"
          )

find_library(celeste_rn_LIBRARY libceleste_rn.dylib 
             HINTS
             "${PROJECT_SOURCE_DIR}/build/dependency/celeste_rn/"
             "/usr/local/lib"
             "/opt/local/lib"
             )

# ---- Verify we found the dependency ----------
if (celeste_rn_INCLUDE)
    message(STATUS "${green}Found celeste_rn include files at: ${celeste_rn_INCLUDE}${reset}")
else()
    message(FATAL_ERROR "${red}Failed to locate celeste_rn include files.${reset}")    
endif()

if (celeste_rn_LIBRARY)
   message(STATUS "${green}Found celeste_rn library at: ${celeste_rn_LIBRARY}${reset}")
endif()


if (celeste_rn_LIBRARY)
    set(LIBCELESTERN_FOUND TRUE)
endif()

if (celeste_rn_INCLUDE)
    set(CELESTERN_FOUND  TRUE)
endif()
