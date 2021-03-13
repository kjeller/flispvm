/*
 * File utils used for reading instructions into flispvm memory
 *
 * Author: Karl Strålman
 */

#ifndef H_FILE_HEADER
#define H_FILE_HEADER

#include <stdio.h>
#include <stdlib.h>

/* Reads bytes from s19 file to memory
 * (Specifically uses S1 and S9 record fields)
 * This file format may also be known as SRECORD, SREC, S19, S28, S37
 * */
int read_s19_file(FILE *file);

/* Open filestream to a file */
int read_file(const char *image_path); 

#endif
