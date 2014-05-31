/*
 * Libdl
 *  (C) Dario Faggioli <raistlin@linux.it>, 2009, 2010
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License (COPYING file) for more details.
 *
 */

#ifndef __DL_SYSCALLS__
#define __DL_SYSCALLS__

#include <time.h>
#include <linux/types.h>

#define SCHED_DEADLINE	6

/* XXX use the proper syscall numbers */
#ifdef __x86_64__
#define __NR_sched_setattr		314
#define __NR_sched_getattr		315
#endif

#ifdef __i386__
#define __NR_sched_setattr		351
#define __NR_sched_getattr		352
#endif

#ifdef __arm__
#define __NR_sched_setattr		380
#define __NR_sched_getattr		381
#endif

#define SF_SIG_RORUN		2
#define SF_SIG_DMISS		4
#define SF_BWRECL_DL		8
#define SF_BWRECL_RT		16
#define SF_BWRECL_OTH		32

#define RLIMIT_DLDLINE		16
#define RLIMIT_DLRTIME		17

struct sched_attr_t {
  __u32 size;

  __u32 sched_policy;
  __u64 sched_flags;

  /* SCHED_NORMAL, SCHED_BATCH */
  __s32 sched_nice;

  /* SCHED_FIFO, SCHED_RR */
  __u32 sched_priority;

  /* SCHED_DEADLINE */
  __u64 sched_runtime;
  __u64 sched_deadline;
  __u64 sched_period;
};

/*
 * Updates the SCHED_DEADLINE scheduling parameters
 * to the task PID, by launching the proper syscall
 */
int sched_setattr(pid_t pid,
          const sched_attr_t * attr,
          unsigned int flags);

/*
 * Gathers the SCHED_DEADLINE scheduling parameters of the
 * task PID, by launching the proper syscall
 */
int sched_getattr(pid_t pid,
          sched_attr_t * attr,
          unsigned int size,
          unsigned int flags);

#endif /* __DL_SYSCALLS__ */
