/*
 * A FLISP VM used for testing if flispcc works correctly.
 * Not all instructions are implemented 
 * Author: Karl Strålman
 * Inspired by: https://justinmeiners.github.io/lc3-vm/
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
  R_SP,
  R_PC,   //Program counter
  R_TA,
  R_CC,
  R_I     //Instruction
};

/* Flags in CC register */
enum {
  FL_I = 0b10000, //Interrupt
  FL_N = 0b01000, //Negative
  FL_Z = 0b00100, //Zero
  FL_V = 0b00010, //Overflow
  FL_C = 0b00001  //Carry
};

/* Registers */
uint8_t reg[R_I];

/* 256 bytes memory */
uint8_t memory [MEM_SIZE];

void mem_write(uint8_t address, uint8_t value) {
  memory[address] = value;
}

uint8_t mem_read(uint8_t address) {
  return memory[address];
}

/* Prints all memory [adr]:[data]
 * e.g. 00:00 01:00 ..*/
void mem_print() {
  printf("[addr]:[data]\n");
  for(int i = 0; i < MEM_SIZE; i ++) {
    printf("%02X:%02X\t", i, mem_read(i));
    if(i % 15 == 0)
      printf("\n");
  }
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

/* Reads bytes from s19 file to memory
 * (Specifically uses S1 and S9 record fields)
 * This file format may also be known as SRECORD, SREC, S19, S28, S37
 */
int read_s19_file(FILE *file) {
  char tmp[2]; //place to store S1 and finally overwrite S9
  unsigned int byte_c; 
  unsigned int addr;
  unsigned int cnt = 0;
  
  /* All instructions (op + operand) will be stored 
   * here temporarily then moved into actual memory*/
  unsigned int *mem_temp; 

  fscanf(file, "%2s", tmp);       /* Read S1 */
  fscanf(file, "%2x", &byte_c);   /* Read byte count 1 bytes */
  fscanf(file, "%4x", &addr);  /* Read address 2 bytes */
  cnt += 2;  // adjust counter 2 byte for address
  mem_temp = malloc(sizeof(unsigned int) * byte_c);

  unsigned int i = 0;
  while(cnt < byte_c) {
    int rv = fscanf(file, "%2x", &mem_temp[i++]);
    if(rv != 1)
      break;
    cnt++;
  }

  if(cnt != byte_c) {
    printf("Error: byte count not correct value: %u\n", cnt);
    return 1;
  }
  /* Copy mem_temp to memory
   * starting from addr offset 
   */
  for(int j = 0; j < i; j++) {
    mem_write(addr + j, mem_temp[j]);
  }
  free(mem_temp);  
  return 0;   
}


/* Open filestream to a file*/
int read_file(const char *image_path) {
  FILE *file = fopen(image_path, "rb");
  if(!file) return 0;
  read_s19_file(file);
  fclose(file);
  printf("File: %s successfully read into memory\n", image_path);
  return 0;
}

int main(int argc, const char *argv[]) {
  if(argc < 2) {
    printf("Usage: flispvm [s19-file]\n");
    return 1;
  }

  for(int i = 1; i < argc; i++) {
    if(read_file(argv[i])) {
      printf("Failed to load file: %s\n", argv[i]);
      return 1;
    }
  }
  
  mem_print();

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
      /* Not implemented case or bad op code */
      default:
        alive = 0;
        printf("Warning: Op code not implemented\n");
        break;
    }
  }
}
