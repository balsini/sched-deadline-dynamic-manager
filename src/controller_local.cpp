#include "controller_local.hpp"
#include "util.hpp"

#include <thread>
#include <vector>
#include <ctime>

#include <unistd.h>
#include <string.h>
#include <errno.h>

std::vector<std::thread> thread_vector;

inline void controller_local_worker(pid_t pid, long unsigned int response_time)
{
  log("controller_local_thread", "controlling pid", pid);
  log("controller_local_thread", "controlling RT", response_time);
}

void * controller_local_thread(pid_t pid, long unsigned int response_time)
{
  long unsigned int period = 1000000; // first execution starts without delay
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

void controller_local_launch_and_control(const std::string &path, const std::string &arguments, long unsigned int response_time)
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
