# Library for core components
add_library(core
    limit.cpp
    order.cpp
    match.cpp
    order_book.cpp
        matching_service.cpp
        log.cpp
)

# Add include directory for public headers
target_include_directories(core PUBLIC include)

# Link required libraries (if any)
# target_link_libraries(core ...)
target_link_libraries(core PRIVATE
        helper
        nlohmann_json::nlohmann_json
)
