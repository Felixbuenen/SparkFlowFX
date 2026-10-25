set(SPARKFLOW_LOG_LEVELS Off Error Warning Info Debug Trace)
set(SPARKFLOW_LOG_LEVEL "Info" CACHE STRING "Compile-time log ceiling")
set_property(CACHE SPARKFLOW_LOG_LEVEL PROPERTY STRINGS ${SPARKFLOW_LOG_LEVELS})

if(NOT SPARKFLOW_LOG_LEVEL IN_LIST SPARKFLOW_LOG_LEVELS)
    message(FATAL_ERROR "SPARKFLOW_LOG_LEVEL must be one of: ${SPARKFLOW_LOG_LEVELS}")
endif()

add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)

set(CORE_SOURCES
    core.cpp
    log.cpp
    log.h
    rand.h
    renderer.cpp
    renderer.h
    simulation.cpp
    simulation.h
)

add_library(Core STATIC ${CORE_SOURCES})

target_compile_features(Core PUBLIC cxx_std_20)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Core
    PUBLIC
        SPARKFLOW_LOG_LEVEL=${SPARKFLOW_LOG_LEVEL}
)

target_link_libraries(Core
    PRIVATE
        glad
    PUBLIC
        glm::glm
)