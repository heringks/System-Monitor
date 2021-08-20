#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);                        // Constructor
  void Set_cpu_utilization(float cpu_utilization);  // Mutator for process's CPU utilization

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // Public data
  LinuxParser::process_CPU_util_data_type process_CPU_util_data;

  // TODO: Declare any necessary private members
 private:
  int pid_;
  float cpu_utilization_;
};

#endif