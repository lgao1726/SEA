#include "sched.h"
#include "syscall.h"
/*pointe le pcb current_process vers kmain*/
void sched_init(){
	current_process = &kmain_process;
	list_pointer = &kmain_process;
	kheap_init();
}

void create_process(func_t* entry){
	struct pcb_s* p =(struct pcb_s*)  kAlloc(sizeof(struct pcb_s));
	
	p->lr_user = entry;
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
	__asm("mov %0,r0" : "=r"(current_process->registres[0]));
	__asm("mov %0,r1" : "=r"(current_process->registres[1]));
	__asm("mov %0,r2" : "=r"(current_process->registres[2]));
	__asm("mov %0,r3" : "=r"(current_process->registres[3]));
	__asm("mov %0,r4" : "=r"(current_process->registres[4]));
	__asm("mov %0,r5" : "=r"(current_process->registres[5]));
	__asm("mov %0,r6" : "=r"(current_process->registres[6]));
	__asm("mov %0,r7" : "=r"(current_process->registres[7]));
	__asm("mov %0,r8" : "=r"(current_process->registres[8]));
	__asm("mov %0,r9" : "=r"(current_process->registres[9]));
	__asm("mov %0,r10" : "=r"(current_process->registres[10]));
	__asm("mov %0,r11" : "=r"(current_process->registres[11]));
	__asm("mov %0,r12" : "=r"(current_process->registres[12]));	
	
	__asm("mov %0,lr" : "=r"(current_process->lr_svc));
	//save cpsr_user by copying spsr_svc
	__asm("mrs r0,spsr");
	__asm("mov %0,r0" : "=r"(current_process->cpsr));
	//switch temporarily to System mode to load sp
	__asm("cps 0x1F");
	__asm("mov %0,sp" :"=r"(current_process->sp));
	__asm("cps 0x12");
	current_process->lr_user = (func_t*) lr_irq;

}

void context_load_from_pcb(){
	//copyRegistres();
	//elect();
	//restore context
	__asm("mov r0, %0" : :"r"(current_process->registres[0]));
	__asm("mov r1, %0" : :"r"(current_process->registres[1]));
	__asm("mov r2, %0" : :"r"(current_process->registres[2]));
	__asm("mov r3, %0" : :"r"(current_process->registres[3]));
	__asm("mov r4, %0" : :"r"(current_process->registres[4]));
	__asm("mov r5, %0" : :"r"(current_process->registres[5]));
	__asm("mov r6, %0" : :"r"(current_process->registres[6]));
	__asm("mov r7, %0" : :"r"(current_process->registres[7]));
	__asm("mov r8, %0" : :"r"(current_process->registres[8]));
	__asm("mov r9, %0" : :"r"(current_process->registres[9]));
	__asm("mov r10, %0" : :"r"(current_process->registres[10]));
	__asm("mov r11, %0" : :"r"(current_process->registres[11]));
	__asm("mov r12, %0" : :"r"(current_process->registres[12]));
	//we can't access the user sp register from IRQ
	//switch temporarily to System mode to load sp
	__asm("cps 0x1F");
	__asm("mov sp, %0" : :"r"(current_process->sp));
	__asm("cps 0x12");

}

void yieldto_irq(){
	context_save_to_pcb();
	elect();
	context_load_from_pcb();
}

//IRQ handler
 void __attribute__((naked)) irq_handler(){
	__asm("SUB lr,lr,#4");//subtract 4 otherwise we "skip" an instruction
	__asm("mov %0,lr" : "=r"(lr_irq));
	__asm("stmfd sp!,{r0-r12}");

	context_save_to_pcb();
	elect();
	context_load_from_pcb();
	lr_irq =(uint32_t*) current_process->lr_user;
	//rearm timer
	set_next_tick_default();
	ENABLE_TIMER_IRQ();
	ENABLE_IRQ();
	__asm("ldmfd sp!,{r0-r12}^");
	__asm("cps 0x10");
	__asm("mov pc, %0" : :"r"(lr_irq));
}
