/*
*   um.c
*   CS40 Hw #6: UM
*       by:  Muhammad Ali Khan, Harnaljia Fenelon 
*   Date: April 10th, 2022
* 
*   Summary: 
*       This file is the heart of our program as it cotains the main
*       function that processes user input and calls the function that 
*       processes all that input. 
*
*/
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
#include "memoryInstruction.h"
#include "register.h"
#include "registerInstruction.h"

static FILE *open_or_abort(char *fname, char *mode);
void readFile (FILE* file, void* inputMemory);
void processInstructions(void * registers, void * initialMemory);

int main(int argc, char *argv[])
{
    /* Crearting an array of registers */
    void* registers = createRegisters();
    
    /* Creating a struct with the memory information*/
    void* initialMemory = createMem();
    
    /* Opening and reading in the file */
    FILE *input;
    if (argc == 2){
        input = open_or_abort(argv[1], "r");
        readFile (input,initialMemory);

        /* Process the given instructions */
        processInstructions(registers, initialMemory);
    }
    else {
        printf("No input file provided \n");
        return EXIT_FAILURE;
    } 

    /* Free Memory */
    freeFunc(initialMemory);
    freeRegister(registers);
    fclose(input);
    return 0;
}

/* function: open_or_abort
 * purpose: opens a provided filename and checks its validity
 * input: valid filename has been taken in for reading
 * output: program is exited if exception is raised, or an open file 
 *                 is returned
 */
static FILE *open_or_abort(char *fname, char *mode)
{
    FILE *fp = fopen(fname, mode);
    assert(fp != NULL);
    return fp;
}

/*
function: readFile 
purpose: reads in the open filestream and stores the information 
    in our data structure. 
input: an open file stream, and a void pointer to a s
output: none
*/
void readFile (FILE* file, void* inputMemory)
{
    fseek(file, 0L, SEEK_END);

    /* calculating the size of the file */
    long int bytesInFile = ftell(file);
    rewind(file);
    int bytesPerElement = 4;
    int numElem = bytesInFile/bytesPerElement;

    uint32_t *input = (uint32_t *)malloc(bytesInFile);
    for(int i = 0; i < numElem; i++){
        uint32_t temp = 0;
        for(int j = 24; j >= 0; j -= 8){
            temp = Bitpack_newu(temp, 8, j, fgetc(file));
        }
        input[i] = temp;
    }
    setFileMem((void *)input, inputMemory, numElem);
}

/*
function: processInstructions
purpose: upacks the opcode from the instruction and calls the 
    nessecary function
input: a pointer to the registers array and a pointer to the memory struct 
output: none
*/
void processInstructions(void * registers, void * initialMemory)
{
    uint32_t current = 0;
    uint32_t memIndex = 0;
    uint32_t count = getLength (initialMemory, memIndex);
    uint64_t instruction;
    
        
    while (current < count){
        instruction = getMem (initialMemory, memIndex, current++);  
        //m0[current++];
        uint32_t opcode = Bitpack_getu(instruction, 4, 28);
        switch (opcode){
            case 0 :
            move(registers,instruction);
            break; 

            case 1 :
            segLoad(registers, initialMemory, instruction);
            break; 

            case 2 :
            segStore(registers, initialMemory, instruction);
            break; 

            case 3 :
            addition(registers, instruction);
            break; 

            case 4 :
            multiplication(registers, instruction);
            break; 

            case 5 :
            division(registers, instruction);
            break; 

            case 6 :
            bNAND(registers, instruction);
            break; 

            case 7 :
            halt(&count, &current);
            break; 

            case 8 :
            mapSeg(registers, initialMemory, instruction);
            break; 

            case 9 :
            unmapSeg(registers, initialMemory, instruction);
            break; 

            case 10 :
            output(registers, instruction);
            break; 

            case 11 :
            input(registers, instruction);
            break; 

            case 12 :
            loadProgram(registers, initialMemory, instruction, &current, 
                                                                &count);
            break; 

            case 13 :
            loadVal(registers, instruction);
            break; 

            default :
            /* Warn user if opcode is one of the specified operations */
            // TODO: IS THIS THE PROPER OUTPUT?
            fprintf(stderr,"Invalid Opcode\n");
        }
    }
}