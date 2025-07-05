cmake_minimum_required(VERSION 3.16)

project(qtonly_scada_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick SerialBus OpcUa)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appqtonly_scada_client
    main.cpp
    scadabackend.cpp
    scadabackend.h
)

qt_add_qml_module(appqtonly_scada_client
    URI qtonly_scada_client
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(appqtonly_scada_client
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::SerialBus
        Qt6::OpcUa
)

include(GNUInstallDirs)
install(TARGETS appqtonly_scada_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
