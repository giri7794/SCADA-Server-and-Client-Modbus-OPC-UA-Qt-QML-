cmake_minimum_required(VERSION 3.16)
project(scada_client VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)

# Find libmodbus
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMODBUS REQUIRED libmodbus)

# --- Force language for open62541.c ---
set_source_files_properties(open62541.c PROPERTIES LANGUAGE C)

# Add OPC UA static library (force compilation of .c)
add_library(open62541_lib STATIC open62541.c)
target_include_directories(open62541_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the main executable
qt_add_executable(appscada_client
    main.cpp
    client_data.h
    modbus_writer.cpp
    modbus_writer.h
    opcua_reader.cpp
    opcua_reader.h
    scada_viewmodel.cpp
    scada_viewmodel.h
    open62541.h
)

# QML module
qt_add_qml_module(appscada_client
    URI scada_client
    VERSION 1.0
    QML_FILES qml/Main.qml
)

# Define WIN32 if not already
target_compile_definitions(appscada_client PRIVATE UA_ARCHITECTURE_WIN32 _WIN32)

# Include dirs
target_include_directories(appscada_client PRIVATE
    ${LIBMODBUS_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(appscada_client
    PRIVATE
        Qt6::Quick
        ${LIBMODBUS_LIBRARIES}
        open62541_lib
)

# Windows-only system libs needed by open62541
if (WIN32)
    target_link_libraries(appscada_client
        PRIVATE
            ws2_32
            iphlpapi
    )
endif()

# Executable properties
set_target_properties(appscada_client PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install
install(TARGETS appscada_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
