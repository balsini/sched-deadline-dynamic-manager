#include "dbus_easy.hpp"

#include <thread>
#include <chrono>
#include <string.h>
#include <string>

#include <glib.h>

#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>

#include "util.hpp"
#include "xml_manager.hpp"
#include "controller_local.hpp"
#include "controller_global.hpp"

/* This defines the per-instance state.

   Each GObject must start with the 'parent' definition so that common
   operations that all GObjects support can be called on it. */
typedef struct {
  /* The parent class object state. */
  GObject parent;
  /* Our first per-object state variable. */
  gint value1;
} ValueObject;

/* Per class state.
   For the first Value implementation we only have the bare minimum,
   that is, the common implementation for any GObject class. */
typedef struct {
  /* The parent class state. */
  GObjectClass parent;
} ValueObjectClass;

/* Forward declaration of the function that will return the GType of
   the Value implementation. Not used in this program since we only
   need to push this over the D-Bus. */
GType value_object_get_type(void);

/* Macro for the above. It is common to define macros using the
   naming convention (seen below) for all GType implementations,
   and that's why we're going to do that here as well. */
#define VALUE_TYPE_OBJECT (value_object_get_type())

#define VALUE_OBJECT(object) \
  (G_TYPE_CHECK_INSTANCE_CAST((object), \
  VALUE_TYPE_OBJECT, ValueObject))
#define VALUE_OBJECT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), \
  VALUE_TYPE_OBJECT, ValueObjectClass))
#define VALUE_IS_OBJECT(object) \
  (G_TYPE_CHECK_INSTANCE_TYPE((object), \
  VALUE_TYPE_OBJECT))
#define VALUE_IS_OBJECT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), \
  VALUE_TYPE_OBJECT))
#define VALUE_OBJECT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj), \
  VALUE_TYPE_OBJECT, ValueObjectClass))

/* Utility macro to define the value_object GType structure. */
G_DEFINE_TYPE(ValueObject, value_object, G_TYPE_OBJECT)

gboolean value_object_control(ValueObject* obj,
                              guint pid,
                              guint response_time,
                              GError** error);
gboolean value_object_launch(ValueObject* obj,
                             gchar * command,
                             gchar * args,
                             guint response_time,
                             GError** error);
gboolean value_object_fixed_add(ValueObject* obj,
                                guint pid,
                                guint runtime,
                                guint deadline,
                                guint period,
                                GError** error);
gboolean value_object_fixed_launch(ValueObject* obj,
                                   gchar * command,
                                   gchar * args,
                                   guint runtime,
                                   guint deadline,
                                   guint period,
                                   GError** error);
gboolean value_object_xml(ValueObject* obj,
                          gchar * path,
                          GError** error);

#include "ProcessManager-server-stub.h"

static void value_object_init(ValueObject* obj)
{
  g_assert(obj != NULL);

  log("DBUS", "Called");
}

static void value_object_class_init(ValueObjectClass* klass)
{
  g_assert(klass != NULL);

  log("DBUS", "Called");
  log("DBUS", "Binding to GLib/D-Bus");

  /* Time to bind this GType into the GLib/D-Bus wrappers.
   NOTE: This is not yet "publishing" the object on the D-Bus, but
         since it is only allowed to do this once per class
         creation, the safest place to put it is in the class
         initializer.
         Specifically, this function adds "method introspection
         data" to the class so that methods can be called over
         the D-Bus. */
  dbus_g_object_type_install_info(VALUE_TYPE_OBJECT,
                                  &dbus_glib_value_object_object_info);

  log("DBUS", "Done");
  /* All done. Class is ready to be used for instantiating objects */
}

gboolean value_object_control(ValueObject* obj,
                              guint pid,
                              guint response_time,
                              GError** error)
{
  g_assert(obj != NULL);

  log("DBUS", "control_process, pid", pid);
  log("DBUS", "control_process, response_time ", response_time);
  controller_local_control(pid, response_time);

  return true;
}

gboolean value_object_launch(ValueObject* obj,
                             gchar * command,
                             gchar * args,
                             guint response_time,
                             GError** error)
{
  g_assert(obj != NULL);

  std::string path;
  std::string arguments;

  path = command;
  arguments = args;

  log("DBUS", "launch_process, path", path);
  log("DBUS", "launch_process, with arguments", arguments);
  log("DBUS", "launch_process, response_time ", response_time);

  controller_local_launch_and_control(path, arguments, response_time);

  log("DBUS", "launch_process, launched, freeing the command");

  // There is probably a bug in g_free or documentation is wrong.
  // Who cares about memory leakage?
  //g_free(command);

  return true;
}

gboolean value_object_fixed_add(ValueObject* obj,
                                guint pid,
                                guint runtime,
                                guint deadline,
                                guint period,
                                GError** error)
{
  g_assert(obj != NULL);

  log("DBUS", "fixed_add, pid", pid);
  log("DBUS", "fixed_add, runtime ", runtime);
  log("DBUS", "fixed_add, deadline ", deadline);
  log("DBUS", "fixed_add, period ", period);
  controller_global_add_fixed(pid, runtime, deadline, period);

  return true;
}

gboolean value_object_fixed_launch(ValueObject* obj,
                                   gchar * command,
                                   gchar * args,
                                   guint runtime,
                                   guint deadline,
                                   guint period,
                                   GError** error)
{
  g_assert(obj != NULL);
  std::string path;
  std::string arguments;

  path = command;
  arguments = args;

  log("DBUS", "fixed_launch, path", path);
  log("DBUS", "fixed_launch, with arguments", arguments);
  log("DBUS", "fixed_launch, response_time ", runtime);
  log("DBUS", "fixed_launch, response_time ", deadline);
  log("DBUS", "fixed_launch, response_time ", period);

  controller_global_launch(path, arguments, runtime, deadline, period);

  log("DBUS", "launch_process, launched, freeing the command");

  // There is probably a bug in g_free or documentation is wrong.
  // Who cares about memory leakage?
  //g_free(command);

  return true;
}

gboolean value_object_xml(ValueObject* obj,
                          gchar * path,
                          GError** error)
{
  g_assert(obj != NULL);
  std::string pathStr;

  pathStr = path;

  log("DBUS", "xml, path", pathStr);

  xml_parse(pathStr);

  // There is probably a bug in g_free or documentation is wrong.
  // Who cares about memory leakage?
  //g_free(command);

  return true;
}

void dbus_easy_init()
{
  log("DBUS", "main Connecting to the Session D-Bus");

  GError * error = NULL;
  DBusGConnection * bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
  if (bus == NULL) {
    /* Print error and terminate. */
    log("DBUS", "Couldn't connect to session bus");
    g_error_free(error);
  }

  log("DBUS", "main Registering the well-known name", VALUE_SERVICE_NAME);

  /* In order to register a well-known name, we need to use the
       "RequestMethod" of the /org/freedesktop/DBus interface. Each
       bus provides an object that will implement this interface.

       In order to do the call, we need a proxy object first.
       DBUS_SERVICE_DBUS = "org.freedesktop.DBus"
       DBUS_PATH_DBUS = "/org/freedesktop/DBus"
       DBUS_INTERFACE_DBUS = "org.freedesktop.DBus" */
  DBusGProxy * busProxy = dbus_g_proxy_new_for_name(bus,
                                                    DBUS_SERVICE_DBUS,
                                                    DBUS_PATH_DBUS,
                                                    DBUS_INTERFACE_DBUS);
  if (busProxy == NULL) {
    log("DBUS", "Failed to get a proxy for D-Bus",
        "Unknown(dbus_g_proxy_new_for_name)");
  }

  /* Attempt to register the well-known name.
       The RPC call requires two parameters:
       - arg0: (D-Bus STRING): name to request
       - arg1: (D-Bus UINT32): flags for registration.
         (please see "org.freedesktop.DBus.RequestName" in
          http://dbus.freedesktop.org/doc/dbus-specification.html)
       Will return one uint32 giving the result of the RPC call.
       We're interested in 1 (we're now the primary owner of the name)
       or 4 (we were already the owner of the name, however in this
       application it wouldn't make much sense).

       The function will return FALSE if it sets the GError. */
  guint result;
  if (!dbus_g_proxy_call(busProxy,
                         /* Method name to call. */
                         "RequestName",
                         /* Where to store the GError. */
                         &error,
                         /* Parameter type of argument 0. Note that
                                                                                                                      * since we're dealing with GLib/D-Bus
                                                                                                                      * wrappers, you will need to find a suitable
                                                                                                                      * GType instead of using the "native" D-Bus
                                                                                                                      * type codes. */
                         G_TYPE_STRING,
                         /* Data of argument 0. In our case, this is
                                                                                                                      * the well-known name for our server
                                                                                                                      * example ("org.maemo.Platdev_ex"). */
                         VALUE_SERVICE_NAME,
                         /* Parameter type of argument 1. */
                         G_TYPE_UINT,
                         /* Data of argument 0. This is the "flags"
                                                                                                                      * argument of the "RequestName" method which
                                                                                                                      * can be use to specify what the bus service
                                                                                                                      * should do when the name already exists on
                                                                                                                      * the bus. We'll go with defaults. */
                         0,
                         /* Input arguments are terminated with a
                                                                                                                      * special GType marker. */
                         G_TYPE_INVALID,
                         /* Parameter type of return value 0.
                                                                                                                      * For "RequestName" it is UINT32 so we pick
                                                                                                                      * the GType that maps into UINT32 in the
                                                                                                                      * wrappers. */
                         G_TYPE_UINT,
                         /* Data of return value 0. These will always
                                                                                                                      * be pointers to the locations where the
                                                                                                                      * proxy can copy the results. */
                         &result,
                         /* Terminate list of return values. */
                         G_TYPE_INVALID)) {
    log("DBUS", "D-Bus.RequestName RPC failed", error->message);
    /* Note that the whole call failed, not "just" the name
         registration (we deal with that below). This means that
         something bad probably has happened and there's not much we can
         do (hence program termination). */
  }

  /* Check the result code of the registration RPC. */

  log("DBUS", "main RequestName returned", result);
  if (result != 1) {
    log("DBUS", "Failed to get the primary well-known name.", "RequestName result != 1");
    /* In this case we could also continue instead of terminating.
         We could retry the RPC later. Or "lurk" on the bus waiting for
         someone to tell us what to do. If we would be publishing
         multiple services and/or interfaces, it even might make sense
         to continue with the rest anyway.

         In our simple program, we terminate. Not much left to do for
         this poor program if the clients won't be able to find the
         Value object using the well-known name. */
  }

  log("DBUS", "main Creating one Value object");
  ValueObjectClass * valueObj = (ValueObjectClass *)g_object_new(VALUE_TYPE_OBJECT, NULL);
  if (valueObj == NULL)
    log("DBUS", "Failed to create one Value instance.", "Unknown(OOM?)");

  log("DBUS", "main Registering it on the D-Bus");

  dbus_g_connection_register_g_object(bus,
                                      VALUE_SERVICE_OBJECT_PATH,
                                      G_OBJECT(valueObj));

  log("DBUS", "main Ready to serve requests (daemonizing)");
}

void dbus_easy_thread()
{
  dbus_easy_init();

  log("dbus_easy_thread", "Initialized and running loop");

  GMainLoop * loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);
}
