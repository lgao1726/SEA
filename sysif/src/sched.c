#include "sched.h"
/*pointe le pcb current_process vers kmain*/
void sched_init(){
	current_process = &kmain_process;
	list_pointer = &kmain_process;
	kheap_init();
}

void create_process(func_t* entry){
	struct pcb_s* p =(struct pcb_s*)  kAlloc(sizeof(struct pcb_s));
	
	p->lr_user = entry;
	//initialize a stackof size 10KB
	p->stack = (uint32_t*) kAlloc(10000);
	p->sp = (p->stack+10000);

	list_pointer->next_prc = p;
	p->next_prc = kmain_process.next_prc;
	list_pointer = p;		
}

void elect(){

	current_process = current_process->next_prc;

}
