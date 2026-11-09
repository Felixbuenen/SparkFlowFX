set(SFX_LOG_LEVELS Off Error Warning Info Debug Trace)
set(SFX_LOG_LEVEL "Trace" CACHE STRING "Compile-time log ceiling")
set_property(CACHE SFX_LOG_LEVEL PROPERTY STRINGS ${SFX_LOG_LEVELS})

if(NOT SFX_LOG_LEVEL IN_LIST SFX_LOG_LEVELS)
    message(FATAL_ERROR "SFX_LOG_LEVEL must be one of: ${SFX_LOG_LEVELS}")
endif()

add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)

set(CORE_SOURCES
    core.cpp
    log.cpp
    log.h
    random.h
    simulation.cpp
    simulation.h
    rendering/viewportRenderer.cpp
    rendering/viewportRenderer.h
    rendering/camera.h
    rendering/camera.cpp
)

add_library(Core STATIC ${CORE_SOURCES})

target_compile_features(Core PUBLIC cxx_std_20)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Core
    PUBLIC
        SFX_LOG_LEVEL=${SFX_LOG_LEVEL}
)

target_link_libraries(Core
    PRIVATE
        glad
    PUBLIC
        glm::glm
)