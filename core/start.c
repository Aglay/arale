#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
//#include "string.h"
#include "global.h"
#include "color.h"
#include "font.h"
#include "mm.h"
#include "vmm.h"
#include "cpu.h"
#include "task.h"
#include "gdt.h"
#include "cache_allocator.h"
#include "time.h"
#include "fs.h"
#include "sysclock.h"
#include "test_fs.h"

#define GUARD_TEST

extern void init_font();
extern void init_graphic();
extern void start_refresh();
extern void hdd_init();

static int screen_init()
{
    set_color(VID_CLR_BLACK, VID_CLR_LIGHT_BLUE);
    clear_screen();
    goto_xy(0, 0);

    return 0;
}


void start_core(struct boot_info bootinfo)
{
    struct boot_info * binfo = &bootinfo;

    screen_init();

    init_gdt();

    x86_init();

    mm_init(binfo);

    init_sysclock();

    task_init(binfo);

    hdd_init();

    fs_init();

    start_sysclock();

    init_timer();

#ifdef GUARD_TEST
    start_test();
#endif

    kprintf("start successfully!!!!!! \n");

    while(1){}
}
