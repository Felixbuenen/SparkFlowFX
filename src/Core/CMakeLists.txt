file(GLOB_RECURSE CORE_SOURCES
    CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
)

add_library(Core STATIC ${CORE_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SOURCES})

add_subdirectory(vendor/glad)
target_link_libraries(Core PRIVATE glad)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)