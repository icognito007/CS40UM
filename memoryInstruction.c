#include "memoryInstruction.h"

void loadProgram(void* registers, void* initialMemory, uint32_t instruction,
                            uint32_t *current, uint32_t *count)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t rB = getRegisters(registers, regB);
    uint32_t rC = getRegisters(registers, regC);
    //
    uint32_t index = 0;

    if (rB != 0) {
        copyNReplace(rB, index, initialMemory, count);
    }
    *current = rC;
}

void segLoad( void * registers, void * initialMemory, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t regA = Bitpack_getu(instruction, 3, 6);
    uint32_t rB = getRegisters(registers, regB);
    uint32_t rC = getRegisters(registers, regC);
    uint32_t rBMem = getMem(initialMemory, rB, rC);
    setRegisters(registers, regA, rBMem);
}
void segStore( void * registers, void * initialMemory, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t regA = Bitpack_getu(instruction, 3, 6);
    uint32_t rA = getRegisters(registers, regA);
    uint32_t rB = getRegisters(registers, regB);
    uint32_t rC = getRegisters(registers, regC);
    setMem (initialMemory, rA, rB, rC);
}

void mapSeg(void* registers, void* initialMemory, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t rC = getRegisters(registers, regC);
    uint32_t newSeg = insertSeg(rC, initialMemory); 
    //fprintf(stderr, "index added: %d \n", newSeg);
    setRegisters(registers, regB, newSeg);
}

void unmapSeg(void * registers, void * initialMemory, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t rC = getRegisters(registers, regC);
    //Cannot unmap the instructions segment of memory
    assert(rC != 0);
    delSeg(rC, initialMemory);
}