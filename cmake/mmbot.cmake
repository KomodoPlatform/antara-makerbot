macro(init_env)
    include(CheckIPOSupported)
    if (UNIX AND NOT APPLE)
        set(LINUX TRUE)
    endif ()
    if (APPLE)
        ##! For user that's install llvm through brew.
        link_directories("/usr/local/opt/llvm/lib")
    endif ()
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(nephtys_compiler clang)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(nephtys_compiler gcc)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(nephtys_compiler msvc)
    endif ()
endmacro()

macro(target_enable_asan target)
    if (USE_ASAN)
        message("-- ASAN Enabled, Configuring...")
        target_compile_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=address -fno-omit-frame-pointer>)
        target_link_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=address -fno-omit-frame-pointer>)
    endif ()
endmacro()


macro(target_enable_tsan target)
    if (USE_TSAN AND NOT ASAN)
        message("-- TSAN Enabled, Configuring...")
        target_compile_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=thread -fno-omit-frame-pointer>)
        target_link_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=thread -fno-omit-frame-pointer>)
    endif ()
endmacro()

macro(target_enable_ubsan target)
    if (USE_UBSAN AND NOT ASAN)
        message("-- UBSAN Enabled, Configuring...")
        target_compile_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=undefined -fno-omit-frame-pointer>)
        target_link_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=undefined -fno-omit-frame-pointer>)
    endif ()
endmacro()

macro(target_enable_coverage target)
    if (ENABLE_COVERAGE)
        target_compile_options(${target} PUBLIC $<$<CXX_COMPILER_ID:GNU>:--coverage> -fprofile-arcs -ftest-coverage)
        target_link_options(${target} PUBLIC $<$<CXX_COMPILER_ID:GNU>:--coverage> -fprofile-arcs -ftest-coverage)
    endif ()
endmacro()


macro(init_mmbot)
    init_env()
endmacro()
