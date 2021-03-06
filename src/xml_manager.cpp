#include "xml_manager.hpp"

#include <string>

#include "controller_local.hpp"
#include "controller_global.hpp"
#include "util.hpp"
#include "pugixml/src/pugixml.hpp"

void xml_parse(const std::string &path)
{
  log("parsing file", path);

  std::string algorithm;
  std::string executablePath;
  std::string args;
  pugi::xml_document doc;

  pugi::xml_parse_result result;

  result = doc.load_file(path.c_str());

  pugi::xml_node SchedulingAlgorithm = doc.child("SchedulingAlgorithm");
  /*
  std::cout
      << "Load result: " << result.description() << std::endl
      << "SchedulingAlgorithm name: " << SchedulingAlgorithm.attribute("name").value() << std::endl
      << "path: " << SchedulingAlgorithm.child_value("path") << std::endl
      << "args: " << SchedulingAlgorithm.child_value("args") << std::endl
      << "period: " << SchedulingAlgorithm.child_value("period") << std::endl
      << "deadline: " << SchedulingAlgorithm.child_value("deadline") << std::endl
      << "runtime: " << SchedulingAlgorithm.child_value("runtime") << std::endl
      << std::endl;
  */


  algorithm = SchedulingAlgorithm.attribute("name").value();

  if (algorithm == "SCHED_DEADLINE") {
    executablePath = SchedulingAlgorithm.child_value("path");
    args = SchedulingAlgorithm.child_value("args");

    controller_global_launch(executablePath,
                             args,
                             atoi(SchedulingAlgorithm.child_value("runtime")),
                             atoi(SchedulingAlgorithm.child_value("deadline")),
                             atoi(SchedulingAlgorithm.child_value("period")));
  } else if (algorithm == "QoS_Feedback") {
    executablePath = SchedulingAlgorithm.child_value("path");
    args = SchedulingAlgorithm.child_value("args");

    controller_local_launch_and_control(executablePath,
                                        args,
                                        atoi(SchedulingAlgorithm.child_value("responsetime")));
  }
}
