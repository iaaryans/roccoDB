cmake_minimum_required(VERSION 3.15)
project(roccoDB VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- NEW: Node.js npm Build Pipeline ---
if(DEFINED CMAKE_JS_VERSION)
    # Ask Node.js where the node-addon-api headers are located
    execute_process(COMMAND node -p "require('node-addon-api').include"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE NODE_ADDON_API_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    # Clean up the path and include the headers
    string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
    include_directories(${CMAKE_JS_INC} ${NODE_ADDON_API_DIR})
    
    # Build a native Node module instead of an executable
    add_library(roccodb-native SHARED src/binding.cpp)
    set_target_properties(roccodb-native PROPERTIES PREFIX "" SUFFIX ".node")
    target_link_libraries(roccodb-native ${CMAKE_JS_LIB})
    
    if(WIN32)
        target_link_libraries(roccodb-native ws2_32)
    endif()


else()
    add_executable(roccodb src/main.cpp)
    if(WIN32)
        target_link_libraries(roccodb ws2_32)
    endif()
endif()