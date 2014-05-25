#include "controller_global.hpp"
#include "util.hpp"

#include <ctime>
#include <semaphore.h>

long unsigned int global_controller_period;
sem_t global_controller_period_mutex;

inline void controller_global_work()
{
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
