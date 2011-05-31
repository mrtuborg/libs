#ifndef SHAREDMEM_H
#define SHAREDMEM_H

extern  int  sharedmem_init   ( int  shared_segment_size);
extern void  sharedmem_deinit ( int  segment_id);
extern char* sharedmem_attach ( int  segment_id);
extern void  sharedmem_detach (void* shared_memory);

#endif
