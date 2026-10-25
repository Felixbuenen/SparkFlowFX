# find QT in the most common places, depending on the build system
if(WIN32)
    set(QT_INSTALL_PATTERN "C:/Qt/6.*/msvc*_64")
elseif(APPLE)
    set(QT_INSTALL_PATTERN "$ENV{HOME}/Qt/6.*/macos")
else()
    set(QT_INSTALL_PATTERN "$ENV{HOME}/Qt/6.*/gcc_64")
endif()

file(GLOB QT_INSTALL_CANDIDATES LIST_DIRECTORIES true ${QT_INSTALL_PATTERN})
list(SORT QT_INSTALL_CANDIDATES COMPARE NATURAL ORDER DESCENDING)
list(APPEND CMAKE_PREFIX_PATH ${QT_INSTALL_CANDIDATES})

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)
qt_standard_project_setup()

set(APP_SOURCES
    main.cpp
    UI/mainwindow.cpp
    UI/mainwindow.h
    UI/mainwindow.ui
    UI/viewportwidget.cpp
    UI/viewportwidget.h
)

add_executable(App ${APP_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${APP_SOURCES})

target_include_directories(App
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/UI
)

target_link_libraries(App
    PRIVATE
        Core
        Qt6::Widgets
        Qt6::OpenGLWidgets
)

if(WIN32)
    add_custom_command(TARGET App POST_BUILD
        COMMAND Qt6::windeployqt --no-translations "$<TARGET_FILE:App>"
        COMMENT "Deploying Qt runtime dependencies"
        VERBATIM
    )
endif()