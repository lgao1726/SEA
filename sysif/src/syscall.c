#include "syscall.h"


uint32_t *stack;

void sys_reboot()
{
	int num = 1;
	__asm("mov r0, %0" : :"r"(num));
	__asm("SWI #0");	
}

void do_sys_reboot()
{
	int reset = 0x0000;
	__asm("mov pc, %0" : :"r"(reset));
}

void sys_nop(void){
	int num = 2;
	
	__asm("mov r0, %0" : :"r"(num));
	__asm("SWI #0");
	return;
}

void do_sys_nop(void){
	return;
}

void sys_settime(uint64_t date_ms){
	int num = 3;
	__asm("mov r0, %0" : :"r"(num));
	uint32_t part1;
	uint32_t part2;
	part1 = (uint32_t)((date_ms & 0xFFFFFFFF00000000) >> 32);
	part2 = (uint32_t)(date_ms & 0xFFFFFFFF);
	__asm("mov r1, %0" : :"r"(part1));
	__asm("mov r2, %0" : :"r"(part2));
	__asm("SWI #0");
}

void do_sys_settime(){
	uint64_t date_ms;
	uint32_t part1 = *(stack+1);
	uint32_t part2 = *(stack+2);
	
	
	date_ms = part1;
	date_ms = (date_ms << 32) | part2;
	set_date_ms(date_ms);
	return;
}

uint64_t sys_gettime(void)
{
	int num = 4;

	uint64_t date_ms;
	__asm("mov r0, %0" : :"r"(num));
	__asm("SWI #0");
	uint32_t part1 = *(stack+1);
	uint32_t part2 = *(stack+2);
	
	date_ms = part1;
	date_ms = (date_ms << 32) | part2;
	return date_ms;
	
}

void do_sys_gettime(void)
{
	uint64_t date_ms = get_date_ms();
	uint32_t part1, part2;
	part1 = (uint32_t)((date_ms & 0xFFFFFFFF00000000) >> 32);
	part2 = (uint32_t)(date_ms & 0xFFFFFFFF);
	*(stack+1) = part1;
	*(stack+2) = part2;
	return;
}

/*dispatcher that schedules processes*/
void sys_yield(){
	int num = 6;	
	__asm("mov r0, %0" : :"r"(num));
	__asm("SWI #0");

}


/*passe la main a un autre processus*/
void sys_yieldto(struct pcb_s* dest){
	int num = 5;
	__asm("mov r0, %0" : :"r"(num));
	__asm("mov r1, %0" : :"r"(dest));
	__asm("SWI #0");
}

void do_sys_yieldto(){
	context_save_to_pcb();
	elect();
	context_load_from_pcb();
}

//terminate the process  
uint32_t sys_exit(){
	int num = 7;
	__asm("mov r0, %0" : :"r"(num));
	__asm("SWI #0");
	return 0;
}

void do_sys_exit(){
	struct pcb_s* to_delete = current_process;
	to_delete->status = PROCESS_TERMINATED;
	do_sys_yieldto();
	int terminated = 0;
	if(to_delete==current_process){
		terminated = 1;
	}

	to_delete->prev_process->next_process = to_delete->next_process;	 to_delete->next_process->prev_process = to_delete->prev_process;

	kFree((void*)to_delete,10000);
	kFree((void*)to_delete,sizeof(struct pcb_s));
	
	if(terminated==1){
		terminate_kernel();
	}	
	
}

void __attribute__((naked)) swi_handler(void)
{
	
	
	__asm("stmfd sp!,{r0-r12,lr}");
	__asm("mov %0,sp" : "=r"(stack));	
	int num;
	__asm("mov %0,r0" : "=r"(num));
	if(num == 1){
		do_sys_reboot();
	}else if(num == 2){
		do_sys_nop();
	}else if(num == 3){
		do_sys_settime();
	}else if(num == 4){
		do_sys_gettime();
	}else if(num == 5){
		do_sys_yieldto();	
	}else if(num ==6){
		do_sys_yieldto();
	}else if(num == 7){
		do_sys_exit();
	}else{
		PANIC();
	}
	__asm("ldmfd sp!,{r0-r12,pc}^");
}




