#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System ();                          // Constructor
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  void CalculateProcessesCpuUtilization();

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};

  // A toggle used to ensure that the file containing both the total # of processes and the # of running processes is only parsed once per cycle
  bool num_processes_read_toggle_ = false;

  // Vector containing all the process IDs
  std::vector<int> pids_ = {};

  // Number of processes
  int num_processes_ = 0;
};

#endif