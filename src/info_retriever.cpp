#include <iostream>

#include "info_retriever.hpp"

#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*********************************************************
 * Sched deadline bandwidth is initialized with the same *
 * values of sched_rt ones and those can be found in:    *
 *   /proc/sys/kernel/sched_rt_period_us                 *
 *   /proc/sys/kernel/sched_rt_runtime_us                *
 * The bandwidth has to be multiplied with the total     *
 * number of processors                                  *
 *********************************************************/

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
    std::ifstream ifs;

    ifs.open("/proc/sys/kernel/sched_rt_period_us", std::ifstream::in);
    ifs >> ret;
    ifs.close();
  }

  return ret;
}

double sched_deadline_bandwidth()
{
  return (double) cpu_num() *
      sched_deadline_runtime_us() /
      sched_deadline_period_us();
}

bool task_exists(pid_t pid)
{
  // Check for external processes to be
  // dead.
  //
  // Warning: this just checks if a process
  // with a certain PID exists. It may happen
  // that another process takes the same PID of
  // a dead one.
  // (not so safe operation)

  std::string folder;

  folder = "/proc/";
  folder = folder + std::to_string((int)pid);

  struct stat st;
  if(stat(folder.c_str(), &st) == 0)
    return true;
  return false;
}

int cpu_num()
{
  static int ret = -1;

  if (ret == -1) {
    std::string s;
    std::ifstream ifs;

    ifs.open("/proc/cpuinfo", std::ifstream::in);

    ret = 0;
    while (ifs.good()) {
      ifs >> s;
      if (s == "processor")
        ++ret;
    }

    ifs.close();
  }

  return ret;
}

void update_task_statistics(pid_t pid,
                            std::vector<long unsigned int> &statistics,
                            unsigned int howMuch)
{
  std::string path;
  std::string s;
  std::ifstream ifs;

  path = "/proc/sched_deadline/";
  path = path + std::to_string((int)pid);

  ifs.open(path, std::ifstream::in);

  statistics.clear();
  for (unsigned int i=0; i<howMuch && ifs.good(); ++i) {
    ifs >> s;
    ifs >> s;
    ifs >> s;
    statistics.push_back(atoi(s.c_str()));
    ifs >> s;
  }

  ifs.close();
}
