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

typedef mach_port_t mach_port_index_t;		/* index values */
typedef mach_port_t mach_port_gen_t;		/* generation numbers */


typedef struct ipc_entry {
	ipc_entry_bits_t ie_bits;
	struct ipc_object *ie_object;
	union {
		mach_port_index_t next;
		ipc_port_request_index_t request;
	} index;
	union {
		mach_port_index_t table;
		struct ipc_tree_entry *tree;
	} hash;
} *ipc_entry_t;


typedef unsigned int ipc_table_index_t;	/* index into tables */
typedef unsigned int ipc_table_elems_t;	/* size of tables */

typedef struct ipc_table_size {
	ipc_table_elems_t its_size;	/* number of elements in table */
} *ipc_table_size_t;


typedef unsigned int ipc_port_timestamp_t;

typedef struct ipc_tree_entry {
	struct ipc_entry ite_entry;
	mach_port_t ite_name;
	struct ipc_space *ite_space;
	struct ipc_tree_entry *ite_lchild;
	struct ipc_tree_entry *ite_rchild;
} *ipc_tree_entry_t;

typedef struct ipc_splay_tree {
	mach_port_t ist_name;		/* name used in last lookup */
	ipc_tree_entry_t ist_root;	/* root of middle tree */
	ipc_tree_entry_t ist_ltree;	/* root of left tree */
	ipc_tree_entry_t *ist_ltreep;	/* pointer into left tree */
	ipc_tree_entry_t ist_rtree;	/* root of right tree */
	ipc_tree_entry_t *ist_rtreep;	/* pointer into right tree */
} *ipc_splay_tree_t;

typedef struct ipc_space {
	decl_simple_lock_data(,is_ref_lock_data)
	ipc_space_refs_t is_references;

	decl_simple_lock_data(,is_lock_data)
	boolean_t is_active;		/* is the space alive? */
	boolean_t is_growing;		/* is the space growing? */
	ipc_entry_t is_table;		/* an array of entries */
	ipc_entry_num_t is_table_size;	/* current size of table */
	struct ipc_table_size *is_table_next; /* info for larger table */
	struct ipc_splay_tree is_tree;	/* a splay tree of entries */
	ipc_entry_num_t is_tree_total;	/* number of entries in the tree */
	ipc_entry_num_t is_tree_small;	/* # of small entries in the tree */
	ipc_entry_num_t is_tree_hash;	/* # of hashed entries in the tree */
} *ipc_space_t;

typedef vm_offset_t ipc_kobject_t;


typedef unsigned int ipc_kobject_type_t;

#endif
