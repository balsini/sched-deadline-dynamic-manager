#ifndef CONTROLLER_LOCAL_HPP
#define CONTROLLER_LOCAL_HPP

#include <string>

void controller_local_launch_and_control(const std::string &path, const std::string &arguments, unsigned long response_time);
void controller_local_control(pid_t pid, unsigned long response_time);

#endif
