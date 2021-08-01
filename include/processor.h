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
  int prevuser = 0;
  int prevnice = 0;
  int prevsystem = 0;
  int previdle = 0;
  int previowait = 0;
  int previrq = 0;
  int prevsoftirq = 0;
  int prevsteal = 0;

  // Current values of the CPU utilization fields from /proc/stat
  int user, nice, system, idle, iowait, irq, softirq, steal;
};

#endif