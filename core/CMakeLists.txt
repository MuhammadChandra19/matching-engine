# Library for core components
add_library(core
    limit.cpp
    order.cpp
    match.cpp
    order_book.cpp
)

# Add include directory for public headers
target_include_directories(core PUBLIC include)

# Link required libraries (if any)
# target_link_libraries(core ...)
