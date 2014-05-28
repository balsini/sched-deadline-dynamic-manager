#ifndef CONTROLLER_GLOBAL_HPP
#define CONTROLLER_GLOBAL_HPP

#include <string>

#include <sched.h>

void controller_global_init();
void * controller_global_thread();
void controller_global_add_fixed(pid_t pid,
                                 unsigned long runtime,
                                 unsigned long deadline,
                                 unsigned long period);
void controller_global_add_dynamic(pid_t pid,
                                   long unsigned int runtime,
                                   long unsigned int deadline,
                                   long unsigned int period);
void controller_global_launch(const std::string &path,
                              const std::string &arguments,
                              long unsigned int runtime,
                              long unsigned int deadline,
                              long unsigned int period);

#endif
