#pragma once
#include <mutex>

struct ClientData {
    uint16_t analog[2] = {0};
    uint8_t digital[2] = {0};
    std::mutex mtx;
};
