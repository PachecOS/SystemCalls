#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  printf("I am here. ");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();

  // Need to make swtich cases here for each syscall
  // Need to pass pointer to user mem stack pointer
  // Validate the pointers, and parse those args
  // After we get the args from the stack we can then
  // use them for each syscall.
}

/* Terminates Pintos by calling shutdown_power_off() */
void
halt(void)
{
	shutdown_power_off();
}

/* Terminates the current user program */
void
exit (int status) 
{
	struct thread *curr = thread_current();

	if(wait(curr->parent))
	{
		status = curr->cp->status;
	}
	printf("%s: exit(%d)\n", curr->name, status);
	process_exit();
}
/* Runs the executable whose name is given in cmd_line*/
pid_t
exec(const char* cmd_line)
{
	pid_t name = process_execute(cmd_line);
	struct child_p *child = curr_child(name);

	if(load(name, void (**eip) (void), void **esp))
	{

	}


	
}

/* Waits for a child process pid and retrieves the childs
   exit status */
int
wait (pid_t pid) 
{
	return process_wait(pid);
}

/* Creates a new file *file intially sized initial_size
   in bytes */
bool
create (const char *file, unsigned initial_size)
{
	bool created;
	lock_acquire(&lock);
	if(filesys_create(file, intial_size) == true)
	{
		created == true;
		return created;
	} else {
		created == false;
		return created;
	}
	lock_release(&lock);
}

bool
remove (const char *file)
{
	bool removed;
	lock_acquire(&lock);
	if(filesys_remove(file) == true)
	{
		removed == true;
		return removed;
	} else {
		removed == false;
		return removed;
	}
	lock_release(&lock);
}

int
open (const char *file)
{
	lock_acquire(&lock);
	struct file *f = filesys_open(file);
	struct thread *t = thread_current();
	// Space for the file attr struct 
	struct file_attr *fa = malloc(sizeof (struct file_attr));
	fa ->file = f;

	// Need to check if the file attr is valid
	if(!fa) 
	{
		lock_release(&lock);
		return -1;
	}
	// Update the fd's and the thread's fd
	fa->fd = t->fd;
	t->fd++;

	// Put the file elem on that thread's file list
	list_push_back(t->files, fa->elem);
	free(fa);
	lock_release(&lock);
	return fa->fd;
	

}

/* Returns the size, in bytes, of the file open as fd */
int
filesize (int fd)
{
	lock_acquire(&lock);
	for (e = list_begin (&files); e != list_end (&files);
       e = list_next (e))
    {
      struct file_attr *fa = list_entry (e, struct file_attr, elem);
      if(fa->fd == fd) 
      {
      	int size = file_length(fa->elem);
      }
    }
	lock_release(&lock);
	return size;
}

int
read (int fd, void *buffer, unsigned size)
{

}

int
write (int fd, const void *buffer, unsigned size)
{

}

void
seek (int fd, unsigned position)
{

}

unsigned 
tell (int fd)
{

}

void
close (int fd)
{

}



