#ifndef __FPGA_COMM__
#define __FPGA_COMM__
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>

void *mem_alloc(int size, unsigned int base_addr, int mem_dev);
void mem_free(void *pointer, int size, unsigned int base_addr);
#endif
