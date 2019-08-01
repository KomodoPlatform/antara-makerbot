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
        URL https://github.com/doom/strong_type/archive/1.0.2.tar.gz
)

FetchContent_Declare(
        restinio
        URL https://bitbucket.org/sobjectizerteam/restinio/downloads/restinio-0.5.1-full.zip
)

SET(JSON_MultipleHeaders ON CACHE BOOL "Override option" FORCE)
SET(JSON_BuildTests OFF CACHE BOOL "Override option" FORCE)

set(TF_BUILD_EXAMPLES OFF CACHE BOOL "Override option" FORCE)
set(TF_BUILD_TESTS OFF CACHE BOOL "Override option" FORCE)
FetchContent_Declare(
        cpp-taskflow
        URL https://github.com/cpp-taskflow/cpp-taskflow/archive/master.zip
)

FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/archive/v3.6.1.tar.gz
)

FetchContent_Declare(
        loguru
        URL https://github.com/emilk/loguru/archive/master.zip
)

FetchContent_Declare(
        restclient-cpp
        URL https://github.com/mrtazz/restclient-cpp/archive/master.zip
)

FetchContent_Declare(jl777-coins
        URL https://github.com/jl777/coins/archive/master.zip)

FetchContent_MakeAvailable(doctest doom_st nlohmann_json loguru restclient-cpp cpp-taskflow restinio jl777-coins)
find_package(Threads)
add_library(logurulog OBJECT)
target_sources(logurulog PUBLIC ${loguru_SOURCE_DIR}/loguru.cpp)
target_include_directories(logurulog PUBLIC ${loguru_SOURCE_DIR})
if (UNIX)
    target_link_libraries(logurulog PUBLIC Threads::Threads $<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS}>)
endif ()
target_compile_definitions(logurulog PUBLIC $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)

add_library(mmbot::log ALIAS logurulog)

find_package(CURL REQUIRED)
add_library(restclient_obj STATIC)
target_sources(restclient_obj PRIVATE
        ${restclient-cpp_SOURCE_DIR}/source/connection.cc
        ${restclient-cpp_SOURCE_DIR}/source/helpers.cc
        ${restclient-cpp_SOURCE_DIR}/source/restclient.cc)
target_compile_features(restclient_obj PRIVATE cxx_std_11)

target_link_libraries(restclient_obj PUBLIC CURL::libcurl Threads::Threads)
target_include_directories(restclient_obj PUBLIC ${restclient-cpp_SOURCE_DIR}/include ${restclient-cpp_BINARY_DIR}/include)
add_library(mmbot::http ALIAS restclient_obj)


add_subdirectory(${restinio_SOURCE_DIR}/dev/fmt ${restinio_BINARY_DIR}/fmt)
add_subdirectory(${restinio_SOURCE_DIR}/dev/nodejs/http_parser ${restinio_BINARY_DIR}/http_parser)
add_subdirectory(${restinio_SOURCE_DIR}/dev ${restinio_BINARY_DIR})

add_library(restiniofull INTERFACE)
target_link_libraries(restiniofull INTERFACE restinio::restinio)
target_include_directories(restiniofull INTERFACE ${restinio_SOURCE_DIR}/dev/asio/include)
target_compile_definitions(restiniofull INTERFACE $<$<CXX_COMPILER_ID:MSVC>:_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:_WIN32_WINNT=0x0A00> -DASIO_STANDALONE -DASIO_HAS_STD_CHRONO -DASIO_DISABLE_STD_STRING_VIEW)
add_library(mmbot::restinio ALIAS restiniofull)

if (UNIX)
    configure_file(${jl777-coins_SOURCE_DIR}/coins ${CMAKE_BINARY_DIR}/bin/assets/coins.json COPYONLY)
else ()
    configure_file(${jl777-coins_SOURCE_DIR}/coins ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/assets/coins.json COPYONLY)
endif ()