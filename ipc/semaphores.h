#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

union m_semun
{
	   int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

extern int binary_semaphore_allocation (key_t key, int sem_flags);
extern int binary_semaphore_deallocate (int semid);
extern int binary_semaphore_initialize (int semid);
extern int binary_semaphore_wait (int semid);
extern int binary_semaphore_post (int semid);

#endif
