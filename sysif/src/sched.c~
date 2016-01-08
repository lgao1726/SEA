#include "sched.h"
#include "syscall.h"
/*pointe le pcb current_process vers kmain*/
uint32_t* stack;

void sched_init(){
	current_process = &kmain_process;
	list_pointer = &kmain_process;
	kheap_init();
}

void create_process(func_t* entry){
	struct pcb_s* p =(struct pcb_s*)  kAlloc(sizeof(struct pcb_s));
	
	p->lr_user = entry;
	p->lr_svc = p->lr_user;
	p->status = PROCESS_RUNNING;
	//initialize a stackof size 10KB
	p->stack = (uint32_t*) kAlloc(10000);
	p->sp = (p->stack+10000);

	list_pointer->next_process = p;
	if(list_pointer==&kmain_process){
		list_pointer->prev_process = p;
	}
	p->next_process = kmain_process.next_process;
	p->prev_process = list_pointer->prev_process;
    kmain_process.next_process->prev_process = p;	
	list_pointer = p;		
}
//how to choose the next process
void elect(){
	current_process = current_process->next_process;	
		
}

void context_save_to_pcb(){
	int i;
	for(i=0;i<14;i++){
		current_process->registres[i] = stack[i];
	}
	//save cpsr_user by copying spsr_svc
	__asm("mrs r0,spsr");
	__asm("mov %0,r0" : "=r"(current_process->cpsr));
	//switch temporarily to System mode to load sp
	//and also save lr
	__asm("cps 0x1F");
	__asm("mov %0,sp" :"=r"(current_process->sp));
	__asm("mov %0,lr" :"=r"(current_process->lr_user));
	__asm("cps 0x13");
	//save lr_irq (called lr_svc)
	current_process->lr_svc = (func_t*)current_process->registres[13];
}

void context_load_from_pcb(){
	//restore context
	int i;
	for(i=0;i<14;i++){
		stack[i] = current_process->registres[i];
	}
	stack[13] = (uint32_t)current_process->lr_svc; 
	//we can't access the user sp register from IRQ
	//switch temporarily to System mode to load sp
	__asm("cps 0x1F");
	__asm("mov sp, %0" : :"r"(current_process->sp));
	__asm("mov lr,%0": :"r"(current_process->lr_user));
	__asm("cps 0x13");

}

void yieldto_irq(){
	context_save_to_pcb();
	elect();
	context_load_from_pcb();
}

//IRQ handler
 void __attribute__((naked)) irq_handler(){
	__asm("SUB lr,lr,#4");//subtract 4 otherwise we "skip" an instruction
	__asm("stmfd sp!,{r0-r12,lr}");
	__asm("mov %0,sp" : "=r"(stack));
	__asm("cps 0x13"); //enter svc mode
	do_sys_yieldto(); 
	__asm("cps 0x12"); //return to irq mode
	//rearm timer
	set_next_tick_default();
	ENABLE_TIMER_IRQ();
	ENABLE_IRQ();
	__asm("ldmfd sp!,{r0-r12,pc}^");
	__asm("cps 0x10");
	__asm("mov pc, %0" : :"r"(lr_irq));
}
