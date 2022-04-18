/*
*   registerInstruction.c
*   CS40 Hw #6: UM
*       by:  Muhammad Ali Khan, Harnaljia Fenelon 
*   Date: April 10th, 2022
* 
*   Summary: 
*       Implementation file for registerInstruction.h. This file deals 
*       with all the register related operation functions, keeping the 
*       register implementation a secret from other function operations
*       in our program.
* 
*/
#include "registerInstruction.h"

/*
function: move 
purpose: a contditional move that places the value of register B into 
    register A if and only if register C does not equal 0.
input: a void pointer to the registers array and the current instruction. 
output: none 
*/
void move( void * registers, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    if ( getRegisters(registers, regC) != 0){
        uint32_t regB = Bitpack_getu(instruction, 3, 3);
        uint32_t regA = Bitpack_getu(instruction, 3, 6);
        setRegisters(registers, regA, getRegisters(registers, regB));
    }
}

/*
function: addition
purpose: Calculates the sum of registers B & C and places 
    the value into register A 
input: a void pointer to the registers array and the current instruction. 
output: none
*/
void addition( void * registers, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t regA = Bitpack_getu(instruction, 3, 6);
    uint32_t rB = getRegisters(registers, regB);
    uint32_t rC = getRegisters(registers, regC);
    setRegisters(registers, regA, rB + rC); 
}

/*
function: multiplication
purpose: Calculates the product of registers B & C and places 
    the value into register A 
input: a pointer to the registers array and the current instruction
output: none
*/
void multiplication( void * registers, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t regA = Bitpack_getu(instruction, 3, 6);
    uint32_t rB = getRegisters(registers, regB);
    uint32_t rC = getRegisters(registers, regC);
    setRegisters(registers, regA, rB * rC); 
}

/*
function: division
purpose: Calculates the quotient of registers B & C and places 
    the value into register A 
input: a pointer to the registers array and the current instruction
output: none
*/
void division( void * registers, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t regA = Bitpack_getu(instruction, 3, 6);
    uint32_t rB = getRegisters(registers, regB);
    uint32_t rC = getRegisters(registers, regC);
    assert (rC != 0);
    setRegisters(registers, regA, rB / rC); 
}

/*
function: bNAND
purpose: This function will perform a bitwise NAND operation
on registers $r[b] and $r[c], the result will be stored in 
r[a].
input: The register data structure, and the bitpacked 
instruction.
output: None
*/
void bNAND( void * registers, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t regB = Bitpack_getu(instruction, 3, 3);
    uint32_t regA = Bitpack_getu(instruction, 3, 6);
    uint32_t rB = getRegisters(registers, regB);
    uint32_t rC = getRegisters(registers, regC);
    setRegisters(registers, regA, ~(rB & rC)); 
}

/*
function: halt 
purpose: stops the program whereever it is at.
input: the total number of instructions and
        the index of the current instruction
output: none 
*/
void halt(uint32_t *count, uint32_t *current)
{
    *current = *count;
}

/*
function: output
purpose: prints out the value of the register C
input: a pointer to the registers array and the current instruction
output: none
*/
void output(void * registers, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    uint32_t rC = getRegisters(registers, regC);
    uint32_t lowerLimit = 0;
    uint32_t upperLimit = 255;
    assert(rC <= 255);
    if (rC >= lowerLimit && rC <= upperLimit){
        printf("%c",rC);
    }
}

/*
function: input
purpose: reads in the user input into register C
input: a pointer to the registers array and the current instruction
output: none
*/
void input(void * registers, uint32_t instruction)
{
    uint32_t regC = Bitpack_getu(instruction, 3, 0);
    int x = getchar();
    uint32_t output = x;
    if (x == EOF){
        output = ~0;
    }
    setRegisters(registers, regC, output);
}

/*
function: loadVal
purpose: unpacks a value from an instruction and places it into a register
input: a pointer to the registers array and the current instruction
output: none
*/
void loadVal(void* registers, uint32_t instruction)
{
    uint32_t regA = Bitpack_getu(instruction, 3, 25);
    uint32_t regVal = Bitpack_getu(instruction, 25, 0);
    setRegisters(registers, regA, regVal);
}