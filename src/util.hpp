#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <mutex>
#include <string>

/*
 * This data struct is used to provide a flexible
 * logging method.
 * By changing this data structure is possible to define
 * how to manage the logging
 */
extern MyLogger log;

/*
 * Transforms an argument std::string to the common format:
 * char * argv[]
 */
char ** parse_arguments(const std::string &argsString);

/*
 * Adds microseconds to a timespec
 */
void timespec_add_us(struct timespec * t, long us);

class MyLogger
{
  std::mutex log_mutex;
public:
  MyLogger() {}

  void operator()(std::string s)
  {
    log_mutex.lock();
    std::cout << "LOG : " << s << std::endl;
    log_mutex.unlock();
  }

  template<class T>
  void operator()(T subject)
  {
    log_mutex.lock();
    std::cout << "LOG : " << subject << std::endl;
    log_mutex.unlock();
  }

  template<class T, class U>
  void operator()(T subject, U v)
  {
    log_mutex.lock();
    std::cout << "LOG : " << subject << " : " << v << std::endl;
    log_mutex.unlock();
  }

  template<class T, class U, class V>
  void operator()(T subject, U v, V w)
  {
    log_mutex.lock();
    std::cout << "LOG : " << subject << " : " << v << " : " << w << std::endl;
    log_mutex.unlock();
  }
};

#endif
