#include "syscall.h"
#include "fb.h"
struct pcb_s pcb1,pcb2;

struct pcb_s *p1,*p2;

void user_process_1()
{
	int v1=5;
	while(1)
	{
		v1++;
	}
}

void user_process_2()
{
	int v2=-12;
	while(1)
	{
	v2-=2;
	}
}

void user_process_3(){
	int v3 = 0;
	while(1){
		v3 += 5;
	}
	sys_exit();
}


int  kmain(void)
{
	/*sched_init();
	
	//initialise processes
	create_process((func_t*)&user_process_1);
	create_process((func_t*)&user_process_2);	
	create_process((func_t*)&user_process_3);

	timer_init();
	ENABLE_IRQ();

	__asm("cps 0x10"); //switch CPU to USER mode
	while(1){
		sys_yield();
	}	*/
	UsbInitialise();
	FramebufferInitialize();
	
	char c;
	while(1){	
		KeyboardUpdate();
		c = KeyboardGetChar();
		
		switch(c){
			case 'a' :
				drawRed();
				break;
			case 's' :
				drawBlue();
				break;
			case 'd' :
				drawGreen();
				break;
			case 'f' :
				drawWhite();
				break;
			case 'g' :
				drawTest();
				break;
				
			default :
				;
		}
	}
	return 0;

}
