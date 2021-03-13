// File utils
// Used for loading executable flisp code into flispvm.
// Supported fileformats: s19

#include "file_utils.h"

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
