#ifndef SYSCALL_H_INCLUDED
#define SYSCALL_H_INCLUDED

#include "util.h"
#include "hw.h"
#include "asm_tools.h"
#include <stdint.h>
#include "sched.h"
void sys_reboot();
void swi_handler(void);
void sys_nop(void);
void sys_settime(uint64_t date_ms);
uint64_t sys_gettime(void);
void sys_yieldto(struct pcb_s* dest);
void sys_yield();
uint32_t sys_exit();

#endif
