#ifndef SCHED_H
#define SCHED_H

#include "kheap.h"



typedef int(func_t)(void);
struct pcb_s{
	uint32_t registres[14];
	func_t* lr_svc;
	func_t* lr_user; 
	uint32_t* stack;
	uint32_t* sp;
	uint32_t* cpsr;

	struct pcb_s * next_prc;
};

struct pcb_s* list_pointer;
struct pcb_s* current_process;
struct pcb_s kmain_process;


void sched_init();
void elect();
void createProcess(func_t* entry);

#endif



