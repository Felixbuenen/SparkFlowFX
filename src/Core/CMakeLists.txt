set(CORE_SOURCES
    core.cpp
    renderer.cpp
    renderer.h
    simulation.cpp
    simulation.h
    vec3.h
)

add_library(Core STATIC ${CORE_SOURCES})

add_subdirectory(vendor/glad)
target_link_libraries(Core PRIVATE glad)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)