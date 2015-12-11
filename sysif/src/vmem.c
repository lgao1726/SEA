#include "vmem.h"
#include "kheap.h"

uint32_t device_flags = 0x436;
uint32_t standard_flags = 0x472;	

// Table de page
uint8_t* page_table;

unsigned int init_kern_translation_table(void)
{
	page_table = kAlloc_aligned(FIRST_LVL_TT_SIZE, 12);
	
	uint32_t virtual_adress = 0;
	uint32_t* second_lvl_page_table_cursor;
	
	for (int i = 0; i < FIRST_LVL_TT_COUN; i++) {
		second_lvl_page_table = kAlloc_aligned(SECON_LVL_TT_SIZE, 8);
		second_lvl_page_table_cursor = (uint32_t*)second_lvl_page_table;
		
		for (int j=0; j < SECON_LVL_TT_COUN; j++) {
			if (virtual_adress <= 0x20FFFFFF & virtual_adress >= 0x20000000) {
				*second_lvl_page_table_cursor = virtual_adress + device_flags;
			}
	}	
}
