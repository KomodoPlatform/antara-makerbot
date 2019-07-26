add_library(mmbot_shared_deps INTERFACE)
target_compile_features(mmbot_shared_deps INTERFACE cxx_std_17)
target_include_directories(mmbot_shared_deps INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mmbot_shared_deps INTERFACE mmbot::default_settings)

target_enable_asan(mmbot_shared_deps)
target_enable_tsan(mmbot_shared_deps)
target_enable_ubsan(mmbot_shared_deps)

add_executable(mmbot)
target_sources(mmbot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(mmbot PUBLIC mmbot_shared_deps)

add_executable(mmbot-tests)
target_sources(mmbot-tests PUBLIC mmbot.tests.cpp)
target_link_libraries(mmbot-tests PRIVATE doctest PUBLIC mmbot_shared_deps)
target_enable_coverage(mmbot-tests)

set_target_properties(mmbot-tests mmbot
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
