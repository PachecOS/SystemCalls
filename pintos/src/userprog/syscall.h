#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "threads/synch.h"

void syscall_init (void);
void halt (void);
void exit (int status);
pid_t exec (const char *cmd_line);
int wait (pid_t pid);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);

struct child_p {
	int load;
	int pid;
	bool wait;
	bool exit;
	int status;
	struct list_elem elem;
	struct lock *lock;
};

struct file_attr {
	int fd;
	struct file *file;
};

#endif /* userprog/syscall.h */


/*
QUESTIONS FOR MYSELF:
- Where do we create the monitors?
- How are monitors used in exec/wait?
- How to set up stack?
- Where do we keep track of file descriptors?
- How do we put our children list in to our thread?
- How to synchornize properly?
- 