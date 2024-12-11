add_executable(matching-engine
        main.cpp
        trade_consumer.cpp
)

# Add the Redis++ include directory.
target_include_directories(matching-engine PRIVATE "${redis_plus_plus_SOURCE_DIR}/src")
# Add include directory for public headers
target_include_directories(matching-engine PUBLIC include)
target_include_directories(matching-engine PRIVATE "${redis_plus_plus_SOURCE_DIR}/src")
target_link_libraries(matching-engine PRIVATE
        core
        helper
        modern-cpp-kafka-api
        hiredis-src
        redis-plus-plus-src/src
)
