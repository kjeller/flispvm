/* 
 * opcodes.h
 * Author: Karl Strålman
 * Description:
 * All opcodes derived from opcode map for the FLISP.
 * There are 250 op codes available.
 */

#ifndef H_OP_CODES
#define H_OP_CODES

enum {
  /* 00-0F */
  NOP = 0,
  ANDCC,
  ORCC,
  NaN_0,
  NaN_1,
  CLRA,
  NEGA,
  INCA,
  DECA,
  TSTA,
  COMA,
  LSLA,
  LSRA,
  ROLA,
  RORA,
  ASRA,
  /* 10-1F */
  PSHA,
  PSHX,
  PSHY,
  PSHCC,
  PULA,
  PULX,
  PULY,
  PULCC,
  TFR_AC,
  TFR_CA,
  TFR_XY,
  TFR_YX,
  TFR_XS,
  TFR_SX,
  TFR_YS,
  TFR_SY,
  /* 20-2F */
  BSR,
  BRA,
  BMI,
  BPL,
  BEQ,
  BNE,
  BVS,
  BVC,
  BCS,
  BCC,
  BHI,
  BLS,
  BGT,
  BGE,
  BLE,
  BLT,
  /* 30-3F */
  STX_AB,
  STY_AB,
  STSP_AB,
  JMP_AB,
  JSR_AB,
  CLR_AB,
  NEG_AB,
  INC_AB,
  DEC_AB,
  TST_AB,
  COM_AB,
  LSL_AB,
  LSR_AB,
  ROL_AB,
  ROR_AB,
  ASR_AB,
  /* 40-4F */
  STX_NS,
  STY_NS,
  STSP_NS,
  RTS,
  RTI,
  CLR_NS,
  NEG_NS,
  INC_NS,
  DEC_NS,
  TST_NS,
  COM_NS,
  LSL_NS,
  LSR_NS,
  ROL_NS,
  ROR_NS,
  ASR_NS,
  /* 50-5F*/
  STX_NX,
  STY_NX,
  STSP_NX,
  JMP_NX,
  JSR_NX,
  CLR_NX,
  NEG_NX,
  INC_NX,
  DEC_NX,
  TST_NX,
  COM_NX,
  LSL_NX,
  LSR_NX,
  ROL_NX,
  ROR_NX,
  ASR_NX,
  /* 60-6F */
  STX_AX,
  STY_AX,
  STSP_AX,
  JMP_AX,
  JSR_AX,
  CLR_AX,
  NEG_AX,
  INC_AX,
  DEC_AX,
  TST_AX,
  COM_AX,
  LSL_AX,
  LSR_AX,
  ROL_AX,
  ROR_AX,
  ASR_AX,
  /* 70-7F */
  STX_NY,
  STY_NY,
  STSP_NY,
  JMP_NY,
  JSR_NY,
  CLR_NY,
  NEG_NY,
  INC_NY,
  DEC_NY,
  TST_NY,
  COM_NY,
  LSL_NY,
  LSR_NY,
  ROL_NY,
  ROR_NY,
  ASR_NY,
  /* 80-8F */
  STX_AY,
  STY_AY,
  STSP_AY,
  JMP_AY,
  JSR_AY,
  CLR_AY,
  NEG_AY,
  INC_AY,
  DEC_AY,
  TST_AY,
  COM_AY,
  LSL_AY,
  LSR_AY,
  ROL_AY,
  ROR_AY,
  ASR_AY,
  /* 90-9F*/
  LDX_IM,
  LDY_IM,
  LDSP_IM,
  SBCA_IM,
  SUBA_IM,
  ADCA_IM,
  ADDA_IM,
  CMPA_IM,
  BITA_IM,
  ANDA_IM,
  ORA_IM,
  EORA_IM,
  CMPX_IM,
  CMPY_IM,
  CMPSP_IM,
  EXG_AC,
  /* A0-AF */
  LDX_AB,
  LDY_AB,
  LDSP_AB,
  SBCA_AB,
  SUBA_AB,
  ADCA_AB,
  ADDA_AB,
  CMPA_AB,
  BITA_AB,
  ANDA_AB,
  ORA_AB,
  EORA_AB,
  CMPX_AB,
  CMPY_AB,
  CMPSP_AB,
  EXG_XY,
  /* B0-BF */
  LDX_NS,
  LDY_NS,
  LDSP_NS,
  SBCA_NS,
  SUBA_NS,
  ADCA_NS,
  ADDA_NS,
  CMPA_NS,
  BITA_NS,
  ANDA_NS,
  ORA_NS,
  EORA_NS,
  CMPX_NS,
  CMPY_NS,
  LEASP_NS,
  EXG_XS,
  /* C0-CF */
  LDX_NX,
  LDY_NX,
  LDSP_NX,
  SBCA_NX,
  SUBA_NX,
  ADCA_NX,
  ADDA_NX,
  CMPA_NX,
  BITA_NX,
  ANDA_NX,
  ORA_NX,
  EORA_NX,
  LEAX_NX,
  LEAY_NY,
  LEASP_NX,
  EXG_YS,
  /* D0-DF */
  LDX_NY,
  LDY_NY,
  LDSP_NY,
  SBCA_NY,
  SUBA_NY,
  ADCA_NY,
  ADDA_NY,
  CMPA_NY,
  BITA_NY,
  ANDA_NY,
  ORA_NY,
  EORA_NY,
  LEAX_NS,
  LEAY_NS,
  LEASP_NY,
  NaN_2,
  /* E0-EF */
  NaN_3,
  STA_AB,
  STA_NS,
  STA_NX,
  STA_AX,
  STA_XPOS,
  STA_XNEG,
  STA_POSX,
  STA_NEGX,
  STA_NY,
  STA_AY,
  STA_YPOS,
  STA_YNEG,
  STA_POSY,
  STA_NEGY,
  NaN_4,
  /* F0-FF */
  LDA_IM,
  LDA_AB,
  LDA_NS,
  LDA_NX,
  LDA_AX,
  LDA_XPOS,
  LDA_XNEG,
  LDA_POSX,
  LDA_NEGX,
  LDA_NY,
  LDA_AY,
  LDA_YPOS,
  LDA_YNEG,
  LDA_POSY,
  LDA_NEGY,
  NaN_6
};

#endif
