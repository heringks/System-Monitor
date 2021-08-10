#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>
#include <iostream>
using namespace LinuxParser;
using namespace std;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    // Call the CpuUtilization function in the LinuxParser namespace to parse the /proc/stat file to get the data necessary to calculate the CPU utilization
    vector<string> cpu_util_text = CpuUtilization();

    // Calculate the aggregate CPU utilization

    // Conver the vector of strings into integer values representing the fields of the cpu line in the /proc/stat file
    user = stof(cpu_util_text[kUser_],nullptr);
    nice = stof(cpu_util_text[kNice_],nullptr);
    system = stof(cpu_util_text[kSystem_],nullptr);
    idle = stof(cpu_util_text[kIdle_],nullptr);
    iowait = stof(cpu_util_text[kIOwait_],nullptr);
    irq = stof(cpu_util_text[kIRQ_],nullptr);
    softirq = stof(cpu_util_text[kSoftIRQ_],nullptr);
    steal = stof(cpu_util_text[kSteal_],nullptr);

    // Algorithm to calulate CPU utilization
 
    float PrevIdle = previdle + previowait;
    float Idle = idle + iowait;

    float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    float NonIdle = user + nice + system + irq + softirq + steal;

    float PrevTotal = PrevIdle + PrevNonIdle;
    float Total = Idle + NonIdle;

    // differentiate: actual value minus the previous one
    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    utilization_ = (totald != 0.0) ? (totald - idled)/totald : 0.0;
 
    // Update previous values of the fields of the cpu line
    prevuser = user;
    prevnice = nice;
    prevsystem = system;
    previdle = idle;
    previowait = iowait;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal;

    // Return CPU utilization
    return utilization_; 
}