#include <stdint.h>
#include <stdio.h>
#include "opcodes.h"

/*
 * A FLISP VM for testing if flispcc works correctly.
 * Author: Karl Strålman
 * Inspired by: https://justinmeiners.github.io/lc3-vm/
 */

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

/* Flags in CC register */
enum {
  FL_I = 0b10000, //Interrupt
  FL_N = 0b01000, //Negative
  FL_Z = 0b00100, //Zero
  FL_V = 0b00010, //Overflow
  FL_C = 0b00001 //Carry
};

#define RST_VECTOR 0xFF

void mem_write(uint8_t address, uint8_t value) {
  memory[address] = value;
}

uint8_t mem_read(uint8_t address) {
  return memory[address];
}

void update_flags(uint8_t r) {
 if(r == 0)
    reg[R_CC] |= FL_Z;
 if(r >> 7)
   reg[R_CC] |= FL_N;
}

//TODO these two flag setters below
void unsigned_fcheck(uint8_t a, uint8_t b) {}
void signed_fcheck(int8_t a, int8_t b) {}

int main(int argc, const char *argv[]) {
  if(argc < 2) {
    printf("Usage: flispvm [image-file]\n");
    return 1;
  }

  for(int i = 1; i < argc; i++) {
    /*if (!read_image(argv[i])) {
        printf("Failed to load image: %s\n", argv[j]);
        return 1;
    }*/
  }

  /* Initial fetch: Set PC from RST_VECTOR*/
  reg[R_PC] = mem_read(RST_VECTOR);

  int alive = 1;
  while(alive) {

    /* Fetch */
    uint8_t op_code = mem_read(reg[R_PC]++);
    
    /* Assumes 2nd byte. Not used by every instruction.
     * It will refer to another instruction in cases 
     * where instr is a one byte instruction,
     *  and will therefore not be used. */
    uint8_t op_arg = mem_read(reg[R_PC]);
    
    switch(op_code) {
      /* LD */
      case LDA_IM:
        reg[R_A] = op_arg;
        update_flags(reg[R_A]);
        break;
      case LDA_AB:
        reg[R_A] = mem_read(op_arg);
        update_flags(reg[R_A]);
        break;
      case LDA_NS:
        reg[R_A] = mem_read(reg[R_SP] + op_arg);
        update_flags(reg[R_A]);
        break;
      /* LEA*/
      case LEASP_NS:
        reg[R_SP] = op_arg + reg[R_SP];
        break;
      /* STA */
      case STA_AB:
        mem_write(op_arg, reg[R_A]);
        break;
      /* PSH */
      case PSHA:
        reg[R_SP] = reg[R_SP] - 1;
        mem_write(reg[R_SP], reg[R_A]);
        break;

      case JSR_AB:
        reg[R_SP] = reg[R_SP] - 1;
        mem_write(reg[R_SP], reg[R_PC]);
        reg[R_PC] = op_arg;
        break;
      case RTS:
        reg[R_PC] = mem_read(reg[R_SP]++);
        break;
      /* Not implemented case */
      default:
        alive = 0;
        printf("Error: Opcode not valid\n");
        break;
    }
  }
}
