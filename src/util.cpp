#include "util.hpp"

#include <ctime>

MyLogger log;
std::mutex log_mutex;

void timespec_add_us(struct timespec * t, long us)
{
  t->tv_nsec += us*1000;
  if (t->tv_nsec > 1000000000) {
    t->tv_nsec = t->tv_nsec - 1000000000;// + ms*1000000;
    t->tv_sec += 1;
  }
}
