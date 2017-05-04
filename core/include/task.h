#include "vmm.h"
#include "mm.h"

#ifndef _TASK_STRUCT_H_
#define _TASK_STRUCT_H_

#define TASK_GDT0 3 //because the first 2 GDT is defined in stage2.asm
#define TASK_MAX 2048

int current_task; //the default pid is 0

//#define GET_CURRENT_TASK() \
//{\
//    return &task_table[current_task]; \
//}

struct task_struct {
    int pid;

    mm_struct *mm;
    struct tss_struct *tss;

    int prio;
    int state;
};

struct task_struct task_table[TASK_MAX];

struct tss_struct {
    int backlink;
    int esp0;
    int ss0;
    int esp1;
    int ss1;
    int esp2;
    int ss2;
    int cr3;
    int eip;
    int eflags;
    int eax;
    int ecx;
    int edx;
    int ebx;
    int esp;
    int ebp;
    int esi;
    int edi;
    int es;
    int cs;
    int ss;
    int ds;
    int fs;
    int gs;
    int ldtr;
    int iomap;
};

struct task_struct *GET_CURRENT_TASK();
void task_set_root();
void task_init(struct boot_info *binfo);


#endif