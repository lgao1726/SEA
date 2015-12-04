#include "syscall.h"

void dummy()
{
	return;
}

int div(int dividend,int divisor)
{
	int result = 0;
	int remainder = dividend;
	while(remainder >= divisor){
		result++;
		remainder -= divisor;
	}
	return result;
}

int __attribute__((naked)) compute_volume(int rad)
{
	int rad3 = rad*rad*rad;
	return div(4*355*rad3,3*113);
}

void __attribute__((naked)) naked_fun()
{
	return;
}



int  kmain(void)
{
/*
	__asm("cps #19");
	__asm("mrs r1,spsr");
	int radius = 5;
	__asm("mov r2,%0" : : "r"(radius));
	__asm("mov %0,r3" : "=r"(radius) );
	int volume;
	volume = compute_volume(radius);
	return volume;	
*/
	__asm("cps 0x10");
	//sys_settime(0x12345678CACACACA);
	sys_gettime();

	//sys_reboot();
	return 0;

}
