#ifndef VMEM_H
#define VMEM_H

#include "kheap.h"

//page size in bytes
uint32_t PAGE_SIZE = 4096;
//number of second level entries
uint32_t SECON_LVL_TT_COUN = 256;
//size of second level table
uint32_t SECON_LVL_TT_SIZE = 1024;
//number of first level entries
uint32_t FIRST_LVL_TT_COUN = 4096;
//size of first level table
uint32_t FIRST_LVL_TT_SIZE = 16384;



#endif 
