
add_library(helper
        string_helper.cpp
        channel.cpp
)

# Add include directory for public headers
target_include_directories(helper PUBLIC include)

