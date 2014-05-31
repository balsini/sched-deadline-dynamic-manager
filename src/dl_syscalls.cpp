#include "dl_syscalls.h"

#include <linux/kernel.h>
#include <linux/unistd.h>
#include <unistd.h>

int sched_setattr(pid_t pid,
          const sched_attr_t * attr,
          unsigned int flags)
{
  return syscall(__NR_sched_setattr, pid, attr, flags);
}

int sched_getattr(pid_t pid,
          struct sched_attr_t *attr,
          unsigned int size,
          unsigned int flags)
{
  return syscall(__NR_sched_getattr, pid, attr, size, flags);
}
