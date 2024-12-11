# Fetch external libraries
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)
FetchContent_Declare(
        modern-cpp-kafka
        GIT_REPOSITORY https://github.com/morganstanley/modern-cpp-kafka.git
        GIT_TAG v2024.07.03
)
FetchContent_MakeAvailable(modern-cpp-kafka)
# Download Hiredis, upon which Redis Plus Plus depends.
FetchContent_Declare(
        hiredis
        GIT_REPOSITORY https://github.com/redis/hiredis
        GIT_TAG        v1.2.0
        SOURCE_DIR     hiredis
)

FetchContent_MakeAvailable(hiredis)
# Set include and library paths for Hiredis
set(HIREDIS_HEADER "${hiredis_SOURCE_DIR}")
set(HIREDIS_LIB "${hiredis_BINARY_DIR}/libhiredis.a")
set(TEST_HIREDIS_LIB "${hiredis_BINARY_DIR}/libhiredis.a")

# Download the Redis binding.
FetchContent_Declare(
        redis_plus_plus
        GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus
        GIT_TAG        1.3.8
)

FetchContent_MakeAvailable(redis_plus_plus)