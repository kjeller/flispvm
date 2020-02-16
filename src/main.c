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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/opcodes.h"

#define RST_VECTOR 0xFF
#define MEM_SIZE 256

/* Reads bytes from s19 file to memory
 * (Specifically uses S1 and S9 record fields)
 * This file format may also be known as SRECORD, SREC, S19, S28, S37
 * */
int read_s19_file(FILE *file);

/* Open filestream to a file*/
int read_file(const char *image_path); 


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

/* Prints all memory [adr]:[data] */
void mem_print() {
  for(unsigned char lsb = 0; lsb <= 0xF; lsb++) {
    for(unsigned char msb = 0; msb <= 0xF; msb++) {
      unsigned int addr = msb << 4 | lsb;  
      printf("%02X:%02X ", addr, mem_read(addr));
      
    }
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

/* Uses next byte in memory as operand.
 * PC will then increase
 * */
int8_t op_arg() {
  return mem_read(reg[R_PC]++);
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
  //reg[R_PC] = mem_read(RST_VECTOR);
  reg[R_PC] = 0x20;
  reg[R_SP] = 0x20;
  int alive = 1;
  while(alive) {
    printf("PC=%02X, SP=%02X\n", reg[R_PC], reg[R_SP]);

    /* Fetch */
    uint8_t op_code = mem_read(reg[R_PC]++);
     
    switch(op_code) {
      case NOP:
        break;
      /* Load/Store */
      case LDA_IM:
        reg[R_A] = op_arg();
        update_flags(reg[R_A]);
        break;
      case LDA_AB:
        reg[R_A] = mem_read(op_arg());
        update_flags(reg[R_A]);
        break;
      case LDA_NS:
        reg[R_A] = mem_read(reg[R_SP] + op_arg());
        update_flags(reg[R_A]);
        break;
      case LEASP_NS:
        reg[R_SP] += op_arg();
        break;
      case STA_AB:
        mem_write(op_arg(), reg[R_A]);
        break;
      
      /* Integer arithmetic */
      case ADDA_NS:
        reg[R_A] += mem_read(op_arg() + reg[R_SP]);
        break;
      case DECA:
        reg[R_A]--;
        break;
      
      /* Stack operations */
      case PSHA:
        reg[R_SP] -= 1;
        mem_write(reg[R_SP], reg[R_A]);
        break;

      /* Program flow */
      case JSR_AB:
        reg[R_SP] -= 1;
        mem_write(reg[R_SP], reg[R_PC]);
        reg[R_PC] = op_arg();
        break;
      case RTS:
        reg[R_PC] = mem_read(reg[R_SP]++);
        break;

      /* Not implemented case or bad op code */
      default:
        alive = 0;
        printf("Warning: Op code (%02X) not implemented\n", op_code);
        printf("Stopped at: PC=%02X\n", reg[R_PC]);
        break;
    }
  }
}

/* ===== File input functions below ===== */
int read_s19_file(FILE *file) {
  char tmp[2]; //place to store S1 and finally overwrite S9
  
  /* Derived directly from file:
   * S1[byte_c][addr][data][chsum] */
  unsigned int byte_c, addr,  chsum;
  unsigned char cnt; /* to check if byte_c is correct after read */
  unsigned long byte_sum;
  
  /* All instructions (op + operand) will be stored 
   * here temporarily then moved into actual memory*/
  unsigned int *mem_temp; 

  fscanf(file, "%2s", tmp);       /* Read S1 */
  fscanf(file, "%2x", &byte_c);   /* Read byte count 1 bytes */
  fscanf(file, "%4x", &addr);     /* Read address 2 bytes */
  cnt = 2;                        /* adjust counter 2 byte for address */
  mem_temp = malloc(sizeof(unsigned int) * byte_c);

  unsigned int i = 0;
  while(cnt < byte_c-1) {
    int rv = fscanf(file, "%2x", &mem_temp[i]);
    if(rv != 1)
      break;

    // add byte to validate
    byte_sum += mem_temp[i++];
    cnt++;
  }
 
  /* Last byte is checksum */
  fscanf(file, "%2x", &chsum);
  cnt++; //include checksum in bytecount

  /* Validate checksum 
   * 1. Add each byte after S1 to total
   * 2. Mask: Keep the lowest byte of total
   * 3. Compute complement of least significant byte
   */

  /* Add individual byte from 2 byte address*/
  byte_sum += (addr >> 8) + (addr & 0xFF);
  byte_sum += byte_c;
  /* Mask */ 
  unsigned char _chsum = (byte_sum & 0xFF);
  /* Complement */
  _chsum = ~_chsum;

  /* Error handling */
  if(_chsum != chsum) {
    printf("Error: Checksum invalid (%2X != %2X)\n", _chsum, chsum);
    return 1;
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

int read_file(const char *image_path) {
  FILE *file = fopen(image_path, "rb");
  if(!file) return 0;
  read_s19_file(file);
  fclose(file);
  printf("%s successfully read into memory\n", image_path);
  return 0;
}


