#include "main.h"

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
 
   run_flisp(); 
 }
