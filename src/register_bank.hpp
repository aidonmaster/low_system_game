// #pragma

// #include <iostream>
// #include <array>

// // Número de "registradores virtuais"
// constexpr size_t NUM_REGISTERS = 40;

// // // Tipo de dado que vamos manipular
// using Data = uint32_t;

// class RegisterBank {
//     public:
//         RegisterBank() {
//             registers.fill(0);
//         }
    
//         void set(size_t index, Data value) {
//             if (index < NUM_REGISTERS) registers[index] = value;
//         }
    
//         Data get(size_t index) const {
//             if (index < NUM_REGISTERS) return registers[index];
//             return 0;
//         }
    
//         void add(size_t index, Data value) {
//             if (index < NUM_REGISTERS) registers[index] += value;
//         }
    
//         void debug() const {
//             for (size_t i = 0; i < NUM_REGISTERS; ++i) {
//                 std::cout << "R[" << i << "] = " << registers[i] << "\n";
//             }
//         }
    
//     private:
//         std::array<Data, NUM_REGISTERS> registers;
// };


#pragma once
#include <array>
// #include <cstdint>
#include <iostream>

constexpr size_t NUM_REGISTERS = 40;
using Data = uint32_t;

class RegisterBank {
public:
    RegisterBank() {
        registers.fill(0);
    }

    void set(size_t index, Data value) {
        if (index < NUM_REGISTERS) registers[index] = value;
    }

    Data get(size_t index) const {
        if (index < NUM_REGISTERS) return registers[index];
        return 0;
    }

    void add(size_t index, Data value) {
        if (index < NUM_REGISTERS) registers[index] += value;
    }

    // Debug
    void debug() const {
        for (size_t i = 0; i < NUM_REGISTERS; ++i) {
            std::cout << "R[" << i << "] = " << registers[i] << "\n";
        }
    }

private:
    std::array<Data, NUM_REGISTERS> registers;
};
