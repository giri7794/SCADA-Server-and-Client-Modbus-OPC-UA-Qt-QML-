// scada_server.cpp
#include <iostream>
#include <signal.h>
#include <thread>
#include <mutex>
#include <modbus/modbus.h>
#include <open62541/server_config_default.h>
#include <open62541/server.h>

std::thread updateThread;
static volatile UA_Boolean serverRunning = true;

struct ScadaData {
    uint16_t analog[2] = {100, 200};
    uint8_t digital[2] = {1, 0};
    std::mutex mtx;
};

ScadaData scada;
std::string analogNames[2] = {"Analog0", "Analog1"};
std::string digitalNames[2] = {"Digital0", "Digital1"};

void stopHandler(int sig) {
    std::cout << "\nCaught signal " << sig << ", stopping..." << std::endl;
    serverRunning = false;
}

// ------------------------ OPC UA Server ------------------------
void run_opcua_server() {
    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(config);  // ✅ This binds to 0.0.0.0:4840 automatically

    // Add analog variables
    for (int i = 0; i < 2; ++i) {
        UA_VariableAttributes attr = UA_VariableAttributes_default;
        UA_Int16 value = scada.analog[i];

        UA_Variant_setScalarCopy(&attr.value, &value, &UA_TYPES[UA_TYPES_INT16]);
        attr.displayName = UA_LOCALIZEDTEXT(const_cast<char*>("en-US"), const_cast<char*>(analogNames[i].c_str()));

        UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char*>(analogNames[i].c_str()));
        UA_QualifiedName varName = UA_QUALIFIEDNAME(1, const_cast<char*>(analogNames[i].c_str()));
        UA_Server_addVariableNode(server, nodeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                  varName,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  attr, nullptr, nullptr);
    }

    // Add digital variables
    for (int i = 0; i < 2; ++i) {
        UA_VariableAttributes attr = UA_VariableAttributes_default;
        UA_Boolean value = scada.digital[i];

        UA_Variant_setScalarCopy(&attr.value, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);
        attr.displayName = UA_LOCALIZEDTEXT(const_cast<char*>("en-US"), const_cast<char*>(digitalNames[i].c_str()));

        UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char*>(digitalNames[i].c_str()));
        UA_QualifiedName varName = UA_QUALIFIEDNAME(1, const_cast<char*>(digitalNames[i].c_str()));
        UA_Server_addVariableNode(server, nodeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                  varName,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  attr, nullptr, nullptr);
    }

    // Update values periodically
    updateThread = std::thread([server]() {
		while (serverRunning) {
			std::lock_guard<std::mutex> lock(scada.mtx);
			for (int i = 0; i < 2; ++i) {
				UA_Variant val;

				UA_Int16 a = scada.analog[i];
				UA_Variant_setScalar(&val, &a, &UA_TYPES[UA_TYPES_INT16]);
				UA_Server_writeValue(server, UA_NODEID_STRING(1, const_cast<char*>(analogNames[i].c_str())), val);

				UA_Boolean d = scada.digital[i];
				UA_Variant_setScalar(&val, &d, &UA_TYPES[UA_TYPES_BOOLEAN]);
				UA_Server_writeValue(server, UA_NODEID_STRING(1, const_cast<char*>(digitalNames[i].c_str())), val);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

    // Run OPC UA server (blocking)
    UA_Server_run(server, &serverRunning);

    // After run exits, wait for thread to finish
    if (updateThread.joinable())
        updateThread.join();
	
    UA_Server_delete(server);
}

// ------------------------ Modbus TCP Server ------------------------
void run_modbus_server() {
    modbus_t *ctx = modbus_new_tcp("0.0.0.0", 1502);
    if (!ctx) {
        std::cerr << "Failed to create Modbus context\n";
        return;
    }

    modbus_mapping_t *mb_map = modbus_mapping_new(0, 0, 4, 0);
    if (!mb_map) {
        std::cerr << "Failed to create Modbus mapping\n";
        modbus_free(ctx);
        return;
    }

    int server_socket = modbus_tcp_listen(ctx, 1);
    if (server_socket == -1) {
        std::cerr << "Failed to listen on Modbus socket\n";
        modbus_mapping_free(mb_map);
        modbus_free(ctx);
        return;
    }

    modbus_tcp_accept(ctx, &server_socket);

    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    while (true) {
        int rc = modbus_receive(ctx, query);
        if (rc > 0) {
            modbus_reply(ctx, query, rc, mb_map);
            std::lock_guard<std::mutex> lock(scada.mtx);
            scada.analog[0] = mb_map->tab_registers[0];
            scada.analog[1] = mb_map->tab_registers[1];
            scada.digital[0] = mb_map->tab_registers[2];
            scada.digital[1] = mb_map->tab_registers[3];
        } else if (rc == -1) {
            std::cerr << "[Modbus] Connection closed or error: " << modbus_strerror(errno) << "\n";
            modbus_tcp_accept(ctx, &server_socket); // Wait for next client
        }
    }

    modbus_mapping_free(mb_map);
    modbus_close(ctx);
    modbus_free(ctx);
}

// ------------------------ Main ------------------------
int main() {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
	
    std::thread t1(run_modbus_server);
    std::thread t2(run_opcua_server);

    t1.join();
    t2.join();

    return 0;
}
