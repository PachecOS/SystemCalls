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
  int arg[MAX_ARGS];
  switch(*(int *) esp)
  {
  	case system_halt:
  	{
  		halt();
  		break;
  	}
  	case system_exit:
  	{
  		exit(arg[0]);
  		break;
  	}
  	case system_exec:
  	{
  		break;
  	}
  	case system_wait:
  	{
  		f->eax = wait(arg[0]);
  		break;
  	}
  	case system_remove:
  	{
  		f->eax = remove((const char*) arg[0]);
  		break;
  	}
  	case system_open:
  	{
  		f->eax = open((const char *) arg[0]);
  		break;
  	}
  	case system_filesize:
  	{
  		f->eax = filesize(arg[0]);
  		break;
  	}
  	case system_read:
  	{
  		f->eax = read(arg[0], (void *) arg[1], (unsigned) arg[2]);
  		break;
  	}
  	case system_write:
  	{
  		f->eax = write(arg[0], (const void *) arg[1], (unsigned) arg[2]);
  		break;
  	}
  	case system_seek:
  	{
  		seek(arg[0], (unsigned) arg[1]);
  		break;
  	}
  	case system_tell:
  	{
  		f->eax = tell(arg[0]);
  		break;
  	}
  	case system_close:
  	{
  		close(arg[0]);
  		break;
  	}

  }

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

/* Reads size bytes from the file open as fd into buffer.
   Returns the number of bytes actually read (0 at the
   end of a file), or -1 if the file could not be read.
   Fd 0 reads from the keyboard using input_getc(). */
int
read (int fd, void *buffer, unsigned size)
{

	lock_acquire(&lock);

	if (fd == 0) 
	{
		int i = 0;
		uint8_t* buff = (uint8_t *) buffer;
	
		while (i < size) 
		{
			//devices/input.h
			buff[i] = input_getc();

			i++;

			if (i >= size) {
				break;
			}
		}
	}
	else 
	{
		struct file_attr *fa;
		struct list_elem *e;
		int bytes = 0;

		for (e = list_begin (&files); e != list_end (&files);
		 	 e = list_next (e))
		{
			fa = list_entry (e, struct file_attr, elem);
			
			if (fa->fd == fd)
			{
				bytes = file_read(&fa->file, buffer, size);
				return bytes;
			}
		}	
	}
	lock_release(&lock);

	return -1;
}

int
write (int fd, const void *buffer, unsigned size)
{

}

void
seek (int fd, unsigned position)
{
	lock_acquire(&lock);

	struct file_attr *fa;
	struct list_elem *e;

	for (e = list_begin (&files); e != list_end (&files);
		 e - list_next (e))
	{
		fa = list_entry (e, struct file_attr, elem);
		if (fa->fd == fd)
		{
			file_seek(fa->file, position);
		}
	}


	lock_release(&lock);
}

/* Returns the position of the next byte to be read
   or written in open file fd, expressed as in bytes
   from the beginning of the file. */
unsigned 
tell (int fd)
{
	lock_acquire(&lock);
	struct file *f;
	// Get the file attributed to this fd
	f = get_file(fd);
	if(f != NULL) 
	{
		// file_tell gets the offset of the next byte
		off_t next = file_tell(f->file);
	}
	lock_release(&lock);
	return next;
	

}

struct 
file* get_file (int fd)
{
	struct threat *t = thread_current();
	struct list_elem *e;

	for(e = list_begin(&files); e != list_end(&files); e = list_next(e))
	{
		struct file_attr fa = list_entry(e, struct file_attr, elem);
		if(fa->fd == fd)
		{
			return fa->file;
		} 
		else 
		{
			return NULL;
		}
	}
}

void
close (int fd)
{

	lock_acquire(&lock);

	struct file_attr *fa;

	for(e = list_begin(&files); e != list_end(&files); e = list_next(e)) 
	{
		fa = list_entry(e, struct file_attr, elem);

		if ((fa->fd == fd) || (fa->fd == CLOSE))
		{
			file_close(fa->file);
			list_remove(&fa->elem);
		}
	}


	lock_release(&lock);

}



