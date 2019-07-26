if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
endif ()

cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(STRINGS "VERSION.md" VERSION_NB)
project(MMBot
        VERSION ${VERSION_NB}
        DESCRIPTION "An automated LP bot for the Komodo AtomicDEX API"
        LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(mmbot)
init_mmbot()

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/src/version/version.hpp)
    configure_file(${CMAKE_MODULE_PATH}/version.hpp.in  ${PROJECT_SOURCE_DIR}/src/version/version.hpp @ONLY)
endif()

option(USE_ASAN "Use address sanitizer" OFF)
option(USE_TSAN "Use thread sanitizer" OFF)
option(USE_UBSAN "Use thread sanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage" OFF)

add_subdirectory(cmake/targets)
add_subdirectory(src)
