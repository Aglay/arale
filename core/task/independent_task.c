#include "dependent_task.h"
#include "task.h"
#include "mm.h"
#include "mmzone.h"
#include "vm_allocator.h"

task_struct *create_independent_task()
{
    addr_t i = 0;
    int index = 0;
    task_struct *task = (task_struct *)kmalloc(sizeof(task_struct));

    task->ticks = DEFAULT_TASK_TICKS;
    task->status = TASK_STATUS_INIT;
    task->pid = task_id;
    task_id++;

    //create memory struct
    mm_struct *_mm = (mm_struct *)kmalloc(sizeof(mm_struct)); //struct need physical address
    _mm->pte_user = (addr_t *)pmalloc(sizeof(addr_t)*PD_ENTRY_CNT*PT_ENTRY_CNT*3/4);
    _mm->pgd = (addr_t *)pmalloc(sizeof(addr_t) * PD_ENTRY_CNT);//??
    _mm->userroot = vm_allocator_init(1024*1024*1024,(uint32_t)1024*1024*1024*3); //user space is 1~3G
    _mm->vmroot = vm_allocator_init(zone_list[ZONE_HIGH].start_pa,1024*1024*1024*1 - zone_list[ZONE_HIGH].start_pa);
    //_mm->pgd = core_mem.pgd;
    _mm->pte_core = core_mem.pte_core;

    //core memory is always same~~~~~
    for (i = 0; i < memory_range_user.start_pgd; i++) {
        _mm->pgd[i] = core_mem.pgd[i];
    }

    //kprintf("task init end i is %d, start i is %d \n",i,memory_range_user.start_pgd);
    int user_index = 0;
    //because kmalloc is continus memory,so there is no need to
    //compute pa again.
    for(i = memory_range_user.start_pgd; i < PD_ENTRY_CNT; i++) {
        _mm->pgd[i] = (addr_t)&_mm->pte_user[user_index*PD_ENTRY_CNT] | ENTRY_PRESENT | ENTRY_RW | ENTRY_SUPERVISOR;
        user_index++;
    }
    //kprintf("task alloc 4 \n");
    addr_t *_pte = (addr_t *)_mm->pte_user;
    for (i = 0; i < PD_ENTRY_CNT*PT_ENTRY_CNT*3/4; i++) {
         //goto_xy(20,20);
         //kprintf("task alloc i is %x \n",i);
        _pte[i] = (i << 12) | ENTRY_PRESENT | ENTRY_RW | ENTRY_SUPERVISOR; // i是页表号
    }
    //kprintf("task alloc 5 \n");
    task->mm = _mm;
    task->context = (context_struct *)kmalloc(sizeof(context_struct));
    kmemset(task->context,0,sizeof(context_struct));

    return task;
}
