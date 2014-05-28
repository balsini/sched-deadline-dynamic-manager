#ifndef COMMUNICATION_PORT_HPP
#define COMMUNICATION_PORT_HPP

#include <map>
#include <semaphore.h>

class ControllerGlobalPort
{
  double utilization_;
  double period_;
  double runtime_;
  sem_t mutex;

public:
  ControllerGlobalPort();
  ~ControllerGlobalPort();

  double period();
  void period(double newPeriod);
  double utilization();
  void utilization(double newUtilization);
  double runtime();
  void runtime(double newRuntime);
};

extern std::map<pid_t, ControllerGlobalPort> taskInfoDynamic;

#endif
