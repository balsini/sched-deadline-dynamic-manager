#include "controller_local.hpp"
#include "util.hpp"
#include "info_retriever.hpp"
#include "communication_port.hpp"

#include <thread>

#include <map>
#include <vector>

#include <ctime>

#include <unistd.h>
#include <string.h>
#include <errno.h>

#define STATISTICS_WINDOW_SIZE 50

std::vector<std::thread> thread_vector;
std::map<pid_t, std::vector<long unsigned int>> statistics;

inline void controller_local_worker(pid_t pid, long unsigned int response_time)
{
  double utilization = 1;
  double max;

  //log("controller_local_thread", "controlling pid", pid);
  //log("controller_local_thread", "controlling RT", response_time);

  update_task_statistics(pid, statistics[pid], STATISTICS_WINDOW_SIZE);

  if (statistics[pid].size() < STATISTICS_WINDOW_SIZE * 3 / 4) {
    // it's a new task
    // let's give it a low value at the beginning
    utilization = 0.1;
    taskInfoDynamic[pid].period(response_time);
  } else {
    // use the maximum in the window
    max = 0.0;
    for (auto o : statistics[pid]) {
      if (o > max)
        max = o;
    }
    utilization = max / (double)response_time;
    if (utilization >= 0.95)
      utilization = 0.95;
  }

  //log("controller_local_worker", "utilization chosen", utilization);
  taskInfoDynamic[pid].utilization(utilization);
}

void * controller_local_thread(pid_t pid, long long unsigned int response_time)
{
  long long unsigned int period = ((double) response_time / 1000.0 * 10); // Every 10 task periods
  struct timespec next;

  clock_gettime(CLOCK_REALTIME, &next);
  for (;;) {
    timespec_add_us(&next, period);

    controller_local_worker(pid, response_time);

    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
  }
  return nullptr;
}

void controller_local_control(pid_t pid, long unsigned int response_time)
{
  thread_vector.push_back(std::thread(controller_local_thread, pid, response_time));
}

void controller_local_launch_and_control(const std::string &path,
                                         const std::string &arguments,
                                         long unsigned int response_time)
{
  pid_t pid;

  pid = fork(); // create child process
  switch (pid) {
    case -1: // error
      log("fork", "ERROR!");
      exit(1);
      break;
    case 0: // child process, try to swap context
      execv(path.c_str(), parse_arguments(path + " " + arguments));

      log("execv", "ERROR!", strerror(errno)); // execl doesn't return unless there is a problem
      exit(1);
      break;
    default: // parent process, pid now contains the child pid
      controller_local_control(pid, response_time);
      break;
  }
}
