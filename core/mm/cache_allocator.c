/**************************************************************
 CopyRight     :No
 FileName      :cache_allocator.c
 Author        :Sunli.Wang
 Version       :0.01
 Date          :20171010
 Description   :memory allocator(like slab)
***************************************************************/

#include "cache_allocator.h"
#include "sys_observer.h"
#include "pmm.h"
#include "klibc.h"
#include "task.h"
#include "mutex.h"
#include "log.h"

/*----------------------------------------------
                local data
----------------------------------------------*/
//we use a global list to save all mem_cache in order
//to free mem directly
private struct list_head global_cache_lists;
private task_struct *mem_reclaim_task;
static mutex *cache_lock;

private void reclaim_cache_critical(void *data);
private void *get_content(core_mem_cache *cache,core_mem_cache_node *cache_node,int size);

/*----------------------------------------------
                public method
----------------------------------------------*/
public void cache_allocator_init()
{
    //LOGD("cache_allocator_init \n");
    INIT_LIST_HEAD(&global_cache_lists);

}

public void cache_allocator_start_monitor()
{
    sys_observer_regist(SYSTEM_EVENT_SHRINK_MEM_NORMAL,reclaim_cache_critical);
    sys_observer_regist(SYSTEM_EVENT_SHRINK_MEM_CRITICAL,reclaim_cache_critical);
    cache_lock = create_mutex();
}

/*
 *
 *
 *
 * Cache Node |CacheNode|pmm_stamp|memory|.....|pmm_stamp|mempory|
 *                |                 |                      |
 *                |                 \                      \
 *                |insert list      start_pa               start_pa
 *                |
 *                |
 * Cache      |free_list|full_list|....
 *
 *
 *
 */
public core_mem_cache *creat_core_mem_cache(size_t size)
{
    if(size > CONTENT_SIZE)
    {
        LOGD("too large");
        return NULL;
    }

    core_mem_cache *cache = pmm_kmalloc(sizeof(core_mem_cache));
    kmemset(cache,0,sizeof(core_mem_cache));

    cache->objsize = size;

    INIT_LIST_HEAD(&cache->full_list);
    INIT_LIST_HEAD(&cache->partial_list);
    INIT_LIST_HEAD(&cache->free_list);
    INIT_LIST_HEAD(&cache->lru_free_list);
    //LOGD("add mem_cache is %x ;",cache);
    list_add(&cache->global_ll,&global_cache_lists);
    return cache;
}

public void *cache_alloc(core_mem_cache *cache)
{
    if(!list_empty(&cache->lru_free_list))
    {
        core_mem_cache_content *free_content = list_entry(cache->lru_free_list.next,core_mem_cache_content,ll);

        list_del(&free_content->ll);
        core_mem_cache_node *node = free_content->head_node;
        node->nr_free--;

        if(node->nr_free == 0)
        {
            list_del(&node->list);
            list_add(&node->list,&cache->full_list);//move this node to full list
        }

        kmemset((char *)free_content->start_pa,0,cache->objsize);
        free_content->is_using = CACHE_USING;

        return (void *)free_content->start_pa;
    }

    //we should find whether there are partial cache
    if(!list_empty(&cache->partial_list))
    {
        core_mem_cache_node *new_content = list_entry(cache->partial_list.next,core_mem_cache_node,list);
        return get_content(cache,new_content,cache->objsize);
    }

    core_mem_cache_node *cache_node = NULL;
    if(list_empty(&cache->free_list))
    {
        cache_node = pmm_kmalloc(CONTENT_SIZE);
        kmemset(cache_node,0,CONTENT_SIZE);
        cache_node->cache = cache;
        cache_node->start_pa = (addr_t)cache_node + sizeof(core_mem_cache_node);
        cache_node->end_pa = (addr_t)cache_node + CONTENT_SIZE;
    } else {
        cache_node = list_entry(cache->free_list.next,core_mem_cache_node,list);
        list_del(&cache_node->list);
    }

    //INIT_LIST_HEAD(&cache_node->head);
    list_add(&cache_node->list,&cache->partial_list);
    //we should compute free contents
    cache_node->nr_free = (CONTENT_SIZE - sizeof(core_mem_cache_node))/(sizeof(pmm_stamp) + cache->objsize);

    //start get free memory
    return get_content(cache,cache_node,cache->objsize);
}

public void cache_free(core_mem_cache *cache,addr_t addr)
{
    pmm_stamp *stamp = (pmm_stamp *)(addr - sizeof(pmm_stamp));
    core_mem_cache_content *content = &stamp->cache_content;

    if(content->is_using == CACHE_FREE)
    {
        //LOGD("free agein \n");
        return;
    }
    content->is_using = CACHE_FREE;

    core_mem_cache_node *node = content->head_node;
    node->nr_free++;
    if(node->nr_free == (CONTENT_SIZE - sizeof(core_mem_cache_node))/(sizeof(pmm_stamp) + cache->objsize))
    {
        list_del(&node->list);
        list_add(&node->list,&cache->free_list);
        return;
    }
    else
    {
        //if current node is in full_list,we should move it to partial_list
        list_del(&node->list);
        list_add(&node->list,&cache->partial_list);
    }

    //we add this content to lru list
    list_add(&content->ll,&cache->lru_free_list);
}

public void cache_destroy(core_mem_cache *cache)
{
    //start free all node
    struct list_head *p = cache->full_list.next;

    while(p != NULL && p!= &cache->full_list) {
        core_mem_cache_node *node = list_entry(p,core_mem_cache_node,list);
        p = node->list.next;
        free(node);
    }

    p = cache->partial_list.next;
    while(p != NULL && p!= &cache->partial_list) {
        core_mem_cache_node *node = list_entry(p,core_mem_cache_node,list);
        p = node->list.next;
        free(node);
    }

    p = cache->free_list.next;
    while(p != NULL && p!= &cache->free_list) {
        core_mem_cache_node *node = list_entry(p,core_mem_cache_node,list);
        p = node->list.next;
        free(node);
    }

    list_del(&cache->global_ll);
    free(cache);
}

public uint32_t cache_free_statistic()
{
    //LOGD("cache_free_statistic \n");
    struct list_head *global_p = NULL;
    uint32_t free_size = 0;
    uint32_t partial_size = 0;

    list_for_each(global_p,&global_cache_lists) {
        //first count free_list length
        core_mem_cache *mem_cache = list_entry(global_p,core_mem_cache,global_ll);
        //LOGD("statistic mem_cache is %x ;",mem_cache);
        uint32_t objsize = mem_cache->objsize;

        struct list_head *p = NULL;

        list_for_each(p,&mem_cache->free_list) {
            //LOGD("global cache free_list\n");
            free_size += CONTENT_SIZE;
        }

        list_for_each(p,&mem_cache->partial_list){
          //LOGD("global cache partial_list\n");
            core_mem_cache_node *content = list_entry(p,core_mem_cache_node,list);
            partial_size += content->nr_free*objsize ;
        }
    }
    //LOGD("free_size is %x,partial_size is %x \n",free_size ,partial_size);
    return free_size + partial_size;
}

/*----------------------------------------------
                private method
----------------------------------------------*/
private void reclaim_cache_normal(void *data)
{
    //TODO
}

private void reclaim_cache_critical(void *data)
{
    struct list_head *p = NULL;
    acquire_lock(cache_lock);

    list_for_each(p,&global_cache_lists){
        core_mem_cache *cache = list_entry(p,core_mem_cache,global_ll);
        //release memory
        struct list_head *p1 = cache->free_list.next;
        while(p1 != NULL && p1!= &cache->free_list) {
            core_mem_cache_node *node = list_entry(p1,core_mem_cache_node,list);
            list_del(p1);
            p1 = node->list.next;
            free(node);
        }
    }
    release_lock(cache_lock);
}

private void *get_content(core_mem_cache *cache,core_mem_cache_node *cache_node,int size)
{
    int need_size = size + sizeof(pmm_stamp);
    addr_t start_pa = cache_node->start_pa;
    addr_t end_pa = cache_node->end_pa;

    for(;(start_pa + need_size) < end_pa;start_pa += need_size)
    {
        pmm_stamp *stamp = (pmm_stamp *)start_pa;
        stamp->type = PMM_TYPE_CACHE;

        core_mem_cache_content *content = &stamp->cache_content;
        if(content->is_using == CACHE_FREE)
        {
            cache_node->nr_free--;
            if(cache_node->nr_free == 0)
            {
                list_del(&cache_node->list);
                list_add(&cache_node->list,&cache->full_list);//move this node to full list
            }
            content->head_node = cache_node;
            content->is_using = CACHE_USING;
            content->start_pa = start_pa + sizeof(pmm_stamp);
            return (void *)content->start_pa;
        }
    }

    return NULL;
}
