/*
*   register.c
*   CS40 Hw #6: UM
*       by:  Muhammad Ali Khan, Harnaljia Fenelon 
*   Date: April 10th, 2022
* 
*   Summary: 
*       Implementation file for register.h. This file deals 
*       with all the register related functions that are not operation
*       functions, which is also helping to keeping the register
*       implementation a secret from other files in our program.
* 
*/
#include "register.h"

/*
function: createRegisters
purpose: creates an array of the eight registers 
input: none
output: returns a void pointer to the array of registers 
*/
void* createRegisters()
{
    int noRegisters = 8;
    uint32_t *registers = (uint32_t*)malloc(sizeof(uint32_t)* noRegisters);
    assert(registers != NULL);
    for (int i = 0; i < noRegisters; i++){
        registers[i] = 0;
    }
    return ((void *) registers); 
}

/*
function: getRegisters
purpose: accesses a specified register and returns its value
input: a void pointer to the array of registers and the
    index of the desired register.
output: returns the value at the register
*/
uint32_t getRegisters(void* registers, uint32_t index) 
{
    validRegister(index);
    uint32_t output = ((uint32_t *) registers)[index];
    return output;
}

/*
function: setRegisters 
purpose: places the given value into the register at the specified index
input: a void pointer to the array of registers, the
    index of the desired register, and a value 
output: none
*/
void setRegisters(void* registers, uint32_t index, uint32_t val) 
{
    validRegister(index);
    ((uint32_t *) registers)[index] = val;
}

/*
function: validRegister 
purpose: checks that the index is associated to a legit register
input: the index of the register
output: none
*/
void validRegister (uint32_t index)
{
    uint32_t noRegisters = 8;
    assert((index < noRegisters));
}

/*
function: freeRegister
purpose: frees the space allocated for the registers
input: a pointer to the array of registers 
output: none
*/
void freeRegister (void *registers)
{
    free(registers);
}