#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
bool Read_processes_info_from_proc_stat(bool read_processes_info_complete);
int TotalProcesses(bool* read_processes_info_complete);
int RunningProcesses(bool* read_processes_info_complete);
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

// Structure type to contain the data from /proc/[PID]/stat needed to calculate a process's CPU utilization
struct process_CPU_util_data_type {
  float utime; // CPU time spent in user code, measured in clock ticks
  float stime; // CPU time spent in kernel code, measured in clock ticks
  float cutime; // Waited-for children's CPU time spent in user code (in clock ticks)
  float cstime; // Waited-for children's CPU time spent in kernel code (in clock ticks)
  float starttime; // Time when the process started, measured in clock ticks
};

std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();
process_CPU_util_data_type Get_process_CPU_utilization_data(int pid);

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(std::string uid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif