set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)

set(APP_SOURCES
    main.cpp
    UI/mainwindow.cpp
    UI/mainwindow.h
    UI/mainwindow.ui
)

add_executable(App ${APP_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${APP_SOURCES})

target_link_libraries(App
    PRIVATE
        Qt6::Widgets
        Qt6::OpenGLWidgets
        Core
)

target_include_directories(App
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/UI
)

add_custom_command(TARGET App POST_BUILD
    COMMAND Qt6::windeployqt
        --no-translations
        "$<TARGET_FILE:App>"
    COMMENT "Deploying Qt runtime dependencies"
)