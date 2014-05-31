#ifndef INFO_RETRIEVER_HPP
#define INFO_RETRIEVER_HPP

#include <vector>

#include <sched.h>

/*
 * Returns the SCHED_DEADLINE runtime for each
 * CPU
 */
long int sched_deadline_runtime_us();

/*
 * Returns the SCHED_DEADLINE period for each
 * CPU
 */
long int sched_deadline_period_us();

/*
 * Returns the total SCHED_DEADLINE bandwidth
 */
double sched_deadline_bandwidth();

/*
 * Checks if a PID has associated a still alive
 * task
 */
bool task_exists(pid_t pid);

/*
 * Returns the number of CPUs of the system
 */
int cpu_num();

/*
 * Gets the statistics of a certain task and inserts them in
 * a vector
 */
void update_task_statistics(pid_t pid,
                            std::vector<long unsigned int> &statistics,
                            unsigned int howMuch);

#endif
