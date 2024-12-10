# Include FetchContent module
include(FetchContent)
FetchContent_Declare(
        modern-cpp-kafka
        GIT_REPOSITORY https://github.com/morganstanley/modern-cpp-kafka.git
        GIT_TAG v2024.07.03
)
FetchContent_MakeAvailable(modern-cpp-kafka)

#add_library(matching-engine
#        trade_consumer.cpp
#)

add_executable(matching-engine
        main.cpp
        trade_consumer.cpp
)

# Add include directory for public headers
target_include_directories(matching-engine PUBLIC include)

target_link_libraries(matching-engine PRIVATE core kafka)
