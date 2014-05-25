#include "controller_local.hpp"
#include "util.hpp"

#include <thread>
#include <vector>
#include <ctime>

#include <unistd.h>
#include <string.h>
#include <errno.h>

std::vector<std::thread> thread_vector;

inline void controller_local_worker(pid_t pid)
{
  log("controller_local_thread", "controlling", pid);
}

void * controller_local_thread(pid_t pid)
{
  long unsigned int period = 1000000; // first execution starts without delay
  struct timespec next;

  clock_gettime(CLOCK_REALTIME, &next);
  for (;;) {
    timespec_add_us(&next, period);

    controller_local_worker(pid);

    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
  }
  return nullptr;
}

void controller_local_control(pid_t pid)
{
  thread_vector.push_back(std::thread(controller_local_thread, pid));
}

void controller_local_launch_and_control(const std::string &s)
{
  pid_t pid = fork(); // create child process
  switch (pid) {
    case -1: // error
      log("fork", "ERROR!");
      exit(1);
      break;
    case 0: // child process
      execl(s.c_str(), "", NULL); // run the command
      log("execl", "ERROR!", strerror(errno)); // execl doesn't return unless there is a problem
      exit(1);
    default: // parent process, pid now contains the child pid
      controller_local_control(pid);
      break;
  }
}
