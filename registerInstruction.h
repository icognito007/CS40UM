/*
*   registerInstruction.h
*   CS40 Hw #6: UM
*       by:  Muhammad Ali Khan, Harnaljia Fenelon 
*   Date: April 10th, 2022
* 
*   Summary: 
*       This file contains all the function declarations that interact
*           with the registers in the program. 
*/
#ifndef REGISTER_INSTRUCTION_H
#define REGISTER_INSTRUCTION_H

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

/* moves value of register B into register A if register c isnt 0 */
void move( void * registers, uint32_t instruction);
/* puts the sum of registers B & C into register A */
void addition( void * registers, uint32_t instruction);
/* puts the product of registers B & C into register A */
void multiplication( void * registers, uint32_t instruction);
/* puts the quotient of registers B & C into register A */
void division( void * registers, uint32_t instruction);
/* The function will perform a NAND operation in registers B $ C
    the result will be stored in register A*/
void bNAND( void * registers, uint32_t instruction);
/* immediately stops the program where it is at */
void halt(uint32_t *count, uint32_t *current);
/* prints out the value of registers C */
void output(void * registers, uint32_t instruction);
/* reads in the user input */
void input(void * registers, uint32_t instruction);
/* unpacks a value from an instruction and places it into register */
void loadVal(void* registers, uint32_t instruction);

#endif