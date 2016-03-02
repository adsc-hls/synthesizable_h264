//Given a physical address, allocate a specified amount of memory
//corresponding to that physical address
//Reference: http://falsinsoft.blogspot.sg/2013/10/access-physical-memory-in-linux.html

#include "fpga_comm.h"

void *mem_alloc(int size, unsigned int base_addr, int mem_dev)
{

  uint32_t alloc_mem_size, page_mask, page_size;
  void *mem_pointer;

  page_size = sysconf(_SC_PAGESIZE);
  alloc_mem_size = (((size / page_size) + 1) * page_size);
  page_mask = (page_size - 1);

  mem_pointer = mmap(NULL,
      alloc_mem_size,
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      mem_dev,
      (base_addr & ~page_mask)
      );

  if(mem_pointer == MAP_FAILED)
  {
    // Error
    printf("Error setting up mem_pointer\n");
    return NULL;
  }

  return (mem_pointer + (base_addr & page_mask));
}

void mem_free(void *pointer, int size, unsigned int base_addr)
{
  uint32_t alloc_mem_size, page_mask, page_size;
  void *mem_pointer;

  page_size = sysconf(_SC_PAGESIZE);
  alloc_mem_size = (((size / page_size) + 1) * page_size);
  page_mask = (page_size - 1);

  mem_pointer = pointer - (base_addr & page_mask);
  munmap(mem_pointer, alloc_mem_size);
}

