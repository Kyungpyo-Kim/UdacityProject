#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, float cpu_util, string user, string ram, long uptime,
                 string command)
    : pid_(pid),
      cpu_util_(cpu_util),
      user_(user),
      ram_(ram),
      uptime_(uptime),
      command_(command) {}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpu_util_; }

string Process::Command() const { return command_; }

string Process::Ram() const { return ram_; }

string Process::User() const { return user_; }

long int Process::UpTime() const { return uptime_; }

bool Process::operator<(Process const& a) const {
  return cpu_util_ < a.CpuUtilization();
}