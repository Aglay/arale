#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include "list.h"

typedef struct semaphore {
    struct list_head wait_list;
    uint32_t count;
}semaphore;

semaphore *sem_create();
void sem_down(semaphore *semaphore);
void sem_up(semaphore *sem);
void sem_up_all(semaphore *semaphore);
void sem_destroy(semaphore *semaphore);

#endif