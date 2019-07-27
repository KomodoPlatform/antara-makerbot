add_library(error_settings INTERFACE)

# Using namespaces causes CMake to error our in case of typos on the
# consuming side, very important.
add_library(mmbot::error_settings ALIAS error_settings)

target_compile_options(
        error_settings
        INTERFACE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wfatal-errors>
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wfatal-errors>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->)


add_library(defaults_features INTERFACE)
add_library(mmbot::defaults_features ALIAS defaults_features)
target_compile_features(defaults_features INTERFACE cxx_std_17)

add_library(optimize_settings INTERFACE)

add_library(mmbot::optimize_settings ALIAS optimize_settings)

##! Msvc flags info
# /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
# /FS - Allows multiple cl.exe processes to write to the same .pdb file
# /DEBUG - Enable debug during linking
# /Od - Disables optimization
# /Ox - Full optimization
# /Oy- do not suppress frame pointers (recommended for debugging)

target_compile_options(optimize_settings INTERFACE
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-O2 -march=native>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-O2 -march=native -pipe>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:-O0 -g -pipe>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-O0 -g>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Zi /FS /DEBUG /Od /MP /MDd /Oy->
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2 -DNDEBUG /MP>
        )

add_library(default_settings INTERFACE)
add_library(mmbot::default_settings ALIAS default_settings)
target_link_libraries(default_settings INTERFACE mmbot::error_settings mmbot::optimize_settings mmbot::defaults_features)

##! Dependancies
include(FetchContent)

FetchContent_Declare(
        doctest
        URL https://github.com/onqtam/doctest/archive/2.3.3.tar.gz
)

FetchContent_Declare(
        doom_st
        URL https://github.com/doom/strong_type/archive/1.0.1.tar.gz
)

SET(JSON_MultipleHeaders ON CACHE BOOL "Override option" FORCE)
SET(JSON_BuildTests OFF CACHE BOOL "Override option" FORCE)

FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/archive/v3.6.1.tar.gz
)

FetchContent_Declare(
        loguru
        URL https://github.com/emilk/loguru/archive/master.zip
)

FetchContent_MakeAvailable(doctest doom_st nlohmann_json loguru)
add_library(logurulog INTERFACE)
target_sources(logurulog INTERFACE ${loguru_SOURCE_DIR}/loguru.cpp)
target_include_directories(logurulog INTERFACE ${loguru_SOURCE_DIR})
add_library(mmbot::log ALIAS logurulog)
