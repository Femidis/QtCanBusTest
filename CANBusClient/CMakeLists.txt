cmake_minimum_required(VERSION 3.16)

project(CANBusClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick SerialBus)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appCANBusClient
    main.cpp
)

qt_add_qml_module(appCANBusClient
    URI CANBus
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES canIClient.h canIClient.cpp
        SOURCES
        SOURCES targetsmodel.h targetsmodel.cpp

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCANBusClient PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCANBus
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCANBusClient
    PRIVATE Qt6::Quick
    PRIVATE Qt6::SerialBus
    PRIVATE CanLib
)

message(${CMAKE_INSTALL_PREFIX})

include(GNUInstallDirs)
install(TARGETS appCANBusClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
