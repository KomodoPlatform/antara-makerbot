add_library(mmbot_error_settings INTERFACE)

# Using namespaces causes CMake to error our in case of typos on the
# consuming side, very important.
add_library(mmbot::error_settings ALIAS mmbot_error_settings)

target_compile_options(
        mmbot_error_settings
        INTERFACE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wfatal-errors>
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wfatal-errors>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->)


add_library(mmbot_defaults_features INTERFACE)
add_library(mmbot::defaults_features ALIAS mmbot_defaults_features)
target_compile_features(mmbot_defaults_features INTERFACE cxx_std_17)

add_library(mmbot_optimize_settings INTERFACE)

add_library(mmbot::optimize_settings ALIAS mmbot_optimize_settings)

##! Msvc flags info
# /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
# /FS - Allows multiple cl.exe processes to write to the same .pdb file
# /DEBUG - Enable debug during linking
# /Od - Disables optimization
# /Ox - Full optimization
# /Oy- do not suppress frame pointers (recommended for debugging)

target_compile_options(mmbot_optimize_settings INTERFACE
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-O2 -march=native>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-O2 -march=native -pipe>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:-O0 -g -pipe>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-O0 -g>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Zi /FS /DEBUG /Od /MP /MDd /Oy->
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2 -DNDEBUG /MP>
        )

add_library(mmbot_default_settings INTERFACE)
add_library(mmbot::default_settings ALIAS mmbot_default_settings)
target_link_libraries(mmbot_default_settings INTERFACE mmbot::error_settings mmbot::optimize_settings mmbot::defaults_features)

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
        URL https://bitbucket.org/sobjectizerteam/restinio/downloads/restinio-0.5.1.1-full.zip
)

FetchContent_Declare(
        trompeloeil
        URL https://github.com/rollbear/trompeloeil/archive/master.zip
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

set(BUILD_TESTING OFF CACHE BOOL "Override option" FORCE)
set(REPROC++ ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        abseil-cpp
        URL https://github.com/abseil/abseil-cpp/archive/master.zip
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

FetchContent_Declare(bcmath
        URL https://github.com/milerius/BCMath-for-Cpp/archive/master.zip)

FetchContent_Declare(reproc
        URL https://github.com/DaanDeMeyer/reproc/archive/master.zip)

if (APPLE)
    FetchContent_Declare(mm2
            URL http://195.201.0.6/mm2/mm2-latest-Darwin.zip)
elseif (LINUX)
    FetchContent_Declare(mm2
            URL http://195.201.0.6/mm2/mm2-latest-Linux.zip)
else()
    FetchContent_Declare(mm2
            URL http://195.201.0.6/mm2/mm2-latest-Windows_NT.zip)
endif ()

FetchContent_MakeAvailable(doctest doom_st nlohmann_json loguru restclient-cpp cpp-taskflow restinio jl777-coins abseil-cpp bcmath trompeloeil mm2 reproc)
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
    configure_file(${jl777-coins_SOURCE_DIR}/coins ${CMAKE_BINARY_DIR}/bin/assets/coins COPYONLY)
    configure_file(${mm2_SOURCE_DIR}/mm2 ${CMAKE_BINARY_DIR}/bin/assets/mm2 COPYONLY)
    file(COPY ${jl777-coins_SOURCE_DIR}/electrums DESTINATION ${CMAKE_BINARY_DIR}/bin/assets/)
else ()
    configure_file(${jl777-coins_SOURCE_DIR}/coins ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/assets/coins.json COPYONLY)
    configure_file(${jl777-coins_SOURCE_DIR}/coins ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/assets/coins COPYONLY)
    configure_file(${mm2_SOURCE_DIR}/mm2.exe ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/assets/mm2.exe COPYONLY)
    file(COPY ${jl777-coins_SOURCE_DIR}/electrums DESTINATION ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/assets/)
endif ()

add_library(bcmath STATIC)
target_include_directories(bcmath INTERFACE ${bcmath_SOURCE_DIR})
target_sources(bcmath PUBLIC ${bcmath_SOURCE_DIR}/bcmath_stl.cpp)
target_link_libraries(bcmath PUBLIC mmbot::default_settings)
target_compile_definitions(bcmath PUBLIC $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)
add_library(mmbot::bcmath ALIAS bcmath)
