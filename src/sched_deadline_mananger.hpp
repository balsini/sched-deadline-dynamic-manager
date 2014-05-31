#ifndef SCHED_DEADLINE_MANAGER_HPP
#define SCHED_DEADLINE_MANAGER_HPP

#include "dl_syscalls.h"

#include <map>

extern std::map<pid_t, sched_attr_t> taskMap;

/*
 * Updates SCHED_DEADLINE patameters of a certain
 * process with PID pid
 */
int update_dl_parameters(pid_t pid,
                         unsigned long int period,
                         unsigned long int deadline,
                         unsigned long int runtime);

#endif
