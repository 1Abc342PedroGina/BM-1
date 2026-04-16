/* SPDX License Indentifier: GPL-2.0-only WITH Mach-Syscalls-Note */

#ifndef _MACH_MACH_TYPES_H_
#define _MACH_MACH_TYPES_H_

#include <stdint.h>

#include <sys/cdefs.h>
#include <sys/proc.h>
#ifdef __cplusplus
#include <c++/Kern.h>
#else
#include <mach/asm.h>
#endif
#include <linux/rbtree.h>
#include <mach/host_info.h>
#include <mach/host_notify.h>
#include <mach/host_special_ports.h>
#include <mach/machine.h>
#include <mach/vm_types.h>
#include <mach/memory_object_types.h>
#include <mach/message.h>
#include <mach/queue.h>
#include <mach/thread.h>
#include <mach/ast.h>
#include <mach/cpu_number.h>
#include <mach/queue.h>
#include <mach/pc_sample.h>
#include <mach/processor.h>
#include <mach/sched_prim.h>	
#include <mach/time_out.h>
#include <mach/timer.h>
#include <mach/lock.h>
#include <mach/sched_cfs.h>
#include <mach/task.h>		
#include <mach/kern_types.h>
#include <mach/exception_types.h>
#include <mach/port.h>
#include <mach/mach_voucher_types.h>
#include <mach/processor_info.h>
#include <mach/task_info.h>
#include <mach/task_inspect.h>
#include <mach/task_policy.h>
#include <mach/task_special_ports.h>
#include <mach/thread_info.h>
#include <mach/thread_policy.h>
#include <mach/thread_special_ports.h>
#include <linux/syscall.h>
#include <mach/thread_status.h>
#include <mach/time_value.h>
#include <mach/clock_types.h>
#include <mach/vm_attributes.h>
#include <mach/vm_inherit.h>
#include <mach/vm_purgable.h>
#include <mach/vm_behavior.h>
#include <mach/vm_prot.h>
#include <mach/vm_statistics.h>
#include <mach/vm_sync.h>
#include <mach/vm_types.h>
#include <mach/vm_region.h>
#include <mach/kmodule.h>
#include <mach/cpu.h>
#ifdef __x86_64__
#include <mach/i386/apple.h>
#elif __aarch64__
#include <mach/arm64/apple.h>
#else
#error
#endif
#include <mach/macro_help.h>
#include <mach-o/object.h>
#include <mach-o/executable.h>
#include <mach/ipc_host.h>
#include <mach/ipc_port.h>
#include <mach/kalloc.h>
#include <mach/zalloc.h>
#include <mach/kport.h>
#include <mach/ipc_space.h>
#include <mach/ipc_entry.h>
#include <mach/ipc_port.h>
#include <mach/ipc_right.h>
#include <mach/ipc_marequest.h>
#include <mach/ipc_notify.h>

#endif
