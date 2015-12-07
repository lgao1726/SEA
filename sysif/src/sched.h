#ifndef SCHED_H_INCLUDED
#define SCHED_H_INCLUDED
#include "kheap.h"
#include "hw.h"
#include "asm_tools.h"

enum process_status{PROCESS_RUNNING,PROCESS_TERMINATED};

typedef int(func_t)(void);
struct pcb_s{
	uint32_t registres[14];
	func_t* lr_svc;
	func_t* lr_user;
	uint32_t* stack;
	uint32_t* sp;
	uint32_t* cpsr;
	struct pcb_s* next_process;
	struct pcb_s* prev_process;
	uint32_t status;
	uint32_t code;
};

struct pcb_s *current_process;
struct pcb_s kmain_process;
struct pcb_s *list_pointer;
uint32_t* lr_irq;

void sched_init();
void elect();
struct pcb_s* createProcess(func_t* entry);


#endif
