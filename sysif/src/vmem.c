#include "vmem.h"

//addresses between 0x20000000 and 0x20ffffff
//where the peripherals are mapped
uint32_t device_flags = 0x036;

unsigned int init_kern_translation_table(void){
	uint32_t *first_lvl_cursor,second_lvl_cursor;
	int i,j;
	
	first_lvl_cursor = (uint32_t*)kAlloc_aligned(FIRST_LVL_TT_SIZE,12);
	for(i=0;i<FIRST_LVL_TT_SIZE;i++){
		second_lvl_cursor = (uint32_t *)kAlloc_aligned(SECON_LVL_TT_SIZE,8);
		first_lvl_cursor[i] = (uint32_t) second_lvl_cursor;
	} 
}

