#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:

  // The calculated aggregate CPU utilization
  float utilization_;

  // Previous values of the CPU utilization fields from /proc/stat
  float prevuser = 0;
  float prevnice = 0;
  float prevsystem = 0;
  float previdle = 0;
  float previowait = 0;
  float previrq = 0;
  float prevsoftirq = 0;
  float prevsteal = 0;

  // Current values of the CPU utilization fields from /proc/stat
  float user, nice, system, idle, iowait, irq, softirq, steal;
};

#endif