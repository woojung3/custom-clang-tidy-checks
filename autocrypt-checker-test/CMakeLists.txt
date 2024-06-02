cmake_minimum_required(VERSION 3.15...3.19)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/module" ${CMAKE_MODULE_PATH})

option(CMAKE_EXPORT_COMPILE_COMMANDS "create compile_commands.json" ON)

MESSAGE(STATUS "==========================================")
MESSAGE(STATUS "AutoCrypt Custom Clang Tidy Checker Test")
MESSAGE(STATUS "==========================================")

project(autocrypt-checker-test LANGUAGES CXX VERSION 0.0.1)

#----------------------------------------------------------
# Compiler config
#----------------------------------------------------------
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

option(ENABLE_CLANG_TIDY "Run clang-tidy automatically as prebuild step" ON)
option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" OFF)
find_program(CLANG_TIDY
    NAMES clang-tidy clang-tidy-7 clang-tidy-8 clang-tidy-9
        clang-tidy-10 clang-tidy-11 clang-tidy-12
    # HINTS /usr/local/opt/llvm/bin/ /usr/local/bin /usr/bin
    HINTS /home/jwlee
)

if(NOT CLANG_TIDY)
    message(FATAL_ERROR "clang-tidy tool not found")
endif()

if(CLANG_TIDY)
    if(ENABLE_CLANG_TIDY)
        message(STATUS "set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})")
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_CMD} CACHE STRING "forced!" FORCE)
    else()
        set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "forced!" FORCE) # delete it
    endif()
endif()

include(ClangTidyConfig)

add_subdirectory_with_static_analysis(cert-cpp)
