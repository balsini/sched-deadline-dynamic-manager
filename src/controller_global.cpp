#include "controller_global.hpp"
#include "util.hpp"
#include "sched_deadline_mananger.hpp"
#include "info_retriever.hpp"
#include "communication_port.hpp"

#include <vector>

#include <ctime>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

long unsigned int global_controller_period;
sem_t global_controller_period_mutex;

std::vector<pid_t> tasksToRemove;
std::map<pid_t, double> taskUtilizationFixed;

void clean_task_lists()
{
  pid_t pid;

  tasksToRemove.clear();

  // Clear forked processes
  // Withouth these steps, processes
  // remain zombies
  pid = waitpid(-1, NULL, WNOHANG);
  while (pid != 0 && pid != -1) {
    taskUtilizationFixed.erase(pid);
    taskInfoDynamic.erase(pid);
    pid = waitpid(-1, NULL, WNOHANG);
  }

  for (auto o : taskUtilizationFixed) {
    if (!task_exists(o.first))
        tasksToRemove.push_back(o.first);
  }

  for (auto o : taskInfoDynamic) {
    if (!task_exists(o.first))
        tasksToRemove.push_back(o.first);
  }

  for (auto o : tasksToRemove) {
    taskUtilizationFixed.erase(o);
    taskInfoDynamic.erase(o);
  }
}

inline void controller_global_work()
{
  clean_task_lists();

  // Sched deadline bandwidth
  double B_SD = sched_deadline_bandwidth();
  // Utilization factor of fixed tasks
  double U_F = 0.0;
  // Utilization factor of dynamic tasks
  double U_D = 0.0;
  double B_residual; // B_SD - U_F

  log("controller_global_thread", "Fixed");
  for (auto o : taskUtilizationFixed) {
    log("controller_global_thread", o.first, o.second);
    U_F += o.second;
  }

  log("controller_global_thread", "Dynamic");
  for (auto o : taskInfoDynamic) {
    log("controller_global_thread", o.first, o.second.utilization());
    U_D += o.second.utilization();
  }

  B_residual = B_SD - U_F;
  log("controller_global_thread", "B_SD", B_SD);
  log("controller_global_thread", "B_residual", B_residual);
  log("controller_global_thread", "U_F", U_F);
  log("controller_global_thread", "U_D", U_D);

  if (B_residual < U_D) {
    // Compress the springs!
    double T_D_sum = 0.0;

    for (auto o : taskInfoDynamic) {
      T_D_sum += o.second.period();
    }

    for (auto o : taskInfoDynamic) {

      double U_D_i_new = o.second.utilization() - (U_D - B_residual) * (o.second.period() / T_D_sum);
      double Runtime_D_i_new = U_D_i_new * o.second.period();
      update_dl_parameters(o.first, o.second.period(), o.second.period(), (unsigned long)Runtime_D_i_new);
    }
  } else {
    for (auto o : taskInfoDynamic) {
      update_dl_parameters(o.first, o.second.period(), o.second.period(), o.second.runtime());
    }
  }
}

void controller_global_init()
{
  // Mutual exclusion initialization
  sem_init(&global_controller_period_mutex, 0, 1);
  global_controller_period = 500000; // 1 sec
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
                                 long unsigned int runtime,
                                 long unsigned int deadline,
                                 long unsigned int period)
{
  // Forces the new parameters (why not?)
  update_dl_parameters(pid, period, deadline, runtime);
  taskUtilizationFixed[pid] = (double)runtime / (double)period;
}

void controller_global_add_dynamic(pid_t pid,
                                   long unsigned int runtime,
                                   long unsigned int deadline,
                                   long unsigned int period)
{
  taskInfoDynamic[pid].utilization((double)runtime / (double)period);
  taskInfoDynamic[pid].period((double)period);
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
      controller_global_add_fixed(pid, runtime, deadline, period);
      break;
  }
}
