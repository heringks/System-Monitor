#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// System class constructor
System::System () {

    // Determine all the process IDs, store them in a vector, and store the # of proceses
    pids_ = LinuxParser::Pids();
    num_processes_ = pids_.size();

    // Construct the processes based on the pids
    for (int i = 0; i < num_processes_; i++) {
        processes_.emplace_back(pids_[i]);
    }
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {

    // Calculate the CPU utilization of all the processes
    CalculateProcessesCpuUtilization();

    // Sort the processes based on CPU utilization, from highest to lowest
    std::sort(processes_.begin(), processes_.end());

    // Update the vector of process IDs to match the sorted order
    for (int i = 0; i < num_processes_; i++) {
        pids_[i] = processes_[i].Pid();
    } // end for

    return processes_; 
} // end function

void System::CalculateProcessesCpuUtilization() {
    const float k_nom = 0.01; // A nominal value to check in the floating point division to avoid divide by zero
    float total_time;
    float hertz;
    float seconds;
    float cpu_usage;
    LinuxParser::process_CPU_util_data_type CPU_util_data;

    // Determine the CPU utilization of all the processes
    for (int i = 0; i < num_processes_; i++) {

        // Get the CPU utilization data
        CPU_util_data = LinuxParser::Get_process_CPU_utilization_data(pids_[i]);

        // Compute the CPU utilization of the process
        // Lines 49 - 52 derived from "Calculation" shown on https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
        total_time = CPU_util_data.utime + CPU_util_data.stime + CPU_util_data.cutime + CPU_util_data.cstime;
        hertz = sysconf(_SC_CLK_TCK);
        seconds = (hertz > k_nom) ? (LinuxParser::UpTime() - (CPU_util_data.starttime / hertz)) : 0.0;
        cpu_usage = (seconds > k_nom) ? ((total_time / hertz) / seconds) : 0.0;
        processes_[i].Set_cpu_utilization(cpu_usage);

        // Store down the CPU utilization data for the process
        processes_[i].process_CPU_util_data = CPU_util_data;
    } // end for

}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(&num_processes_read_toggle_);
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(&num_processes_read_toggle_);
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }