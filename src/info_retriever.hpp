#ifndef INFO_RETRIEVER_HPP
#define INFO_RETRIEVER_HPP

#include <vector>

#include <sched.h>

long int sched_deadline_runtime_us();
long int sched_deadline_period_us();
double sched_deadline_bandwidth();
bool task_exists(pid_t pid);
int cpu_num();
void update_task_statistics(pid_t pid, std::vector<long unsigned int> &statistics,
                            unsigned int howMuch);

#endif
