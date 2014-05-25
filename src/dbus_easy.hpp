#ifndef DBUS_EASY_HPP
#define DBUS_EASY_HPP

/* Well-known name for this service. */
#define VALUE_SERVICE_NAME        "core.sched.dl"
/* Object path to the provided object. */
#define VALUE_SERVICE_OBJECT_PATH "/"
/* And we're interested in using it through this interface.
   This must match the entry in the interface definition XML. */
#define VALUE_SERVICE_INTERFACE   "core.sched.dl"

void dbus_easy_thread();

#endif
