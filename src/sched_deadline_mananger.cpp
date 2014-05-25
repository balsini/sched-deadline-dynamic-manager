#include "sched_deadline_mananger.hpp"

#include <cstring>

std::map<pid_t, sched_attr_t> taskMap;

int update_dl_parameters(pid_t pid,
                         unsigned long int period,
                         unsigned long int deadline,
                         unsigned long int runtime)
{
  sched_attr_t attr;

  memset(&attr, 0, sizeof(attr));

  attr.sched_policy = SCHED_DEADLINE;

  attr.sched_runtime = runtime;
  attr.sched_deadline = deadline;
  attr.sched_period = period;

  attr.size = sizeof(attr);

  taskMap[pid] = attr;

  return sched_setattr(pid, &attr, 0);
}
