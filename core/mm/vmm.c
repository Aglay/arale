/******************************************************************************
 *      Virtual Memory Manager
 *
 ******************************************************************************/

#include <klibc.h>
#include <error.h>
#include "mm.h"
#include "vmm.h"

void *vmm_vmalloc(mm_struct *pd,size_t bytes);
void *vmm_kmalloc(mm_struct *pd,size_t bytes);
void *vmm_malloc(mm_struct *pd,size_t bytes);

#define SIZE_TO_PGD(b) \
    ((b) / (1024*1024*4))

#define SIZE_TO_PTE(b)  \
    ((b/(1024*4)) % (1024) )

#define PDT_FIND 1
#define PDT_LOSS 2


int scan_start_pgd = 0;
int scan_start_pte = 0;

void dealloc(mm_struct *mm,void *ptr);

static void load_pd(addr_t pde)
{
     __asm__ volatile ("mov %0, %%cr3": :"r"(pde));
}

mm_struct *get_root_pd() {
    return &core_mem;
}

static void *alloc_bytes(mm_struct *mm, size_t b, enum mem_area area)
{
    int block = bytes_to_blocks(b);

    addr_t pgd = 0;
    addr_t pte = 0;


    char *mem_ptr = NULL;//mm->mem_map;
    addr_t *ptem = NULL;
    //we should do get find range(core or usr)
    uint32_t offset = 0;
    int PGD_NUM = 0;

    switch(area) {
        case MEM_USR:
            pgd = 0;
            pte = 0;
            mem_ptr = mm->user_mem_map;
            ptem = mm->pte_core;
            //offset = memory_range_user.start_pgd *1024 + memory_range_user.start_pte;
            PGD_NUM = PD_ENTRY_CNT*3/4;
            break;

        case MEM_CORE:
            pgd = memory_range_core.start_pgd;
            pte = memory_range_core.start_pte;
            mem_ptr = mm->core_mem_map;
            ptem = mm->pte_user;
            PGD_NUM = PD_ENTRY_CNT/4;
            break;
    }

    printf("alloc_bytes,pgd is %x,pte is %x,PGD_NUM is %x \n",pgd,pte,PGD_NUM);
    //find block

    int find_block = 0;
    int hit = PDT_LOSS;

    addr_t start_pgd = -1;
    addr_t start_pte = -1;

    int i = 0;

    //printf("set 2 mem_ptr is %d \n",mem_ptr[2562]);

    for(;pgd < PGD_NUM;pgd++)
    {
        //because 0<<22|0<<12 is still 0
        //so we start from pte 1
        if(pgd == 0) {
            pte = 1;
        } else if(pte == PD_ENTRY_CNT){
            pte = 0;
        }

        for(;pte < PD_ENTRY_CNT;pte++) {
            int bit = get_bit(mem_ptr,pgd*PD_ENTRY_CNT + pte);
            if(bit == 0) {
               find_block++;
               if(start_pgd == -1) {
                   start_pgd = pgd;
                   start_pte = pte;
               }
            } else {
               //printf("wangsl,not fit pgd is %d,pte is %d,mm->mem_map[pgd][pte] is %d \n",pgd,pte,mm->mem_map[pgd][pte]);
               find_block = 0;
               start_pgd = -1;
               start_pte = -1;
            }

            if(find_block == block) {
                hit = PDT_FIND;
                break;
            }
        }

        if(hit == PDT_FIND)
        {
            break;
        }
    }

    //printf("mm is %x,start pte is %d \n",mm,start_pte);
    //printf("mm is %x,start pgd %d \n",mm,start_pgd);
    //printf("mm is %x,find pte is %d \n",mm,pte);
    //printf("mm is %x,find pgd is %d \n",mm,pgd);

    //addr_t *ptem = (addr_t *)mm->pte_kern;
    addr_t mem = 0;
    printf("start i is %x,end i is %x \n",PD_ENTRY_CNT*start_pgd + start_pte,PD_ENTRY_CNT*pgd + pte);

    for (i = PD_ENTRY_CNT*start_pgd + start_pte; i <= PD_ENTRY_CNT*pgd + pte; i++) {
        //int page = 0;
        if(b/PAGE_SIZE == 0) {
            mem = pmm_alloc(b);
            //page = b;
        } else {
            mem = pmm_alloc(PAGE_SIZE);
            //page = PAGE_SIZE;
        }
        //goto_xy(10,10);
        //printf("index i is %d ,mem alloc is %x,page is %d",i,mem,page);
        ptem[i] = mem | ENTRY_PRESENT | ENTRY_RW | ENTRY_SUPERVISOR;
        //mem_ptr[i] |= ENTRY_PRESENT;
        set_bit(mem_ptr,i,1);
        b -= PAGE_SIZE;
        if(b < 0) {
            break;
        }
    }

    printf("alloc start_pgd is %d,start_pte is %d \n",start_pgd,start_pte);

    if(area == MEM_USR) {
        start_pgd += memory_range_user.start_pgd;
    }
    return (start_pgd<<22) | (start_pte<<12);
}

int vmm_init(size_t mem_kb, addr_t krnl_bin_end,size_t reserve)
{
    addr_t i;
    addr_t user_index = 0;

    //we should also update kernel memory range
    memory_range_core.start_pgd = SIZE_TO_PGD(reserve);
    memory_range_core.start_pte = SIZE_TO_PTE(reserve);

    //we should init memory range
    addr_t user_start_memory = 1024*1024*1024; //may be align 4K

    memory_range_user.start_pgd = SIZE_TO_PGD(user_start_memory);
    memory_range_user.start_pte = SIZE_TO_PTE(user_start_memory); //4K for gully

    printf("user_start_memory is %x,memory_range_user.start_pgd is %x,start_pte is %x \n",
    user_start_memory,memory_range_user.start_pgd,
    memory_range_user.start_pte);
    core_mem.pgd = &process_core_pgd;
    core_mem.pte_core = &process_core_pte;

#ifdef CORE_PROCESS_USER_SPACE
    core_mem.pte_user = &process_user_pte;
#endif
    //printf("scan_start_pgd is %d scan_start_pte is %d \n",scan_start_pgd,scan_start_pte);

    //map 4G memory, physcial address = virtual address
    for (i = 0; i < memory_range_user.start_pgd; i++) {
        core_mem.pgd[i] = (addr_t)&core_mem.pte_core[i*PD_ENTRY_CNT] | ENTRY_PRESENT | ENTRY_RW | ENTRY_SUPERVISOR;
    }
    //printf("i1 is %x",i);

#ifdef CORE_PROCESS_USER_SPACE
    for(i = memory_range_user.start_pgd; i < PD_ENTRY_CNT; i++) {
        core_mem.pgd[i] = (addr_t)&core_mem.pte_user[user_index*PD_ENTRY_CNT] | ENTRY_PRESENT | ENTRY_RW | ENTRY_SUPERVISOR;
        user_index++;
    }
#endif
    //printf("i2 is %x,user_index is %x \n",i,user_index);


    //because there is no user memory in core process,
    //needn't init user core memory in core process~
    addr_t *pte = (addr_t *)core_mem.pte_core;
    core_mem.core_mem_map = &core_mem_reserve_map;
    addr_t core_init_end = memory_range_user.start_pgd*PD_ENTRY_CNT + memory_range_user.start_pte;

    for (i = 0; i < PD_ENTRY_CNT*PT_ENTRY_CNT/4; i++) {
        pte[i] = (i << 12) | ENTRY_PRESENT | ENTRY_RW | ENTRY_SUPERVISOR; // i是页表号
        set_bit(core_mem.core_mem_map,i,0);
    }
//printf("i3 is %x",i);
#ifdef CORE_PROCESS_USER_SPACE
    pte = (addr_t *)core_mem.pte_user;
    for(i = 0;i<PD_ENTRY_CNT*PT_ENTRY_CNT*3/4;i++) {
        pte[i] = ((i + PD_ENTRY_CNT*PT_ENTRY_CNT/4)<< 12) | ENTRY_PRESENT | ENTRY_RW | ENTRY_SUPERVISOR; // i是页表号
    }
#endif
//printf("i4 is %x",i);

    load_pd((addr_t)core_mem.pgd);
    enable_paging();

    //reconfig struct mm
    mm_operation.vmalloc = vmm_vmalloc;
    mm_operation.kmalloc = vmm_kmalloc;
    mm_operation.malloc = vmm_malloc;

    mm_operation.free = dealloc;

    return 0;
}

/*
 * Every freshly allocated memory chunk will have the first DW
 * reserved for keeping track the allocation size in bytes,
 * which will be used when calling free().
 */
inline static void *mark_size(void *mem, size_t bytes)
{
    *((int *) mem) = bytes;

    return mem + MEM_MARK_SIZE;
}

inline static void *unmark_size(void *mem)
{
    return mem - MEM_MARK_SIZE;
}

inline static size_t get_mark_size(void *mem)
{
    return *((int *) (mem - MEM_MARK_SIZE));
}


/*
 * Frees previously allocated memory chunk.
 */
void dealloc(mm_struct *mm,void *ptr)
{
    size_t b = get_mark_size(ptr);
    //printf("dealloc1 ptr is %x \n",ptr);

    if (!b)
        return;

    ptr = unmark_size(ptr);
    //printf("dealloc2 ptr is %x \n",ptr);
    dealloc_bytes(mm,ptr, b);
}

void dealloc_bytes(mm_struct *mm,void *ptr,size_t size) {
    //we should make virtual memroy to dirty
    int start_pgd = va_to_pt_idx((size_t)ptr);
    int start_pte = va_to_pte_idx((size_t)ptr);

    int end_pgd = va_to_pt_idx((size_t)ptr + size);
    int end_pte = va_to_pte_idx((size_t)ptr + size);

#if 0 //TODO
    char *mem_ptr = mm->mem_map;
    addr_t *ptem = (addr_t *)mm->pte_kern;
    int i = PD_ENTRY_CNT*start_pgd + start_pte;
    //printf("ptr is %x,dealloc_bytes pgd is %d,start_pte is %d \n",ptr,start_pgd,start_pte);
    for (;i <= PD_ENTRY_CNT*end_pgd + end_pte; i++) {
        addr_t pmm = (ptem[i]&0xFFFF000);
        ptem[i] == pmm|ENTRY_PAGE_DIRTY;

        set_bit(mem_ptr,i,0);
        pmm_dealloc_page(pmm);
    }
#endif //TODO
}


void *vmm_malloc(mm_struct *mm,size_t bytes)
{
    void *va;

    bytes += MEM_MARK_SIZE;
    va = alloc_bytes(mm, bytes, MEM_USR);
    if (!va)
        return 0;
    va = mark_size(va, bytes);
    return va;
}


/*
 * Allocates `bytes` sized memory chunk in kernel space.
 */
void *vmm_kmalloc(mm_struct *mm,size_t bytes)
{
    void *va;

    bytes += MEM_MARK_SIZE;
    va = alloc_bytes(mm, bytes, MEM_CORE);
    if (!va)
        return 0;
    va = mark_size(va, bytes);
    return va;
}

/*
 * Allocates `bytes` sized memory chunk in user space.
 */
void *vmm_vmalloc(mm_struct *mm,size_t bytes)
{
    void *va;

    bytes += MEM_MARK_SIZE;
    va = alloc_bytes(mm, bytes, MEM_CORE);
    if (!va)
        return 0;
    va = mark_size(va, bytes);
    return va;
}


void enable_paging()
{
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0" : : "r" (cr0));
}

void disable_paging()
{
    __asm__ __volatile__("mov %%cr0, %%eax \n"
                         "and %0, %%eax \n"
                         "mov %%eax, %%cr0 \n"
                    :
                    : "n" (~CR0_ENABLE_PAGING)
                    : "eax");
}
