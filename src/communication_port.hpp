#ifndef COMMUNICATION_PORT_HPP
#define COMMUNICATION_PORT_HPP

#include <map>
#include <semaphore.h>

/*
 * Shared class containing dynamic task information.
 * Each function is executed in mutual exclusion, allowing
 * different threads to access it.
 */
class ControllerGlobalPort
{
  double utilization_;
  double period_;
  double runtime_;
  sem_t mutex;

public:
  ControllerGlobalPort();
  ~ControllerGlobalPort();

  /*
   * gets the period
   */
  double period();
  /*
   * sets the period
   */
  void period(double newPeriod);
  /*
   * gets the utilization factor
   */
  double utilization();
  /*
   * sets the utilization factor
   */
  void utilization(double newUtilization);
  /*
   * gets the runtime
   */
  double runtime();
  /*
   * sets the runtime
   */
  void runtime(double newRuntime);
};

/*
 * The map of the global controller ports.
 * Each dynamic task has a port associated.
 */
extern std::map<pid_t, ControllerGlobalPort> taskInfoDynamic;

#endif
