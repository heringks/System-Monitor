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
    user = stoi(cpu_util_text[0],nullptr,10);
    nice = stoi(cpu_util_text[1],nullptr,10);
    system = stoi(cpu_util_text[2],nullptr,10);
    idle = stoi(cpu_util_text[3],nullptr,10);
    iowait = stoi(cpu_util_text[4],nullptr,10);
    irq = stoi(cpu_util_text[5],nullptr,10);
    softirq = stoi(cpu_util_text[6],nullptr,10);
    steal = stoi(cpu_util_text[7],nullptr,10);

    // Algorithm to calulate CPU utilization
 
    int PrevIdle = previdle + previowait;
    int Idle = idle + iowait;

    int PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    int NonIdle = user + nice + system + irq + softirq + steal;

    int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

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