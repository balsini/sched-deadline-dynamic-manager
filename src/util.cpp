#include "util.hpp"

#include <vector>
#include <ctime>

MyLogger log;

void timespec_add_us(struct timespec * t, long us)
{
  t->tv_nsec += us*1000;
  if (t->tv_nsec > 1000000000) {
    t->tv_nsec = t->tv_nsec - 1000000000;// + ms*1000000;
    t->tv_sec += 1;
  }
}

char ** parse_arguments(const std::string &argsString)
{
  char ** argv;
  std::vector<std::string> argsVector;
  std::string s;
  auto it = argsString.begin();

  while (it != argsString.end()) {
    s.clear();

    while (it != argsString.end() && *it == ' ')
      ++it;
    while (it != argsString.end() && *it != ' ') {
      s.push_back(*it);
      ++it;
    }

    argsVector.push_back(s);
  }

  argv = new char*[argsVector.size() + 1];
  argv[argsVector.size()] = NULL;

  for (unsigned int i=0; i<argsVector.size(); i++) {
    int argLen = argsVector.at(i).length();
    argv[i] = new char[argLen + 1];
    for (int j=0; j<argLen; j++) {
      argv[i][j] = argsVector.at(i).at(j);
    }
    argv[i][argLen] = '\0';
  }

  return argv;
}
