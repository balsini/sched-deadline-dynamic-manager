#include "info_retriever.hpp"

#include <fstream>

/*
 * Sched deadline bandwidth is initialized with the same
 * values of sched_rt ones and those can be found in:
 *   /proc/sys/kernel/sched_rt_period_us
 *   /proc/sys/kernel/sched_rt_runtime_us
 */

long int sched_deadline_runtime_us()
{
  static long int ret = -1;

  if (ret == -1) {
    std::ifstream ifs;

    ifs.open("/proc/sys/kernel/sched_rt_runtime_us", std::ifstream::in);
    ifs >> ret;
    ifs.close();
  }

  return ret;
}

long int sched_deadline_period_us()
{
  static long int ret = -1;

  if (ret == -1) {
    if (ret == -1) {
      std::ifstream ifs;

      ifs.open("/proc/sys/kernel/sched_rt_period_us", std::ifstream::in);
      ifs >> ret;
      ifs.close();
    }
  }

  return ret;
}

double sched_deadline_bandwidth()
{
  return (double) sched_deadline_runtime_us() / (double) sched_deadline_period_us();
}
