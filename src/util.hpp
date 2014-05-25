#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <mutex>
#include <string>

void timespec_add_us(struct timespec * t, long us);

extern std::mutex log_mutex;

class MyLogger
{
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

extern MyLogger log;

#endif
