cmake_minimum_required(VERSION 3.20)

project(CryptoExchange VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings and clang-tidy/static analysis
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
option(ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
    endif()
endif()

# Fetch external libraries
include(FetchContent)

enable_testing()

# Add subdirectories
add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(tests)
add_subdirectory(helper)
