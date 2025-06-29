// modbus_writer.cpp
#include "modbus_writer.h"

void modbus_writer(ClientData &data) {
    modbus_t *ctx = modbus_new_tcp("192.168.29.18", 1502);
    if (modbus_connect(ctx) == -1) return;

    uint16_t regs[4];
    int value = 0;
    while (true) {
        {
            std::lock_guard<std::mutex> lock(data.mtx);
            regs[0] = data.analog[0] = value;
            regs[1] = data.analog[1] = value + 10;
            regs[2] = data.digital[0] = value % 2;
            regs[3] = data.digital[1] = (value + 1) % 2;
        }

        modbus_write_registers(ctx, 0, 4, regs);
        value += 5;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    modbus_close(ctx);
    modbus_free(ctx);
}
