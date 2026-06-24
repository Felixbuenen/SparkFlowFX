file(GLOB_RECURSE APP_SOURCES
    CONFIGURE_DEPENDS
    "*.cpp"
)

add_executable(App
    ${APP_SOURCES}
)

target_link_libraries(App
    PRIVATE
        Core
)

target_include_directories(App
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)