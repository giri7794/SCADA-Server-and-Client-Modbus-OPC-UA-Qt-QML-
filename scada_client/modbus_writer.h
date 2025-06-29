// modbus_writer.h
#pragma once

#include <modbus/modbus.h>
#include <thread>
#include <chrono>

#include "client_data.h"

void modbus_writer(ClientData &data);
