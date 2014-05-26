#include "controller_global.hpp"
#include "util.hpp"
#include "sched_deadline_mananger.hpp"

#include <map>

#include <ctime>
#include <semaphore.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>

long unsigned int global_controller_period;
sem_t global_controller_period_mutex;

std::map<pid_t, double> taskUtilizationDynamic;
std::map<pid_t, double> taskUtilizationFixed;

inline void controller_global_work()
{
  log("controller_global_thread", "Dynamic");
  for (auto o : taskUtilizationDynamic)
    log("controller_global_thread", o.first, o.second);

  log("controller_global_thread", "Fixed");
  for (auto o : taskUtilizationFixed)
    log("controller_global_thread", o.first, o.second);

  //log("controller_global_thread", "doing stuff");
}

void controller_global_init()
{
  // Mutual exclusion initialization
  sem_init(&global_controller_period_mutex, 0, 1);
  global_controller_period = 1000000; // 1 sec
}

void * controller_global_thread()
{
  struct timespec next;

  controller_global_init();

  log("controller_global_thread", "Initialized and running loop");

  clock_gettime(CLOCK_REALTIME, &next);
  for (;;) {
    sem_wait(&global_controller_period_mutex);
    timespec_add_us(&next, global_controller_period);
    sem_post(&global_controller_period_mutex);

    controller_global_work();

    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
  }
  return nullptr;
}

void controller_global_add_fixed(pid_t pid,
                           double U)
{
  taskUtilizationFixed[pid] = U;
}

void controller_global_add_dynamic(pid_t pid,
                           double U)
{
  taskUtilizationDynamic[pid] = U;
}

void controller_global_launch(const std::string &path,
                              const std::string &arguments,
                              long unsigned int runtime,
                              long unsigned int deadline,
                              long unsigned int period)
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
      update_dl_parameters(pid, period, deadline, runtime);
      controller_global_add_fixed(pid, (double)runtime/(double)period);
      break;
  }
}
