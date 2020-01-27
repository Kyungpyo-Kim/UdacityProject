#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, float cpu_util, string user, string ram, long uptime,
          string command);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;

 private:
  int pid_;
  float cpu_util_;
  string user_;
  string ram_;
  long uptime_;
  string command_;
};

#endif