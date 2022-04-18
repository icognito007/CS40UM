/*
*   register.h
*   CS40 Hw #6: UM
*       by:  Muhammad Ali Khan, Harnaljia Fenelon 
*   Date: April 10th, 2022
* 
*   Summary: 
*       This file holds all the function declarations for the functions that
*       creates and directly inpact the registers. 
*/
#ifndef REGISTER_H
#define REGISTER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <seq.h>
#include <stack.h>
#include <assert.h>
#include <bitpack.h>

/* creates the array of registers */
void* createRegisters();
/* acceses a register */
uint32_t getRegisters(void* registers, uint32_t index);
/* sets the value of a register to the given value */
void setRegisters(void* registers, uint32_t index, uint32_t val);
/* verifies that the give index corresponds to an existig register */
void validRegister (uint32_t index);
/* frees all the registers */
void freeRegister (void *registers);

#endif