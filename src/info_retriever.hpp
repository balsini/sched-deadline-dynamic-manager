#ifndef INFO_RETRIEVER_HPP
#define INFO_RETRIEVER_HPP

#include <sched.h>

long int sched_deadline_runtime_us();
long int sched_deadline_period_us();
double sched_deadline_bandwidth();
bool task_exists(pid_t pid);

#endif
