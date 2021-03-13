#include "flispvm.h"

/* Registers */
uint8_t reg[R_I];

void reg_print() {
  printf("----------------------------------------\n");
  printf("A=%02X, T=%02X, R=%02X, X=%02X, Y=%02X,\n" 
          "SP=%02X, PC=%02X, TA=%02X, CC=%02X, I=%02X\n", 
          reg[R_A], reg[R_T], reg[R_R], reg[R_X], reg[R_Y],
          reg[R_SP], reg[R_PC], reg[R_TA], reg[R_CC], reg[R_I]);
}

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
    reg[R_CC] = FL_Z;
 else if(r >> 7)
   reg[R_CC] = FL_N;
 else
   reg[R_CC] = 0;
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

void run_flisp() {
  mem_print();

  /* Initial fetch: Set PC from RST_VECTOR*/
  //reg[R_PC] = mem_read(RST_VECTOR);
  //TODO this will be replaced with LDSP #20, and ORG $FF FCB $20 later on
  reg[R_SP] = reg[R_PC] = 0x20;

  int alive = 1;
  while(alive) {
    reg_print();
    /* Fetch */
    uint8_t op_code = mem_read(reg[R_PC]++);
    reg[R_I] = op_code;
     
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
        update_flags(reg[R_A]);
        break;
      case INCA:
        reg[R_A]++;
        update_flags(reg[R_A]);
        break; 
      /* Stack operations */
      case PSHA:
        reg[R_SP] -= 1;
        mem_write(reg[R_SP], reg[R_A]);
        break;

      /* Program flow */
      case BSR:
        reg[R_SP] -= 1;
        mem_write(reg[R_SP], reg[R_PC]);
        reg[R_PC] += op_arg();
        break;
      case BRA:
        reg[R_PC] += op_arg();
        break;
      case BNE:
        if(!(reg[R_CC] & FL_Z))
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
      case BCC:
        if(!(reg[R_CC] & FL_C))
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
      case BCS:
        if(reg[R_CC] & FL_Z)
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
      case BEQ:
        if(reg[R_CC] & FL_Z)
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
      case BGE:
        if(((reg[R_CC] & FL_N) ^ (reg[R_CC] & FL_V)) == 0)
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
      case BGT:
        if(((reg[R_CC] & FL_N) ^ (reg[R_CC] & FL_V) || reg[R_CC] & FL_Z) == 0)
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
      case BHI:
        if((reg[R_CC] & FL_Z) || (reg[R_CC] & FL_C))
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
      case BLE:
        if(((reg[R_CC] & FL_N) ^ (reg[R_CC] & FL_V) || reg[R_CC] & FL_Z))
           reg[R_PC] += op_arg();
        else
          reg[R_PC] += 1;
        break;
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
  reg_print();

}
