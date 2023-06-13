cmake_minimum_required(VERSION 3.5)

project(3D_Viewer VERSION 0.1 LANGUAGES CXX)
project(3D_Viewer VERSION 0.1 LANGUAGES C)
#project(LANGUAGES C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(GIFCreation)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)
# find_package(../QtGifImage REQUIRED)

# include_directories(${QTGIFIMAGE_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
        viewerwindow.cpp
        viewerwindow.h
        viewerwindow.ui
        myopenglwidget.cpp
        myopenglwidget.h
        ../Backend/Sourses/parser.c
        ../Backend/Sourses/affinity_transformations.c
        ../Backend/Headers/parser.h
        ../Backend/Headers/affinity_transformations.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3D_Viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 3D_Viewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3D_Viewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3D_Viewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(3D_Viewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::OpenGLWidgets GIFLIB)
#target_link_libraries(3D_Viewer PRIVATE Qt6::OpenGLWidgets)

set_target_properties(3D_Viewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3D_Viewer)
endif()
