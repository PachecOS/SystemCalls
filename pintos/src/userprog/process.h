#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct start_struct {
	char *fn_copy;
	struct semaphore *start_synch;
	bool success;
};

// void start_struct_init(struct start_struct *ss);
#endif /* userprog/process.h */
