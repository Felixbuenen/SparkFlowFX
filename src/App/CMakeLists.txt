file(GLOB_RECURSE APP_SOURCES
    CONFIGURE_DEPENDS
    "*.cpp"
)

add_executable(Application
    ${APP_SOURCES}
)

target_link_libraries(Application
    PRIVATE
        Core
)

target_include_directories(Application
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)