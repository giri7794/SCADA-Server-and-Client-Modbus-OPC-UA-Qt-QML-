// opcua_reader.h
#pragma once

#include <thread>
#include <chrono>
#include <string>    // For std::string and std::to_string

extern "C" {
#include "open62541.h"
}

#include "client_data.h"

void opcua_reader(ClientData &data);
