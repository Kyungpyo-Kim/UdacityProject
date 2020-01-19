#include "processor.h"

#include <iostream>

Processor::Processor() {
  vector<string> utils;
  while (utils.size() != 10) 
    utils = LinuxParser::CpuUtilization();
  prevuser_ = stoi(utils[0]);
  prevnice_ = stoi(utils[1]);
  prevsystem_ = stoi(utils[2]);
  previdle_ = stoi(utils[3]);
  previowait_ = stoi(utils[4]);
  previrq_ = stoi(utils[5]);
  prevsoftirq_ = stoi(utils[6]);
  prevsteal_ = stoi(utils[7]);
}

float Processor::Utilization() {
  vector<string> utils;
  while (utils.size() != 10) utils = LinuxParser::CpuUtilization();

  int user, nice, system, idle, iowait, irq, softirq, steal;
  user = stoi(utils[0]);
  nice = stoi(utils[1]);
  system = stoi(utils[2]);
  idle = stoi(utils[3]);
  iowait = stoi(utils[4]);
  irq = stoi(utils[5]);
  softirq = stoi(utils[6]);
  steal = stoi(utils[7]);

  int PrevIdle = previdle_ + previowait_;
  int Idle = idle + iowait;
  int PrevNonIdle = prevuser_ + prevnice_ + prevsystem_ + previrq_ +
                    prevsoftirq_ + prevsteal_;
  int NonIdle = user + nice + system + irq + softirq + steal;
  int PrevTotal = PrevIdle + PrevNonIdle;
  int Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one;
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  float CPU_Percentage = (totald - idled) / totald;

  prevuser_ = stoi(utils[0]);
  prevnice_ = stoi(utils[1]);
  prevsystem_ = stoi(utils[2]);
  previdle_ = stoi(utils[3]);
  previowait_ = stoi(utils[4]);
  previrq_ = stoi(utils[5]);
  prevsoftirq_ = stoi(utils[6]);
  prevsteal_ = stoi(utils[7]);

  if (totald == 0.f)
    return 0.f;
  else
    return CPU_Percentage;
}