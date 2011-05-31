#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "stdio.h"
#include "sharedmem.h"



int sharedmem_init(int shared_segment_size)
{
	/* Allocate a shared memory segment. */
	return shmget (IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
}

void sharedmem_deinit(int segment_id)
{
	/* Deallocate the shared memory segment. */
	shmctl (segment_id, IPC_RMID, 0);
}

char* sharedmem_attach(int segment_id)
{
	           char* shared_memory = 0;
	struct shmid_ds  shmbuffer;
	            int  segment_size;

	/* Attach the shared memory segment. */
	shared_memory = (char*) shmat (segment_id, 0, 0);
	/*printf ("shared memory attached at address %p\n", shared_memory);*/

	/* Determine the segment’s size. */
	shmctl (segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	/*printf ("segment size: %d\n", segment_size);*/
	return shared_memory;
}

void sharedmem_detach(void* shared_memory)
{
	/* Detach the shared memory segment. */
	shmdt (shared_memory);
}
