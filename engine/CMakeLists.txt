add_executable(matching-engine
        main.cpp
        trade_consumer.cpp
        engine.cpp
        redis_snapshot_store.cpp
        order_reader.cpp
)

# Add include directory for public headers
target_include_directories(matching-engine PUBLIC include)
target_link_libraries(matching-engine PRIVATE
        core
        helper
        modern-cpp-kafka-api
        hiredis
        redis++
        nlohmann_json::nlohmann_json
)
