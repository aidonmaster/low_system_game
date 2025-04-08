#pragma once

#include <memory>
#include <cstring>
#include <iostream>

constexpr size_t MEMORY_POOL_SIZE = 2 * 1024 * 1024;
using Data = uint32_t;

class MemoryPool {
public:
    MemoryPool() {
        memory = std::make_unique<uint8_t[]>(MEMORY_POOL_SIZE);
        std::memset(memory.get(), 0, MEMORY_POOL_SIZE);
    }

    void write(size_t address, Data value) {
        if (address + sizeof(Data) > MEMORY_POOL_SIZE) {
            std::cout << "WRITE ERROR: " << address << "\n";
            return;
        }
        std::memcpy(memory.get() + address, &value, sizeof(Data));
    }

    Data read(size_t address) const {
        if (address + sizeof(Data) > MEMORY_POOL_SIZE) {
            std::cout << "READ ERROR: " << address << "\n";
            return 0;
        }
        Data value;
        std::memcpy(&value, memory.get() + address, sizeof(Data));
        return value;
    }

private:
    std::unique_ptr<uint8_t[]> memory;
};
