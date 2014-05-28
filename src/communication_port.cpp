#include "communication_port.hpp"

std::map<pid_t, ControllerGlobalPort> taskInfoDynamic;

ControllerGlobalPort::ControllerGlobalPort()
{
  sem_init(&mutex, 0, 1);
}

ControllerGlobalPort::~ControllerGlobalPort()
{
  sem_destroy(&mutex);
}

double ControllerGlobalPort::period()
{
  sem_wait(&mutex);

  double ret = period_;

  sem_post(&mutex);

  return ret;
}

void ControllerGlobalPort::period(double newPeriod)
{
  sem_wait(&mutex);

  period_ = newPeriod;

  sem_post(&mutex);
}

void ControllerGlobalPort::utilization(double newUtilization)
{
  sem_wait(&mutex);

  utilization_ = newUtilization;
  runtime_ = newUtilization * period_;

  sem_post(&mutex);
}

double ControllerGlobalPort::utilization()
{
  sem_wait(&mutex);

  double newUtilization = utilization_;

  sem_post(&mutex);

  return newUtilization;
}

void ControllerGlobalPort::runtime(double newRuntime)
{
  sem_wait(&mutex);

  runtime_ = newRuntime;
  utilization_ = newRuntime / period_;

  sem_post(&mutex);
}

double ControllerGlobalPort::runtime()
{
  sem_wait(&mutex);

  double newRuntime = runtime_;

  sem_post(&mutex);

  return newRuntime;
}
