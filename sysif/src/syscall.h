#ifndef SYSCALL_H
#define SYSCALL_H

#include "util.h"
#include "hw.h"
#include <stdint.h>
#include "sched.h"
void sys_reboot();
void swi_handler(void);
void sys_nop(void);
void sys_settime(uint64_t date_ms);
uint64_t sys_gettime(void);
void sys_yieldto(struct pcb_s* dest);
void do_sys_yieldto();
void sys_yield();
void do_sys_yield();

#endif


