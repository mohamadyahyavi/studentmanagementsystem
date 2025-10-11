cmake_minimum_required(VERSION 3.16)

project(Studentmanagementsystem VERSION 0.1 LANGUAGES CXX)

# Enable automatic processing for Qt UI, MOC, RCC
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt 6 (Widgets + SQL)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Project sources
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dbmanager.cpp
    dbmanager.h
)

# Create executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Studentmanagementsystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Studentmanagementsystem SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Studentmanagementsystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link Qt libraries (Widgets + SQL)
target_link_libraries(Studentmanagementsystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# macOS / iOS bundle settings
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Studentmanagementsystem)
endif()
set_target_properties(Studentmanagementsystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Installation
include(GNUInstallDirs)
install(TARGETS Studentmanagementsystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Studentmanagementsystem)
endif()
