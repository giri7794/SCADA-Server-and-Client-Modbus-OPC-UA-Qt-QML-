// opcua_reader.cpp
#include "opcua_reader.h"

void opcua_reader(ClientData &data) {
    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    if (UA_Client_connect(client, "opc.tcp://192.168.29.18:4840") != UA_STATUSCODE_GOOD)
        return;

    while (true) {
        for (int i = 0; i < 2; ++i) {
            UA_Variant val;
            UA_Int16 aVal;
            UA_Boolean dVal;

            std::string aName = "Analog" + std::to_string(i);
            UA_Client_readValueAttribute(client, UA_NODEID_STRING(1, (char*)aName.c_str()), &val);
            if (UA_Variant_hasScalarType(&val, &UA_TYPES[UA_TYPES_INT16]))
                aVal = *(UA_Int16*)val.data;

            std::string dName = "Digital" + std::to_string(i);
            UA_Client_readValueAttribute(client, UA_NODEID_STRING(1, (char*)dName.c_str()), &val);
            if (UA_Variant_hasScalarType(&val, &UA_TYPES[UA_TYPES_BOOLEAN]))
                dVal = *(UA_Boolean*)val.data;

            std::lock_guard<std::mutex> lock(data.mtx);
            data.analog[i] = aVal;
            data.digital[i] = dVal;
        }

        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Sleep(500);
    }

    UA_Client_disconnect(client);
    UA_Client_delete(client);
}
