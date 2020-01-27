#include "processor.h"

#include <iostream>

Processor::Processor() {
  prev_idle_ = LinuxParser::IdleJiffies();
  prev_nonidle_ = LinuxParser::ActiveJiffies();
}

float Processor::Utilization() {
  int PrevIdle = prev_idle_;
  int PrevNonIdle = prev_nonidle_;
  int PrevTotal = PrevIdle + PrevNonIdle;

  int Idle = LinuxParser::IdleJiffies();
  int NonIdle = LinuxParser::ActiveJiffies();
  long Total = LinuxParser::Jiffies();

  // differentiate: actual value minus the previous one;
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  float CPU_Percentage = (totald - idled) / totald;

  prev_nonidle_ = NonIdle;
  prev_idle_ = Idle;

  if (totald == 0.f)
    return 0.f;
  else
    return CPU_Percentage;
}