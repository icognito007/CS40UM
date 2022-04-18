#ifndef MEMORY_INSTRUCTION_H
#define MEMORY_INSTRUCTION_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <seq.h>
#include <stack.h>
#include <assert.h>
#include <bitpack.h>
#include "memory.h"
#include "register.h"


void loadProgram(void* registers, void* initialMemory, 
                uint32_t instruction, uint32_t *current, uint32_t *count);
void segLoad( void * registers, void * initialMemory, 
                                        uint32_t instruction);
void segStore( void * registers, void * initialMemory, 
                                        uint32_t instruction);
void mapSeg(void * registers, void* initialMemory, uint32_t instruction);
void unmapSeg(void * registers, void * initialMemory, uint32_t instruction);

#endif