#include <stdint.h>
#include "opcodes.h"

/* 256 bytes memory */
uint8_t memory [256];

/* All registers in Flisp */
enum {
  R_A = 0, //ACC
  R_T,
  R_R,
  R_X,
  R_Y,
  R_SP,
  R_PC,   //Program counter
  R_TA,
  R_CC,
  R_I     //Instruction
};

uint8_t reg[R_I];


//TODO: Replace me with actual Op codes
enum
{
    OP_BR = 0, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
};

/* Flags in CC register */
enum {
  FL_I = 0b10000, //Interrupt
  FL_N = 0b01000, //Negative
  FL_Z = 0b00100, //Zero
  FL_V = 0b00010, //Overflow
  FL_C = 0b00001 //Carry
};

#define PC_START 0x20

int main(int argc, const char *argv[]) {
  /* RESET */
  reg[R_PC] = PC_START;

  int running = 1;
  while(running) {

    /* FETCH */
    /* In memory, OP code takes 1 byte and then its argument may be 
     * one byte or more.v*/
    uint8_t instr = mem_read(reg[R_PC]++);
    uint8_t op = instr; 

  }
}
