add_library(simulator STATIC
    src/main.cpp
)

target_include_directories(simulator PUBLIC include)