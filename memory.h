/*
*   memory.h
*   CS40 Hw #6: UM
*       by:  Muhammad Ali Khan, Harnaljia Fenelon 
*   Date: April 10th, 2022
* 
*   Summary: 
*       Contains all the memory related functions for our program. 
* 
*/
#ifndef MEMORY_H
#define MEMORY_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <seq.h>
#include <stack.h>
#include <assert.h>
#include <bitpack.h>


/* making structs into pointers */
typedef struct memory* memory;
typedef struct segment* segment;

/* creates the memory data structure */
extern void* createMem();
/* takes in file info and fills the memory structure */
void setFileMem (void * input, void* inputMemory, uint32_t length);

/* gets a requested piece of memory from the memory structure */
uint32_t getMem (void *initialMemory, uint32_t memIndex, uint32_t seqIndex);
/* gets the length of a memory segment*/
uint32_t getLength (void *initialMemory, uint32_t memIndex);
/* This function will set $m[memIndex][seqIndex] = val */
void setMem (void *initialMemory, uint32_t memIndex, uint32_t seqIndex,
                                                         uint32_t val);
/* insert a new memory segment */
uint32_t insertSeg (uint32_t length, void *initialMemory);
/* delete a memory segment */     
void delSeg (uint32_t index, void *initialMemory);
/* duplicates a memory segment and replaces the information a m0 */     
void copyNReplace(uint32_t rB, uint32_t index, void * initialMemory,
                                                    uint32_t *count);
/* frees all allocated memory */
void freeFunc(void * initialMemory);

#endif