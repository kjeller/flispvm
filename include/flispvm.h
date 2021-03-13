/*
 * This is a VM implementation for the FLISP 
 * (FLexible Instructions Set Processor).
 *
 * flispvm used for testing if flispcc works correctly.
 * Reads S19 file into memory and then executes starting from 
 * address 20. There are many instructions in the FLISP ISA
 * and not all are implemented here. Only the most common ones, that
 * is used within the flispcc compiler.
 *  
 * Author: Karl Strålman
 * Inspired by: https://justinmeiners.github.io/lc3-vm/
 */

#ifndef H_FLISPVM
#define H_FLISPVM

#include <stdio.h>
#include <stdint.h>
#include "opcodes.h"

#define RST_VECTOR 0xFF
#define MEM_SIZE 256

/* All registers in Flisp */
enum {
  R_A = 0, //ACC
  R_T,
  R_R,
  R_X,
  R_Y,
  R_SP,   // Stack pointer
  R_PC,   // Program counter
  R_TA,   
  R_CC,   // Flag register
  R_I     // Instruction
};

/* Flags in CC register */
enum {
  FL_I = 0b10000, //Interrupt
  FL_N = 0b01000, //Negative
  FL_Z = 0b00100, //Zero
  FL_V = 0b00010, //Overflow
  FL_C = 0b00001  //Carry
};

void run_flisp();
#endif
