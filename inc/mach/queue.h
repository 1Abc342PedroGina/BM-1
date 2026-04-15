/* SPDX License Indentifier: GPL-2.0 WITH Mach-Syscall-Exceptions */
#ifndef MACH_QUEUE_H
#define MACH_QUEUE_H

#include <mach/vm_types.h>
#include <mach/kern_return.h>
#include <mach/boolean.h>
#include <mach/time_value.h>
#include <mach/message.h>
#ifdef __x86_64__
#include <i386/thread.h>
#elif defined(__aarch64__) || defined(__arm64__)
#include <arm/thread.h>
#else 
#error "This kernel is not compatibble for others architerture "
#endif

struct queue_entry {
	struct queue_entry	*next;		/* next element */
	struct queue_entry	*prev;		/* previous element */
};
 
typedef struct queue_entry	*queue_t;
typedef	struct queue_entry	queue_head_t;
typedef	struct queue_entry	queue_chain_t;
typedef	struct queue_entry	*queue_entry_t;

struct slock {
	volatile natural_t lock_data;	/* in general 1 bit is sufficient */
};

typedef struct slock	simple_lock_data_t;
typedef struct slock	*simple_lock_t;


#define	decl_simple_lock_data(class,name) \
class	simple_lock_data_t	name;

#define	simple_lock_addr(lock)	(&(lock))

typedef	void	*event_t;			/* wait event */


typedef unsigned int ipc_object_refs_t;
typedef unsigned int ipc_object_bits_t;
typedef unsigned int ipc_object_type_t;

typedef struct ipc_object {
	decl_simple_lock_data(,io_lock_data)
	ipc_object_refs_t io_references;
	ipc_object_bits_t io_bits;
} *ipc_object_t;

#endif
