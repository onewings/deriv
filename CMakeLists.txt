project(hire_me)
cmake_minimum_required(VERSION 3.23)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake_modules
)

if (NOT CMAKE_BUILD_TYPE)
    message("Build type not defined. Using 'Release'")
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BIN_FOLDER ${CMAKE_BINARY_DIR}/Release)
    add_definitions(-DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BIN_FOLDER ${CMAKE_BINARY_DIR}/Debug)
else()
    message(FATAL_ERROR "Invalid build type specified: " ${CMAKE_BUILD_TYPE})
endif()

set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_FOLDER})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_FOLDER})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_FOLDER})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra -Wdeprecated-declarations -fpermissive -Wno-sign-compare -Wmissing-declarations -Wwrite-strings -fno-strict-aliasing")



set (WEBSOCKETPP_BOOST_LIBS system thread)

set (Boost_FIND_REQUIRED TRUE)
set (Boost_FIND_QUIETLY TRUE)
set (Boost_DEBUG FALSE)
set (Boost_USE_MULTITHREADED TRUE)
set (Boost_ADDITIONAL_VERSIONS "1.39.0" "1.40.0" "1.41.0" "1.42.0" "1.43.0" "1.44.0" "1.46.1") # todo: someone who knows better spesify these!

find_package (Boost 1.39.0 COMPONENTS "${WEBSOCKETPP_BOOST_LIBS}")

if (Boost_FOUND)
    # Boost is a project wide global dependency.
    include_directories (${Boost_INCLUDE_DIRS})
    link_directories (${Boost_LIBRARY_DIRS})

    # Pretty print status
    message (STATUS "-- Include Directories")
    foreach (include_dir ${Boost_INCLUDE_DIRS})
        message (STATUS "       " ${include_dir})
    endforeach ()
    message (STATUS "-- Library Directories")
    foreach (library_dir ${Boost_LIBRARY_DIRS})
        message (STATUS "       " ${library_dir})
    endforeach ()
    message (STATUS "-- Libraries")
    foreach (boost_lib ${Boost_LIBRARIES})
        message (STATUS "       " ${boost_lib})
    endforeach ()
    message ("")
else ()
    message (FATAL_ERROR "Failed to find required dependency: boost")
endif ()

find_package(OpenSSL)
find_package(ZLIB)


message("Building websocketpp")
include(cmake_modules/external_webscoketpp.cmake) 

set(WEBSOCKET_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/websocketpp )

include_directories(${WEBSOCKET_INCLUDE_DIRS})

set(TARGET_NAME deriv_test)
add_executable (${TARGET_NAME} main.cpp)

target_include_directories(${TARGET_NAME} PUBLIC
    ${WEBSOCKET_INCLUDE_DIRS}
)

target_link_libraries (${TARGET_NAME} ${Boost_LIBRARIES})
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

target_link_libraries (${TARGET_NAME} ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR})
