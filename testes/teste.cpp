#include <iostream>


#include "memory_pool.hpp"
#include "register_bank.hpp"


int main() {    
    
    MemoryPool mem;
    RegisterBank regs;

    // Escreve valor na memória
    mem.write(100, 42);
    
    // Carrega da memória para registrador 0
    regs.set(0, mem.read(100));
    
    // Soma 10 no registrador 0
    regs.add(0, 10);
    
    // Salva o resultado de volta na memória
    mem.write(100, regs.get(0));
    
    // Debug
    regs.debug();
    
    std::cout << "Valor final na memoria[100]: " << mem.read(100) << "\n";

    return 0;
}