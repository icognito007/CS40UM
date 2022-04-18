/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <limits.h>

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction cmov(Um_register a, Um_register b,
 Um_register c) 
{
        return three_register(CMOV, a, b, c);
}
static inline Um_instruction segLoad(Um_register a, Um_register b, 
Um_register c) 
{
        return three_register(SLOAD, a, b, c);
}
static inline Um_instruction mapSeg(Um_register a, Um_register b, 
Um_register c) 
{
        return three_register(ACTIVATE, a, b, c);
}
static inline Um_instruction segStore(Um_register a, Um_register b,
 Um_register c) 
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b,
 Um_register c) 
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction division(Um_register a, Um_register b,
 Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction bitwiseNAND(Um_register a, Um_register b,
 Um_register c) 
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction unMap(Um_register a, Um_register b,
 Um_register c) 
{
        return three_register(INACTIVATE, a, b, c);
}

static inline Um_instruction loadProgram(Um_register a, Um_register b,
 Um_register c) 
{
        return three_register(LOADP, a, b, c);
}

Um_instruction input(Um_register c);

Um_instruction output(Um_register c);

/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, op);
        word = Bitpack_newu(word, 3, 6, ra);
        word = Bitpack_newu(word, 3, 3, rb);
        word = Bitpack_newu(word, 3, 0, rc);

        return word;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, LV);
        word = Bitpack_newu(word, 3, 25, ra);
        word = Bitpack_newu(word, 25, 0, val);

        return word;
}

Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}
/* Unit tests for the UM */

/* */
void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

void build_add_test(Seq_T stream)
{
        append(stream, loadval(r2, 'a'));
        append(stream, loadval(r3, 1));
        append(stream, add(r1, r2, r3));
        append(stream, output(r1));
        append(stream, loadval(r4, 'b'));
        append(stream, loadval(r7, 10));
        append(stream, add(r3, r4, r7));
        append(stream, output(r3));
        append(stream, halt());
}

/* testing the print function */
void build_print_test(Seq_T stream)
{
        append(stream, loadval(r1, 50));
        append(stream, loadval(r2, 52));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, halt());  
}

/* Testing that the conditional move only occurs when register C is not 0 */
void build_condMove_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 50));

        /* conditional move shouldn't run */
        append(stream, loadval(r3, 0));
        append(stream, cmov(r1, r2, r3));
        append(stream, output(r1));
        append(stream, output(r2));

        /* conditional move should run */ 
        append(stream, loadval(r3, 10));
        append(stream, cmov(r1, r2, r3));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, halt());
}

/*This function will exhasutively check the mapSeg command. It will
    first create a new segment using that command and ensure that
    the address of the newly created segment is as expected. */
void build_mapSeg_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 2));
        append(stream, loadval(r3, 10));
        append(stream, mapSeg(r1, r2, r3));
        append(stream, output(r1)); /* rA */
        //rB should = 1 because mapSeg stores the adress of the
        //newly created segment into register $r[b] which in
        //our implementation should be at one.
        append(stream, output(r2)); /* rB */
        append(stream, output(r3)); /* rC */ 
        append(stream, halt());
}

/*  This function will first create a new segment in memory, it will
    then load values into the segment using segLoad. The values are
    then output inorder to ensure that they were correct loaded.
    This function has previously also tested inorder to ensure that a 
    value cannot be loaded from a sement using segLoad that is out of 
    bounds of that segment.*/
void build_segLoad_test(Seq_T stream)
{
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r2, 2));
        append(stream, loadval(r3, 10));
        append(stream, mapSeg(r1, r2, r3));
        
        append(stream, loadval(r3, 1));
        append(stream, segLoad(r1, r2, r3));
        append(stream, output(r1));

        /* testing out of bounds inputs which should break code */
        // append(stream, loadval(r3, 15));
        // append(stream, segLoad(r1, r2, r3));
        // append(stream, output(r1));
        append(stream, halt());
}

/* This function will first create a new segment in memory using the 
    memSeg command. The function will then load values into the newly
    mapped segment inorder to ensure that it is operational and working.
    The function will display the results of the loaded values in order
    to ensure that they are as expected.
    This function has previously also tested inorder to ensure that a 
    value cannot be loaded into a sement using segStore that is out of 
    bounds of that segment.*/
void build_segStore_test(Seq_T stream)
{
        append(stream, loadval(r1, 'a'));
        append(stream, loadval(r2, 2));
        append(stream, loadval(r3, 10));
        append(stream, mapSeg(r1, r2, r3));
        append(stream, loadval(r3, 1));
        append(stream, output(r3));
        /* rA now contains value of rB returned from mapSeg */
        append(stream, cmov(r1, r2, r3));
        append(stream, loadval(r2, 5));
        append(stream, loadval(r3, 100));
        append(stream, output(r3));
        /* stores $m[r1][r2] = r3 */
        append(stream, segStore(r1, r2, r3));
        /* loads $r4 = m[r1][r2] -> this way we can see 
                                if the function actually worked */
        append(stream, segLoad(r4, r1, r2));
        append(stream, output(r4));
        append(stream, halt());


}

/* */
void build_multiply_test(Seq_T stream)
{
        append(stream, loadval(r1, 200));
        append(stream, loadval(r2, 2));
        append(stream, loadval(r3, 50));
        append(stream, multiply(r1,r2,r3));
        append(stream, output(r1));

        /* Testing overflow situiations */
        // append(stream, loadval(r2, (uint32_t) INT_MAX));
        // append(stream, multiply(r1,r2,r3));
        // append(stream, output(r1));
        append(stream, halt());
}

void build_division_test(Seq_T stream)
{
        append(stream, loadval(r1, 'a'));
        append(stream, loadval(r2, 4000));
        append(stream, loadval(r3, 40));
        append(stream, division(r1,r2,r3));
        append(stream, output(r1));

        append(stream, loadval(r2, 4000));
        append(stream, loadval(r3, 400000));
        append(stream, division(r1,r2,r3));
        append(stream, output(r1));
        append(stream, halt());

        /* Test: dividing by 0 which should abort the programm */
        // append(stream, loadval(r3, 0));
        // append(stream, division(r1,r2,r3));
        // append(stream, output(r1));
        // append(stream, halt());

}

/*This function will perform a bitwiseNAND between several
   values and then check that the output is as expected.
void build_bitwiseNAND_test(Seq_T stream)*/
void build_bitwiseNAND_test(Seq_T stream)
{
        /* manually checking the registers with a calculator to see
                if the output is what we expect */
        append(stream, loadval(r1, 'a'));
        append(stream, loadval(r2, 'e'));
        append(stream, loadval(r3, 'g'));
        append(stream, bitwiseNAND(r1,r2,r3));

        append(stream, loadval(r1, 50));
        append(stream, loadval(r2, 50));
        append(stream, loadval(r3, 80));
        append(stream, bitwiseNAND(r1,r2,r3));

        append(stream, loadval(r1, 50));
        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 1));
        append(stream, bitwiseNAND(r1,r2,r3));
        append(stream, halt());
}

/* This function will first map a segment.
    This ensures that mapSeg is 
    is actually creating new segment in memory. 
    After which the funciton will unMap that segment,
    if we write again to the unmapped segment the 
    function will return a exception.

    The function will then issue another mapSeg command,
    this command is used to ensure that the mapSegment
    function is acutally reusing the already created space
    in the memory sequence.
*/
void build_unMap_test(Seq_T stream)
{ 
        append(stream, loadval(r1, 36));
        append(stream, loadval(r2, 103));
        append(stream, loadval(r3, 100));
        
        append(stream, mapSeg(r1, r2, r3));
        append(stream, output(r2));
        /*  Stores the value of $m[r2][r1] = r3 = 100 */
        append(stream, segStore(r2,r1,r3));
        append(stream, unMap(r1, r2, r2));
        /* This test fails as expected because r2 is unmapped */
        /* append(stream, segStore(r2,r1,r3)); */

        /* Map uses the same index as the previous unmapped*/
        append(stream, mapSeg(r1, r2, r3));
        append(stream, output(r2));
        append(stream, halt());
}

/* Tests to ensure that the input register r3 is being correctly
input into
*/
void build_input_test(Seq_T stream)
{
        append(stream, loadval(r1, 36));
        append(stream, loadval(r2, 'e'));
        append(stream, input(r3));
        append(stream, output(r3));
        append(stream, halt());
}

/*
This test will test the loadProgram command exhaustively.

The test constructs an instruction segment, where in instructions
7- 10 are not executed in the inital pass of the program due to 
the first loadProgram command. A new segment is mapped and
these 4 instructions are copied into it. The new segment is then
used to replace the original instruction set using the second
loadProgram command. The new instruction
set ensures that it is a deep copy of the original instruction
set using the operation append(stream, segStore(r4,r5,r6)). 
*/
void build_loadProgram_test(Seq_T stream)
{
        append(stream, loadval(r4, 1));
        append(stream, loadval(r5, 1));
        append(stream, loadval(r6, 0));

        append(stream, loadval(r1, 97));
        append(stream, loadval(r2, 0));
        append(stream, loadval(r3, 11));
        /* If r2 == 0 */
        append(stream, loadProgram(r1, r2, r3));
        append(stream, segStore(r4,r5,r6));
        append(stream, loadval(r7, 100));
        append(stream, output(r7));
        append(stream, halt());

        /* Testing duplicate factor */
        append(stream, loadval(r1, 36));
        append(stream, loadval(r2, 103));
        append(stream, loadval(r3, 4));
        append(stream, output(r2));
        append(stream, mapSeg(r1, r2, r3));


        append(stream, loadval(r2, 0));
        append(stream, loadval(r3, 7));
        append(stream, segLoad(r1, r2, r3));
        
        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 0));
        append(stream, segStore(r2,r3,r1));

        append(stream, loadval(r2, 0));
        append(stream, loadval(r3, 8));
        append(stream, segLoad(r1, r2, r3));
        
        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 1));
        append(stream, segStore(r2,r3,r1));

        append(stream, loadval(r2, 0));
        append(stream, loadval(r3, 9));
        append(stream, segLoad(r1, r2, r3));
        
        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 2));
        append(stream, segStore(r2,r3,r1));

        append(stream, loadval(r2, 0));
        append(stream, loadval(r3, 10));
        append(stream, segLoad(r1, r2, r3));
        
        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 3));
        append(stream, segStore(r2,r3,r1));

        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 0));
        append(stream, loadProgram(r1, r2, r3));
}

