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
FetchContent_Declare(hiredis
        GIT_REPOSITORY https://github.com/redis/hiredis.git
        GIT_TAG v1.2.0)

if(NOT hiredis)
    FetchContent_Populate(hiredis)
    add_subdirectory(${hiredis_SOURCE_DIR} ${hiredis_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


# <------------ add hiredis dependency --------------->
find_path(HIREDIS_HEADER hiredis)
include_directories(${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
#target_link_libraries(target ${HIREDIS_LIB})

# <------------ add redis-plus-plus dependency -------------->
# NOTE: this should be *sw* NOT *redis++*
find_path(REDIS_PLUS_PLUS_HEADER sw)
include_directories(${REDIS_PLUS_PLUS_HEADER})
#target_include_directories(target PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
#target_link_libraries(target ${REDIS_PLUS_PLUS_LIB})

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

