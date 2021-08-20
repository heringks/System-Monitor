#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "system.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Define the class constructor
Process::Process(int pid) {
    pid_ = pid;
    cpu_utilization_ = 0.0;
}

// Define the mutator for the process's CPU utilization
void Process::Set_cpu_utilization(float cpu_utilization) {
    cpu_utilization_ = cpu_utilization;
}

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    return cpu_utilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    string ram = {};
    long int ramlmb = 0; // Converted into long integer in units of megabytes
    const long int k_kb_to_mb = 1024;

    // Get the RAM of the process from /proc/[PID]/status
    ram = LinuxParser::Ram(pid_);

    // Convert from KB to MB and back into a string
    ramlmb = stol(ram,nullptr,10) / k_kb_to_mb;
    ram = to_string(ramlmb);
    return ram; 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string user = "";

    // Determine the User ID of the process
    string uid = LinuxParser::Uid(pid_);

    // If the UID is not a null string
    if (uid != "") {

      // Match the UID to the name of the user
      user = LinuxParser::User(uid);
    }

    return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    float hertz = sysconf(_SC_CLK_TCK);
    const float k_nom = 0.01; // A nominal value to check in the floating point division to avoid divide by zero

    float pid_uptime = (hertz > k_nom) ? (process_CPU_util_data.starttime / hertz): 0.0;
    return pid_uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return a.cpu_utilization_ < this->cpu_utilization_;
}