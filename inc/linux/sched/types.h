/* SDPX License Indentifier: GPL-2.0-only */
#ifndef LINUX_SCHED_TYPES_H
#define LINUX_SCHED_TYPES_H

#include <kern/thread.h>
#include "rbtree.h"

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef int ssize_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef long time_t;
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef long clock_t;
#endif

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef int ptrdiff_t;
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

typedef int pid_t;
typedef unsigned short uid_t;
typedef unsigned short gid_t;
typedef unsigned short dev_t;
#ifdef OLD_LINUX
typedef unsigned short ino_t;
#else
typedef unsigned long ino_t;
#endif
typedef unsigned short mode_t;
typedef unsigned short umode_t;
typedef unsigned short nlink_t;
typedef int daddr_t;
typedef long off_t;

/* bsd */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

/* sysv */
typedef unsigned char unchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef char *caddr_t;

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned long tcflag_t;

/*
 * This allows for 256 file descriptors: if NR_OPEN is ever grown beyond that
 * you'll have to change this too. But 256 fd's seem to be enough even for such
 * "real" unices like SunOS, so hopefully this is one limit that doesn't have
 * to be changed.
 *
 * Note that POSIX wants the FD_CLEAR(fd,fdsetp) defines to be in <sys/time.h>
 * (and thus <linux/time.h>) - but this is a more logical place for them. Solved
 * by having dummy defines in <sys/time.h>.
 */

/*
 * Those macros may have been defined in <gnu/types.h>. But we always
 * use the ones here. 
 */
#undef __FDSET_LONGS
#define __FDSET_LONGS 8

typedef struct fd_set {
	unsigned long fds_bits [__FDSET_LONGS];
} fd_set;

#undef __NFDBITS
#define __NFDBITS	(8 * sizeof(unsigned long))

#undef __FD_SETSIZE
#define __FD_SETSIZE	(__FDSET_LONGS*__NFDBITS)

#undef	__FD_SET
#define __FD_SET(fd,fdsetp) \
		__asm__ __volatile__("btsl %1,%0": \
			"=m" (*(fd_set *) (fdsetp)):"r" ((int) (fd)))

#undef	__FD_CLR
#define __FD_CLR(fd,fdsetp) \
		__asm__ __volatile__("btrl %1,%0": \
			"=m" (*(fd_set *) (fdsetp)):"r" ((int) (fd)))

#undef	__FD_ISSET
#define __FD_ISSET(fd,fdsetp) (__extension__ ({ \
		unsigned char __result; \
		__asm__ __volatile__("btl %1,%2 ; setb %0" \
			:"=q" (__result) :"r" ((int) (fd)), \
			"m" (*(fd_set *) (fdsetp))); \
		__result; }))

#undef	__FD_ZERO
#define __FD_ZERO(fdsetp) \
		__asm__ __volatile__("cld ; rep ; stosl" \
			:"=m" (*(fd_set *) (fdsetp)) \
			:"a" (0), "c" (__FDSET_LONGS), \
			"D" ((fd_set *) (fdsetp)) :"cx","di")

struct ustat {
	daddr_t f_tfree;
	ino_t f_tinode;
	char f_fname[6];
	char f_fpack[6];
};

typedef unsigned int sigset_t;	
typedef void (*__sighandler_t)(int);

struct sigaction {
	__sighandler_t sa_handler;
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restorer)(void);
};

struct wait_queue {
	struct task_struct * task;
	struct wait_queue * next;
};

struct semaphore {
	int count;
	struct wait_queue * wait;
};

#define MUTEX ((struct semaphore) { 1, NULL })

struct select_table_entry {
	struct wait_queue wait;
	struct wait_queue ** wait_address;
};

typedef struct select_table_struct {
	int nr;
	struct select_table_entry * entry;
} select_table;

struct rlimit {
	int	rlim_cur;
	int	rlim_max;
};

struct vm86_regs {
/*
 * normal regs, with special meaning for the segment descriptors..
 */
	long ebx;
	long ecx;
	long edx;
	long esi;
	long edi;
	long ebp;
	long eax;
	long __null_ds;
	long __null_es;
	long __null_fs;
	long __null_gs;
	long orig_eax;
	long eip;
	long cs;
	long eflags;
	long esp;
	long ss;
/*
 * these are specific to v86 mode:
 */
	long es;
	long ds;
	long fs;
	long gs;
};

struct vm86_struct {
	struct vm86_regs regs;
	unsigned long flags;
	unsigned long screen_bitmap;
};

struct buffer_head {
	char * b_data;			/* pointer to data block (1024 bytes) */
	unsigned long b_size;		/* block size */
	unsigned long b_blocknr;	/* block number */
	dev_t b_dev;			/* device (0 = free) */
	unsigned short b_count;		/* users using this block */
	unsigned char b_uptodate;
	unsigned char b_dirt;		/* 0-clean,1-dirty */
	unsigned char b_lock;		/* 0 - ok, 1 -locked */
	unsigned char b_req;		/* 0 if the buffer has been invalidated */
	struct wait_queue * b_wait;
	struct buffer_head * b_prev;		/* doubly linked list of hash-queue */
	struct buffer_head * b_next;
	struct buffer_head * b_prev_free;	/* doubly linked list of buffers */
	struct buffer_head * b_next_free;
	struct buffer_head * b_this_page;	/* circular list of buffers in one page */
	struct buffer_head * b_reqnext;		/* request queue */
};


struct inode_operations {
	struct file_operations * default_file_ops;
	int (*create) (struct inode *,const char *,int,int,struct inode **);
	int (*lookup) (struct inode *,const char *,int,struct inode **);
	int (*link) (struct inode *,struct inode *,const char *,int);
	int (*unlink) (struct inode *,const char *,int);
	int (*symlink) (struct inode *,const char *,int,const char *);
	int (*mkdir) (struct inode *,const char *,int,int);
	int (*rmdir) (struct inode *,const char *,int);
	int (*mknod) (struct inode *,const char *,int,int,int);
	int (*rename) (struct inode *,const char *,int,struct inode *,const char *,int);
	int (*readlink) (struct inode *,char *,int);
	int (*follow_link) (struct inode *,struct inode *,int,int,struct inode **);
	int (*bmap) (struct inode *,int);
	void (*truncate) (struct inode *);
	int (*permission) (struct inode *, int);
};

struct super_operations {
	void (*read_inode) (struct inode *);
	int (*notify_change) (int flags, struct inode *);
	void (*write_inode) (struct inode *);
	void (*put_inode) (struct inode *);
	void (*put_super) (struct super_block *);
	void (*write_super) (struct super_block *);
	void (*statfs) (struct super_block *, struct statfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
};

struct file_lock {
	struct file_lock *fl_next;	/* singly linked list */
	struct task_struct *fl_owner;	/* NULL if on free list, for sanity checks */
        unsigned int fl_fd;             /* File descriptor for this lock */
	struct wait_queue *fl_wait;
	char fl_type;
	char fl_whence;
	off_t fl_start;
	off_t fl_end;
};


typedef enum {
  SS_FREE = 0,				/* not allocated		*/
  SS_UNCONNECTED,			/* unconnected to any socket	*/
  SS_CONNECTING,			/* in process of connecting	*/
  SS_CONNECTED,				/* connected to socket		*/
  SS_DISCONNECTING			/* in process of disconnecting	*/
} socket_state;

struct socket {
  short			type;		/* SOCK_STREAM, ...		*/
  socket_state		state;
  long			flags;
  struct proto_ops	*ops;		/* protocols do most everything	*/
  void			*data;		/* protocol data		*/
  struct socket		*conn;		/* server socket connected to	*/
  struct socket		*iconn;		/* incomplete client conn.s	*/
  struct socket		*next;
  struct wait_queue	**wait;		/* ptr to place to wait on	*/
  struct inode		*inode;
};

struct timestamp {
  unsigned char	len;
  unsigned char ptr;
  union {
	unsigned char	flags:4,
			overflow:4;
	unsigned char	full_char;
  } x;
  unsigned long	data[9];
};


#define MAX_ROUTE	16

struct route {
  char		route_size;
  char		pointer;
  unsigned long route[MAX_ROUTE];
};


struct options {
  struct route		record_route;
  struct route		loose_route;
  struct route		strict_route;
  struct timestamp	tstamp;
  unsigned short	security;
  unsigned short	compartment;
  unsigned short	handling;
  unsigned short	stream;
  unsigned		tcc;
};

struct vm_area_struct {
	struct task_struct * vm_task;		/* VM area parameters */
	unsigned long vm_start;
	unsigned long vm_end;
	unsigned short vm_page_prot;
	struct vm_area_struct * vm_next;	/* linked list */
	struct vm_area_struct * vm_share;	/* linked list */
	struct inode * vm_inode;
	unsigned long vm_offset;
	struct vm_operations_struct * vm_ops;
};


typedef struct desc_struct {
	unsigned long a,b;
} desc_table[256];


/*
 * These are the virtual MM functions - opening of an area, closing it (needed to
 * keep files on disk up-to-date etc), pointer to the functions called when a
 * no-page or a wp-page exception occurs, and the function which decides on sharing
 * of pages between different processes.
 */
struct vm_operations_struct {
	void (*open)(struct vm_area_struct * area);
	void (*close)(struct vm_area_struct * area);
	void (*nopage)(int error_code,
		       struct vm_area_struct * area, unsigned long address);
	void (*wppage)(struct vm_area_struct * area, unsigned long address);
	int (*share)(struct vm_area_struct * from, struct vm_area_struct * to, unsigned long address);
	int (*unmap)(struct vm_area_struct *area, unsigned long, size_t);
};

struct sockaddr {
  unsigned short	sa_family;	/* address family, AF_xxx	*/
  char			sa_data[14];	/* 14 bytes of protocol address	*/
};

struct inode {
	dev_t		i_dev;
	unsigned long	i_ino;
	umode_t		i_mode;
	nlink_t		i_nlink;
	uid_t		i_uid;
	gid_t		i_gid;
	dev_t		i_rdev;
	off_t		i_size;
	time_t		i_atime;
	time_t		i_mtime;
	time_t		i_ctime;
	unsigned long	i_blksize;
	unsigned long	i_blocks;
	struct semaphore i_sem;
	struct inode_operations * i_op;
  struct vm_object_t    vnode;
	struct super_block * i_sb;
	struct wait_queue * i_wait;
	struct file_lock * i_flock;
	struct vm_area_struct * i_mmap;
	struct inode * i_next, * i_prev;
	struct inode * i_hash_next, * i_hash_prev;
	struct inode * i_bound_to, * i_bound_by;
	struct inode * i_mount;
	struct socket * i_socket;
	unsigned short i_count;
	unsigned short i_flags;
	unsigned char i_lock;
	unsigned char i_dirt;
	unsigned char i_pipe;
	unsigned char i_seek;
	unsigned char i_update;
};

struct shm_info {
	int   used_ids;
	ulong shm_tot; /* total allocated shm */
	ulong shm_rss; /* total resident shm */
	ulong shm_swp; /* total swapped shm */
	ulong swap_attempts;
	ulong swap_successes;
};


/*
 * Per process internal structure for managing segments.
 * A shmat will add to and shmdt will remove from the list.
 */
struct	shm_desc {
	struct task_struct *task;     /* attacher */
	unsigned long shm_sgn;        /* signature for this attach */
	unsigned long start;   /* virt addr of attach, multiple of SHMLBA */
	unsigned long end;            /* multiple of SHMLBA */
	struct shm_desc *task_next;   /* next attach for task */
	struct shm_desc *seg_next;    /* next attach for segment */
};

struct i387_hard_struct {
	long	cwd;
	long	swd;
	long	twd;
	long	fip;
	long	fcs;
	long	foo;
	long	fos;
	long	st_space[20];	/* 8*10 bytes for each FP-reg = 80 bytes */
};

struct i387_soft_struct {
	long	cwd;
	long	swd;
	long	twd;
	long	fip;
	long	fcs;
	long	foo;
	long	fos;
	long    top;
	struct fpu_reg	regs[8];	/* 8*16 bytes for each FP-reg = 128 bytes */
	unsigned char	lookahead;
	struct info	*info;
	unsigned long	entry_eip;
};

union i387_union {
	struct i387_hard_struct hard;
	struct i387_soft_struct soft;
};

struct tss_struct {
	unsigned short	back_link,__blh;
	unsigned long	esp0;
	unsigned short	ss0,__ss0h;
	unsigned long	esp1;
	unsigned short	ss1,__ss1h;
	unsigned long	esp2;
	unsigned short	ss2,__ss2h;
	unsigned long	cr3;
	unsigned long	eip;
	unsigned long	eflags;
	unsigned long	eax,ecx,edx,ebx;
	unsigned long	esp;
	unsigned long	ebp;
	unsigned long	esi;
	unsigned long	edi;
	unsigned short	es, __esh;
	unsigned short	cs, __csh;
	unsigned short	ss, __ssh;
	unsigned short	ds, __dsh;
	unsigned short	fs, __fsh;
	unsigned short	gs, __gsh;
	unsigned short	ldt, __ldth;
	unsigned short	trace, bitmap;
	unsigned long	io_bitmap[IO_BITMAP_SIZE+1];
	unsigned long	tr;
	unsigned long	cr2, trap_no, error_code;
	union i387_union i387;
};

struct task_struct {
/* these are hardcoded - don't touch */
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	long counter;
	long priority;
	unsigned long signal;
	unsigned long blocked;	/* bitmap of masked signals */
	unsigned long flags;	/* per process flags, defined below */
	int errno;
	int debugreg[8];  /* Hardware debugging registers */
/* various fields */
	struct task_struct *next_task, *prev_task;
	struct sigaction sigaction[32];
	unsigned long saved_kernel_stack;
	unsigned long kernel_stack_page;
	int exit_code, exit_signal;
	int elf_executable:1;
	int dumpable:1;
	int swappable:1;
	unsigned long start_code,end_code,end_data,start_brk,brk,start_stack,start_mmap;
	unsigned long arg_start, arg_end, env_start, env_end;
	int pid,pgrp,session,leader;
	int	groups[NGROUPS];
	/* 
	 * pointers to (original) parent process, youngest child, younger sibling,
	 * older sibling, respectively.  (p->father can be replaced with 
	 * p->p_pptr->pid)
	 */
	struct task_struct *p_opptr,*p_pptr, *p_cptr, *p_ysptr, *p_osptr;
	struct wait_queue *wait_chldexit;	/* for wait4() */
	/*
	 * For ease of programming... Normal sleeps don't need to
	 * keep track of a wait-queue: every task has an entry of its own
	 */
	unsigned short uid,euid,suid;
	unsigned short gid,egid,sgid;
	unsigned long timeout;
	unsigned long it_real_value, it_prof_value, it_virt_value;
	unsigned long it_real_incr, it_prof_incr, it_virt_incr;
	long utime,stime,cutime,cstime,start_time;
	unsigned long min_flt, maj_flt;
	unsigned long cmin_flt, cmaj_flt;
	struct rlimit rlim[RLIM_NLIMITS]; 
	unsigned short used_math;
	unsigned short rss;	/* number of resident pages */
	char comm[16];
	struct vm86_struct * vm86_info;
	unsigned long screen_bitmap;
/* file system info */
	int link_count;
	int tty;		/* -1 if no tty, so it must be signed */
	unsigned short umask;
	struct inode * pwd;
	struct inode * root;
	struct inode * executable;
	struct vm_area_struct * mmap;
	struct shm_desc *shm;
	struct sem_undo *semun;
fd_set close_on_exec;
/* ldt for this task - used by Wine.  If NULL, default_ldt is used */
	struct desc_struct *ldt;
/* tss for this task */
	struct tss_struct tss;
#ifdef NEW_SWAP
	unsigned long old_maj_flt;	/* old value of maj_flt */
	unsigned long dec_flt;		/* page fault count of the last time */
	unsigned long swap_cnt;		/* number of pages to swap on next pass */
	short swap_table;		/* current page table */
	short swap_page;		/* current page */
#endif NEW_SWAP
	struct vm_area_struct *stk_vma;
    rb_node_t	rb_node;
    rb_node_linked		rnl;
    struct rb_root 		*rb_root;
};

#endif
