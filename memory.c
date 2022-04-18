/*
*   memory.c
*   CS40 Hw #6: UM
*       by:  Muhammad Ali Khan, Harnaljia Fenelon 
*   Date: April 10th, 2022
* 
*   Summary: 
*       Implementation file for memory.h. This file deals with all
*       the memory related functions, keeping the memory implementation 
*       a secret from other files in our program.
* 
*/
#include "memory.h"

/*
purpose: memory struct that holds a sequence of pointers 
    and a stack of unmapped indeces. 
*/
struct memory
{
    Seq_T activeMem;
    Stack_T unmapStack;
};

/*
purpose: segment struct that holds a uint32_t array of memory
    and a counter variable that keeps track of the length of array.  
*/
struct segment
{
    uint32_t *currentMem;
    uint32_t counter; 
};

/*
function: createMem 
purpose: creates the data structure to hold the memory. 
input: none 
output: returns a void pointer to a struct containing 
    the memory sequence and a stack. 
*/
void* createMem()
{
    memory initial = (memory) malloc(sizeof(struct memory)); 
    assert(initial != NULL);
    int initialCapacity = 1000;

    /* Creating a sequence of uint64_t pointers */
    Seq_T memory = Seq_new(initialCapacity);
    Seq_addlo(memory, (void *) NULL);

    /* Creating a stack that holds the index of unmapped segments */
    Stack_T unmapStack = Stack_new();
    uint32_t *val = (uint32_t *) malloc(sizeof(uint32_t));
    assert(val != NULL);
    *val = 0; 
    Stack_push (unmapStack, (void *) val);
    
    /* Packing the information into a struct */
    initial->activeMem = memory;
    initial->unmapStack = unmapStack;

    /* Returning the memory struct */
    return (void *) initial;
}

/*
function: setFileMem
purpose: Stores the instructions from the input file. 
input: a void pointer to the file steam, the memory struct,
    and the length of the file. 
output: none
*/
void setFileMem (void * input, void* inputMemory, uint32_t length)
{
    /* input the instructions into the first slot of the sequence */
    Seq_T access = ((memory)inputMemory)->activeMem;
    uint32_t *index = (uint32_t*)Stack_pop(((memory)inputMemory)->unmapStack);
    segment initial = malloc (sizeof(struct segment)); 
    assert(initial != NULL);
    initial->counter = length;
    initial->currentMem = (uint32_t *)input;
    Seq_put(access, *index, (void *) initial);
    free(index);
}

/*
function: insertSeg
purpose: adds a new segment into the sequence either by reusing
    an unmapped index or by allocating new space. 
input: the length of the file and the memory struct
output: returns the index of the newly formed segment. 
*/
uint32_t insertSeg (uint32_t length, void *initialMemory)
{
    Stack_T *stackMem = &(((memory) initialMemory)->unmapStack);
    Seq_T *currentSeq = &(((memory) initialMemory)->activeMem); 
    uint32_t index;
    segment newSeg = (segment) malloc(sizeof(struct segment));
    assert(newSeg != NULL);
    newSeg->currentMem = (uint32_t*)malloc(length * 4);
    assert(newSeg->currentMem != NULL);
    for (uint32_t i = 0; i < length; i++){
        (newSeg->currentMem)[i] = 0;
    }
    newSeg->counter = length;

    /* If the stack is empty, add a new slot at the end of the sequence */
    if (Stack_empty(*stackMem)){ 
        Seq_addhi(*currentSeq, NULL);
        index = Seq_length(*currentSeq) - 1;
    }
    else { /* If a stack isnt empty then used than reuse that index */
        uint32_t *temp = (uint32_t *)Stack_pop(*stackMem);
        index = *temp;
        free(temp);
    }

    Seq_put(*currentSeq, index, newSeg);
    return index;
}

/* TODO CHECK INDEX IS IN BOUNDS
function: delSeg 
purpose: deletes a segment form a specified index
input: an index in the sequence and the memory struct 
output: none 
*/
void delSeg (uint32_t index, void *initialMemory)
{
    Stack_T *stackMem = &(((memory) initialMemory)->unmapStack);
    Seq_T *currentSeq = &(((memory) initialMemory)->activeMem); 
    segment memory = (segment) Seq_put(*currentSeq,index,NULL);

    /*Free the memory array, the struct and add the index to the stack */
    assert(memory != NULL);
    free(memory->currentMem);
    free(memory);
    uint32_t *stackIndex = (uint32_t *)malloc(sizeof(uint32_t));
    assert(stackIndex != NULL);
    *stackIndex = index;
    Stack_push(*stackMem, (void *) stackIndex); 
}

/*
function: getMem
purpose: This function will return the value of $m[memIndex][seqIndex]
input: This function will take the memory architecture initalMemory 
    as input, as well as the indices memIndex and seqIndex.
output: return the value stored in $m[memIndex][seqIndex]
*/
uint32_t getMem (void *initialMemory, uint32_t memIndex, uint32_t seqIndex)
{
    Seq_T *access = &(((memory)initialMemory)->activeMem);
    assert(Seq_get(*access, memIndex)!= NULL);
    assert(seqIndex < ((segment)Seq_get(*access, memIndex))->counter);
    return (((segment)Seq_get(*access, memIndex))->currentMem)[seqIndex];
}

/*
function: setMem
purpose: This function will set $m[memIndex][seqIndex] = val
input: This function will take the memory architecture initialMemory 
    as input, as well as the indices memIndex and seqIndex. 
    Lastly, it will also take the value val.
output: None
*/
void setMem (void *initialMemory, uint32_t memIndex, uint32_t seqIndex,
                                                            uint32_t val)
{
    Seq_T *access = &(((memory)initialMemory)->activeMem);
    assert(Seq_get(*access, memIndex)!= NULL); 
    assert(seqIndex < ((segment)Seq_get(*access, memIndex))->counter);
    (((segment)Seq_get(*access, memIndex))->currentMem)[seqIndex] = val;
}

/*
function: copyNReplace
purpose: duplicates a segment of memory and replaces the memory segment at m0
input: a uint32 register, the index of the sequence to be replaced,
    the memory struct, the size of the segment, and a double pointer 
    to the memory segment m0.
output: none 
*/
void copyNReplace(uint32_t rB, uint32_t index, void *initialMemory, 
                                        uint32_t *count)
{
    segment duplicate = malloc (sizeof(struct segment)); 
    assert(duplicate != NULL);
    Seq_T *access = &(((memory)initialMemory)->activeMem);
    assert(Seq_get(*access, rB)!= NULL); 
    segment temp = (segment)(Seq_get((((memory)initialMemory)->activeMem),rB));
    duplicate->counter = temp->counter;
    
    /* magic number 4 */
    duplicate->currentMem = (uint32_t*) malloc (4 * duplicate->counter);
    assert(duplicate->currentMem != NULL);
    /* Making a deep copy of the memory */
    for (uint32_t i = 0; i < duplicate->counter; i++){
        (duplicate->currentMem)[i] = (temp->currentMem)[i];
    }
    /* Freeing the orignal M0 segment */
    free(((segment)(Seq_get((((memory) initialMemory)->activeMem),0)))
    -> currentMem);
    free(((segment)(Seq_get((((memory) initialMemory)->activeMem),0))));

    /* Putting the copied segment into the available M0 slot */
    Seq_put(((memory) initialMemory)-> activeMem, index, (void *)duplicate);
    *count = duplicate->counter;
}

/*
function: getLength
purpose: gets the length of the memory segment 
input:the memory struct and the index of the memory in the sequence
output: returns the length of that memory segment. 
*/
uint32_t getLength (void *initialMemory, uint32_t memIndex)
{
    Seq_T access = ((memory)initialMemory)->activeMem;
    return (((segment)Seq_get(access,memIndex))->counter);
}

/*
function: freeFunc
purpose: frees all allocated memory in the program 
input: a pointer to the registers array and the memory struct
output: none 
*/
void freeFunc(void * initialMemory){
    /* Free the stack */
    Stack_T *stackMem = &(((memory) initialMemory)->unmapStack);

    /* Free every memory array before freeing the sequence */
    Seq_T *currentSeq = &(((memory) initialMemory)->activeMem); 
    int length = Seq_length(*currentSeq);
    for( int i = 0; i < length; i++){
        segment temp = (segment) Seq_get( *currentSeq,i);
        if (temp != NULL){
            free(temp->currentMem);
            free(temp);
        }
    }
    Seq_free(currentSeq);

    //while the stack is not empty
    while (!Stack_empty(*stackMem)){
        free(Stack_pop(*stackMem));
    }
    Stack_free(stackMem);
    free(initialMemory);
}