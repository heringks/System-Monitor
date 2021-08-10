#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stoi;
using std::stof;
using std::stol;

// Global Data
std::string total_processes = {};
std::string running_processes = {};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  // Declare local data
  string line;
  string key;
  string value;
  float MemTotal = 0.01; // Initialized to non-zero value to avoid divide by zero in case unable to read the file
  float MemFree = 0.0;
  float Total_used_memory = 0.0;
  float memory_util = 0.0;

  // Determine the file to be parsed
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);

  // If able to open the file
  if (filestream.is_open()) {

    // Read a line of data while there are lines to read
    while (std::getline(filestream, line)) {

      // Parse the line by extracting the data needed for the memory utilization formula
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal") {
        MemTotal = stof(value,nullptr);
      }
      else if (key == "MemFree") {
        MemFree = stof(value,nullptr);
      }
    }  // end while
  }  // end if

  // Calculate memory utilization
  Total_used_memory = MemTotal - MemFree;
  memory_util = Total_used_memory / MemTotal;

  return memory_util;
}  // end LinuxParser::MemoryUtilization()

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime_str;
  string line;
  long uptime = 0;

  // Determine the file to be parsed
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  // If able to open the file
  if (stream.is_open()) {

    // Read the line, get the first value which is the uptime
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str;

    // Convert the sting into a long integer
    uptime = stol(uptime_str,nullptr,10);
  }
 
  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string cpu_txt;
  string line;
  string number;
  vector<string> cpu_data_agg{};

  // Determine the file to be parsed
  std::ifstream stream(kProcDirectory + kStatFilename);

  // If able to open the file
  if (stream.is_open()) {

    // Read the 1st line of data, which provides the aggregate CPU information
    std::getline(stream, line);

    // Parse the line by extracting the data needed for the CPU utilization formula
    std::istringstream linestream(line);
    linestream >> cpu_txt;
    for (int i = 0; i < 10; i++) {
      linestream >> number;
      cpu_data_agg.push_back(number);
    }
  }

  // Return the aggregate CPU information
  return cpu_data_agg;

}  // end LinuxParser::CPUUtilization()

bool LinuxParser::Read_processes_info_from_proc_stat(bool read_processes_info_complete) {
  string key;
  string value;
  string line;
  bool read_file_success = false;

  // If the /proc/stat file has not already been parsed during this iteration to read the number of processes
  if (!read_processes_info_complete) {

    // Determine the file to be parsed
    std::ifstream stream(kProcDirectory + kStatFilename);
      
    // If able to open the file
    if (stream.is_open()) {

      // Read a line of data while there are lines to read
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> value;

        // Read total number of processes
        if (key == "processes") {
          total_processes = value;
        } // end if

        // Read number of running processes
        else if (key == "procs_running") {
          running_processes = value;
        } // end else if
      }  // end while

      // Indicate the read of the file was successful
      read_file_success = true;    
    } // end if
  }  //end if

  return read_file_success;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses(bool* read_processes_info_complete){

  // Call the function to parse the /proc/stat file to get the two lines needed for # of processes
  *read_processes_info_complete = Read_processes_info_from_proc_stat(*read_processes_info_complete);

  // Convert the string into an integer and return  
  return std::stoi(total_processes,nullptr,10); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses(bool* read_processes_info_complete) { 

  // Call the function to parse the /proc/stat file to get the two lines needed for # of processes
  *read_processes_info_complete = Read_processes_info_from_proc_stat(*read_processes_info_complete);

  // Convert the string into an integer and return  
  return std::stoi(running_processes,nullptr,10); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { 
  string uptime;
  string line;

  // Determine the file to be parsed
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  // If able to open the file
  if (stream.is_open()) {

    // Read the line, get the first value which is the uptime
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
 
  return stol(uptime,nullptr,10); 
}
