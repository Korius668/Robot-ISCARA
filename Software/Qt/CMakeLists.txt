cmake_minimum_required(VERSION 3.16)
project(iSCARA_Application LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/serialport/iSCARA_Application")

find_package(Qt6 REQUIRED COMPONENTS Core Gui SerialPort Widgets Core5Compat)

qt_standard_project_setup()

qt_add_executable(iSCARA_Application
    serialcommunication.cpp serialcommunication.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui

)

set_target_properties(iSCARA_Application PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(iSCARA_Application PRIVATE
    Qt::Core
    Qt::Gui
    Qt::SerialPort
    Qt::Widgets
    Qt6::Core5Compat
)


install(TARGETS iSCARA_Application
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
