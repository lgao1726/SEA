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



void copyRegistres(){
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
	//we can't access the user LR register from SVC
	//switch temporarily to System mode to load LR
	__asm("cps 0x1F");
	__asm("mov %0,lr" :"=r"(current_process->lr_user));
	__asm("mov %0,sp" :"=r"(current_process->sp));
	__asm("cps 0x13");


	
}
/*passe la main a un autre processus*/
void sys_yieldto(struct pcb_s* dest){
	int num = 5;
	__asm("mov r0, %0" : :"r"(num));
	__asm("mov r1, %0" : :"r"(dest));
	__asm("SWI #0");
}

void do_sys_yieldto(){

	__asm("mov %0,r1" : "=r"(current_process));
 	

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
	//we can't access the user LR register from SVC
	//switch temporarily to System mode to load LR
	__asm("cps 0x1F");
	__asm("mov lr, %0" : :"r"(current_process->lr_user));
	__asm("mov sp, %0" : :"r"(current_process->sp));
	__asm("cps 0x13");


}

void sys_yield(){
	int num = 6;
	__asm("mov r0, %0" : : "r"(num));
	__asm("SWI #0");
}

void do_sys_yield(){
	
	copyRegistres();
	elect();
	__asm("mov r1, %0" : :"r"(current_process));
	do_sys_yieldto();
	
}

void __attribute__((naked)) swi_handler(void)
{
	__asm("stmfd sp!,{r0-r12,lr}");	
	int num;
	__asm("mov %0,r0" : "=r"(num));
	if(num == 1){
		do_sys_reboot();
	}else if(num == 2){
		do_sys_nop();
	}else if(num == 3){
		__asm("mov %0,sp" : "=r"(stack));
		do_sys_settime();
	}else if(num == 4){
		__asm("mov %0,sp" : "=r"(stack));
		do_sys_gettime();
	}else if(num == 5){
		do_sys_yieldto();
	}else if(num == 6){
		do_sys_yield();	
	}else{
		PANIC();
	}
	__asm("ldmfd sp!,{r0-r12,pc}^");
}





