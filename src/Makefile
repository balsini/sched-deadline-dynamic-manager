CXXFLAGS = -Wall --std=c++11
CFLAGS = -Wall
LDFLAGS = -lpthread -ldbus-1
DBUSINCLUDE = $(shell pkg-config --libs --cflags dbus-1 dbus-glib-1 glib-2.0)
#DBUSINCLUDE = $(shell pkg-config dbus-glib-1 --cflags)
CPP = g++
LD = g++
RM = rm -rf

.SUFFIXES:
.SUFFIXES: .o .cpp

EXECS = sched-deadline-dynamic-manager

interface_xml = dbus_interface.xml
dbus_headers = ProcessManager-server-stub.h
pugixmlBin = pugixml.o

SRCS_CPP = \
	main.cpp util.cpp \
	controller_global.cpp \
	controller_local.cpp \
	sched_deadline_mananger.cpp \
	task_info_retriever.cpp \
	dl_syscalls.cpp \
	request_handler.cpp \
	dbus_easy.cpp \
	xml_manager.cpp
OBJS_CPP = ${SRCS_CPP:.cpp=.o}

all: $(pugixmlBin) $(dbus_headers) $(EXECS)

.cpp.o:
	$(CPP) $(CXXFLAGS) -c $< $(DBUSINCLUDE)

sched-deadline-dynamic-manager: $(OBJS_CPP) $(pugixmlBin)
	$(LD) -o $@ $^ $(LDFLAGS) $(DBUSINCLUDE)

ProcessManager-server-stub.h: $(interface_xml)
	dbus-binding-tool --prefix=value_object --mode=glib-server $< > $@

pugixml.o: pugixml/src/pugixml.cpp
	$(CPP) $(CXXFLAGS) -c $<
	
checkxml: $(interface_xml)
	@xmllint --valid --noout $<
	@echo $< : correct

value-client-stub.h: $(interface_xml)
	dbus-binding-tool --prefix=value_object --mode=glib-client $< > $@

clean:
	$(RM) *.o
	$(RM) $(EXECS)
	$(RM) $(dbus_headers)

cleanedit:
	$(RM) -rf *~
