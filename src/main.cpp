#include <thread>

#include "controller_global.hpp"
#include "dbus_easy.hpp"

int main()
{
  // Creating threads

  std::thread cg_thread(controller_global_thread);
  std::thread dbus_thread(dbus_easy_thread);

  // Running, joining threads

  cg_thread.join();
  dbus_thread.join();

  return 0;
}
